import {io} from 'https://cdn.jsdelivr.net/npm/socket.io-client/+esm';

export class Sensor {
  constructor(target) {
    this.target = target;

    this.socket = io(window.location.origin);

    // Registramos el sensor en el server
    this.socket.emit('registerSensor', this.target);
  }

  // Función para mandar un evento
  sendEvent(value){
    this.socket.emit('event', {
      target: this.target,
      value: value,
    });
  }
}