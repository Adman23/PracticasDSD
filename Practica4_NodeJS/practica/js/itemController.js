import {createSensor, createActuator} from "./factory.js";

// Conexión al servidor
const socket = io(window.location.origin);

let sensors = {};
let actuators = {};

const formAddSensor = document.getElementById("create-sensor-form");
const formAddActuator = document.getElementById("create-actuator-form");

formAddSensor.addEventListener('submit', (event) => {
  event.preventDefault();
  createNewSensor(event.target.elements["sensor-target"].value.trim().toLowerCase());
})

formAddActuator.addEventListener('submit', (event) => {
  event.preventDefault();
  createNewActuator(event.target.elements['actuator-target'].value.trim().toLowerCase(),
                    event.target.elements['sensor-friend'].value.trim().toLowerCase());
})



// Escuchar cambios en actuadores
socket.on('actuatorChange', (change) => {
  const { target, state } = change;
  if (actuators[target]) {
    let stateP = document.getElementById(`${target}_actuator`);
    stateP.textContent = state;
  }
});


function createNewSensor(target) {

  if(!target) {
    alert("Cant get the sensor target!");
    return;
  }

  if (sensors[target]) {
    alert("Sensor already exists!");
    return;
  }

  // Creamos el sensor
  sensors[target] = createSensor(target);
  console.log(`Se ha creado sensor (itemController)`);


  // Colocamos el sensor en lista
  const sensor_list = document.getElementById("sensor-list");
  const li = document.createElement("li");

  const head = document.createElement("h3");
  const last_value = document.createElement("p");
  console.log(target);
  head.textContent = target;
  last_value.textContent = "-";

  // Escuchamos los eventos del sensor para cambiar "last_value"
  sensors[target].socket.on('event', (event) => {
      if (event.target === target) {
        last_value.textContent = event.value;
      }
  })

  const event_form = document.createElement("form");
  const label = document.createElement("label");
  const input = document.createElement("input");
  const button = document.createElement("button");

  label.setAttribute("for", "value");
  input.setAttribute("id", "value");
  input.setAttribute("name", "value");
  input.setAttribute("type", "number");
  input.required = true;

  button.setAttribute("type", "submit");
  button.textContent = "send event";

  event_form.append(label);
  event_form.append(input);
  event_form.append(button);

  event_form.addEventListener("submit", (event) => {
    event.preventDefault();
    console.log("Sending...");
    let value = input.value;
    sensors[target].sendEvent(value);
  })

  li.append(head);
  li.append(last_value);
  li.append(event_form);

  sensor_list.appendChild(li);
}


// Función para crear un nuevo actuador
function createNewActuator(target, sensor_friend){

  if(target === "") {
    alert("Cant get the actuator target!");
    return;
  }

  if (sensor_friend === "") {
    sensor_friend = "no sensor";
  }

  actuators[target] = createActuator(target, sensor_friend);
  console.log(`Se ha creado actuador (itemController)`);


  // Colocamos el actuador en lista
  const actuator_list = document.getElementById("actuator-list");
  const li = document.createElement("li");

  const head = document.createElement("h3");
  const sensor = document.createElement("p");
  const state = document.createElement("p");
  head.textContent = target;
  sensor.textContent = sensor_friend;
  state.textContent = actuators[target].state;
  state.id = `${target}_actuator`;




  li.append(head);
  li.append(sensor);
  li.append(state);

  actuator_list.appendChild(li);
}


// Añadidos iniciales
createNewSensor("temperature");
createNewSensor("luminosity");

createNewActuator("ac", "temperature");
createNewActuator("blinds","luminosity");