import {Sensor} from './Sensor';
import {Actuator} from './Actuator';

// Función para crear sensor
export function createSensor(target) {
  return new Sensor(target);
}

// Función para crear actuator
export function createActuator(target, sensor) {
  return new Actuator(target, sensor);
}