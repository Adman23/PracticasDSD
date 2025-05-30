import {createSensor, createActuator} from "./factory.js";

let sensors = {};
let actuators = {};

function createNewSensor() {
  const target = document.getElementById("sensor-target");

  if(!target) {
    alert("Cant get the sensor target!");
    return;
  }

  // Creamos el sensor
  sensors[target] = createSensor(target);
  console.log(`Se ha creado sensor (itemController)`);

  // Colocamos el sensor en lista

}

function createNewActuator() {
  const target = document.getElementById("actuator-target");
  const sensor_friend = document.getElementById("sensor-friend");

  if(!target) {
    alert("Cant get the sensor target!");
    return;
  }

  sensors[target] = createActuator(target);
  console.log(`Se ha creado actuador (itemController)`);


  // Colocamos el actuador en lista


}