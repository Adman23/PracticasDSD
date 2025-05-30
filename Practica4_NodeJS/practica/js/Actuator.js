import {io} from 'https://cdn.jsdelivr.net/npm/socket.io-client/+esm';

export class Actuator {
  constructor(target, sensor) {
    this.target = target;
    this.state = false;
    this.sensor = sensor;

    this.socket = io(window.location.origin);

    // Registramos el actuador en el server
    this.socket.emit('registerActuator', {
      target: this.target,
      state: this.state,
      sensor: this.sensor
    });


    // Recibimos alertas del agente
    this.socket.on('alert', (alert) => {
      if (alert.target === this.sensor) {
        if (alert.event === "under"){
          this.state = false;
        }
        else if (alert.event === "over"){
          this.state = true;
        }

        this.socket.emit('actuatorChange', {
          target: this.target,
          state: this.state,
        });
      }
    });

    // Recibimos comandos del usuario
    this.socket.on('command', (cmd) => {
      if (cmd.target === this.target) {
        this.state = cmd.state;

        this.socket.emit('actuatorChange', {
          target: this.target,
          state: this.state,
        });
      }
    });
  }
}
