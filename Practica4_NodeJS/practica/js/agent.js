import {io} from "socket.io-client"

console.log(window.location.origin);
const serviceURL = window.location.origin;
const socket = io(serviceURL);

// Contiene como keys los tipos de actuadores
let sensors   = {
  /*
   temperature: {
    over: 30,
    under: 20,
    hasThreshold: true,
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
  sensors.keys = status.sensors.keys;
  // Los sensores se establecen sin thresholds al principio
  // Estos thresholds los tiene que establecer alguna interfaz
  sensors.keys.forEach((key) => {
    sensors[key] = {
      over: -1,
      under: -1,
      hasThreshold: false,
    }
  })

  actuators = status.actuators;
});

socket.on('event', (data) => {
  const { type, value } = data;
  const threshold = sensors[type];

  // Si no tiene umbrales no tiene sentido que procese
  if (threshold && threshold.hasThreshold) {
    const num = parseFloat(value);
    let alert;
    if (num > threshold.over){
      alert = "over";
    }
    else if (num < threshold.under){
      alert = "under";
    }

    // Si no ha roto un umbral no hay alerta que poner
    if (alert){
      console.log(`Agente envía alerta: ${type} con ${alert}`);
      socket.emit('alert', {
        target: type,
        event: alert,
        message: `Umbral ${alert} superado con el valor: ${num}`,
      });
    }
  }
});

// Esto es en caso de que algún actuador cambie (puede ser por usuario o por alerta)
socket.on('actuatorChange', (data) => {
  const { target, state } = data;
  if (actuators[target]){
    actuators.target.active = state;
  }
})

console.log("Agente preparado");
