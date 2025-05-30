import {io} from "socket.io-client"

const serviceURL = "http://localhost:8080";
const socket = io(serviceURL);

// Contiene como keys los tipos de actuadores
let sensors   = {
  /*
   temperature: {
    id: id
    over: 30,
    under: 20,
   }
   */
}; // Mandará las alertas según los sensores
let actuators = {
    /*
      target: {
                id: id
                state: true/false,
                sensor: temperature/null,
               }
     */
}; // Los tiene para establecer alertas personalizadas

// Status trae tanto los sensores como los actuadores (maps)
socket.on('systemStatus', (status) => {
  sensors = status.sensors;
  actuators = status.actuators;
});

socket.on('event', (data) => {
  const { target, value } = data;
  const threshold_over = sensors[target].over ?? null;
  const threshold_under = sensors[target].under ?? null;

  // Si no tiene umbrales no tiene sentido que procese
  const num = parseFloat(value);
  let alert;
  if (threshold_over && num > threshold_over){
    alert = "over";
  }
  else if (threshold_under && num < threshold_under){
    alert = "under";
  }

  // Si no ha roto un umbral no hay alerta que poner
  if (alert)
    // buscamos el actuador
    for (const [key, value] of Object.entries(actuators)) {
      if (value.sensor === target) {
        value["buscado"] = key;
      }
    }


    if (value["buscado"]) {
      console.log(`Agente envía alerta: ${target} con ${alert}`);
      socket.emit('alert', {
        target: value["buscado"],
        event: alert,
        message: `Umbral ${alert} superado con el valor: ${num}`,
      });
      value["buscado"] = null;
    }
});

// Esto es en caso de que algún actuador cambie (puede ser por usuario o por alerta)
socket.on('actuatorChange', (data) => {
  const { target, state } = data;
  if (actuators[target]){
    actuators[target].state = state;
  }
})

console.log("Agente preparado");
