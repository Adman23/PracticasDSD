import http          from 'node:http';
import {join}        from 'node:path';
import {MongoClient} from 'mongodb';
import express from 'express';
import {fileURLToPath} from 'url';
import * as path from "node:path";
import {Server} from 'socket.io';
import { spawn } from 'child_process';

// Establecemos la url de la base de datos
const uri_db = "mongodb://localhost:27017";
const client_db = new MongoClient(uri_db); // "cliente"


// Establecemos rutas usando módulos ES6
const __filename = fileURLToPath(import.meta.url); // Representa url absoluta del archivo actual
const __dirname = path.dirname(__filename); // Ruta absoluta del proyecto /practica

// Establecemos express para enrutar
const app = express();
const PORT = 8080; // Puerto en el que funcionar

app.use('/js', express.static(join(__dirname, 'js')));

// Establecemos el localhost para renderizar index
app.get("/", (req, res) => {
  const filePath = join(__dirname, 'public', 'index.html');
  res.sendFile(filePath);
})

// Establecemos localhost/sensors para renderizar la interfaz de sensores
app.get("/sensors", (req, res) => {
  const filePath = join(__dirname, 'public', 'item_simulator.html');
  res.sendFile(filePath);
})

// Creamos el servidor y el socket
const server = http.createServer(app);
const io = new Server(server);

// Función para que el agente funcione en segundo plano
function launchAgent() {
  const agentProcess = spawn('node', [path.join(__dirname, 'js', 'agent.js')]);

  agentProcess.stdout.on('data', (data) => {
    console.log(`[Agente] ${data}`);
  });

  agentProcess.stderr.on('data', (data) => {
    console.error(`[Agente - ERROR] ${data}`);
  });

  agentProcess.on('close', (code) => {
    console.log(`[Agente] Proceso cerrado con código: ${code}`);
  });

  return agentProcess;
}


// Creamos la API de uso para las funciones del cliente
// Función de conexión a la base de datos (se guarda en db)
async function connectDB() {
  try{
    await client_db.connect();
    const db = client_db.db("sistemaDomotico");
    return db
  }
  catch(err){
    console.log(`Error connecting DB -> ${err}`);
  }
}


// Conectamos a la base de datos y definimos los sockets
connectDB().then((db) => {
    // Solo vamos a tener la colección de eventos
    let eventsCollection = db.collection("events");
    const connectedActuators = {
      /*
        target: {
                  id: id
                  state: true/false,
                  sensor: temperature/null,
                 }
       */
    };
    const connectedSensors = {
      /*
        sensorName: {
                    id: id
                    over: overValue, default null
                    under: underValue default null
                    }
       */
    };

    function broadcastStatus(){
      const status = {
        sensors: connectedSensors,
        actuators: connectedActuators,
      };
      io.sockets.emit('systemStatus', status);
    }

    // Definimos el comportamiento de los sockets una vez abierta la base de datos
    io.sockets.on('connection', (client) => {
      console.log(`Se ha conectado un nuevo cliente: ${client.id}`);

      // -- DEVOLVER SENSORES Y ACTUADORES CONECTADOS --
      const currentStatus = {
        sensors: connectedSensors,
        actuators: connectedActuators,
      }
      client.emit('systemStatus', currentStatus);


      // -- HISTORIAL DE EVENTOS --
      client.on('getHistory', () => {
        eventsCollection.find().sort({timestamp: -1}).limit(50).toArray()
            .then(history => client.emit('history', history));
      });

      // -- REGISTRO ACTUADOR --
      client.on('registerActuator', (act) => {
        if (!connectedActuators[act.target]) {
          connectedActuators[act.target] = {
            id: client.id,
            state: act.state,
            sensor: act.sensor,
          };
          broadcastStatus();
          console.log(`Actuador de ${act.target} registrado con id: ${client.id}`);
        }
      });

      // -- EVENTO ACTUATOR CHANGE --
      client.on('actuatorChange', (data) => {
        const { target, state } = data;
        if (connectedActuators[target]) {
          console.log(`emiting change on actuator: ${target} con ${state}`);
          connectedActuators[target].state = state;
          io.sockets.emit('actuatorChange', {target: target, state: state});
        }
        else{
          console.log("No such actuator");
        }
      })


      // -- REGISTRO SENSOR --
      client.on('registerSensor', async (target) => {
        if (!connectedSensors[target]) {
          const thresholdCollection = db.collection("thresholds");

          // Buscamos a ver si tiene umbrales
          const stored = await thresholdCollection.findOne({target: target});

          // Si tiene que se los guarde, si no tiene que sea null
          connectedSensors[target] = {
            id: client.id,
            over: stored?.thresholds?.over ?? null,
            under: stored?.thresholds?.under ?? null,
          }
          broadcastStatus();
          console.log(`Sensor de ${target} registrado con id: ${client.id}`);
        }
      });

      // -- DESCONEXIÓN --
      client.on('disconnect', () => {
        // En caso de que sea un sensor
        for (const [target,info] of Object.entries(connectedSensors)) {
          if (info.id === client.id) {
            delete connectedSensors[target];
          broadcastStatus();
          }
          console.log(`Actuador de ${target} desconectado`);
        }

        // En caso de que sea un actuador
        for (const [target,info] of Object.entries(connectedActuators)) {
          if (info.id === client.id) {
            delete connectedActuators[target];
          }
          console.log(`Actuador de ${target} desconectado`);
        }

        console.log(`Se ha desconectado cliente: ${client.id}`);
        broadcastStatus();
      });

      // -- GESTIÓN DE EVENTO --
      client.on('event', async (data) => {
        const event = { target: data.target, value: data.value, timestamp: new Date(),
          over: connectedSensors[data.target].over,
          under: connectedSensors[data.target].under};
        console.log(`evento recibido: ${data.target} y ${data.value}`);
        await eventsCollection.insertOne(event);
        io.sockets.emit('event', event);
      })

      // -- COMANDOS DE USUARIOS -- cmd -> {target: ac/blinds, state: true/false}
      client.on('command', (cmd) => {
        // Reenvía el comando en caso de que exista el tipo de actuador entre los conectados
        if (connectedActuators[cmd.target]) {
          io.sockets.emit('command', cmd);
        }
        else{
          io.sockets.emit('error', {
            message: `El actuador ${cmd.target} no está conectado`, target: cmd.target});
        }
      });

      // -- ALERTAS DE AGENTE -- alert -> {target: temperature/luminosity, event: wentOver/wentUnder}
      client.on('alert', (alert) => {
        // Reenvía el comando en caso de que exista el tipo de actuador entre los conectados
          console.log("SE REENVIA ALERTA");
        if (connectedSensors[alert.target]) {
          io.sockets.emit('alert', alert);
        }
        else{
          io.sockets.emit('error', {
            message: `El actuador ${alert.target} no está conectado`, target: alert.target});
        }
      });

      // -- ACTUALIZACIÓN DE UMBRALES --
      client.on('updateThreshold', (sensor) => {
        const { target, thresholds } = sensor;

        if ( !target || !thresholds || (isNaN(thresholds.over) || isNaN(thresholds.under)) ) {
          console.log("El usuario ha mandado datos invalidos para los umbrales");
          return;
        }

        const thresholdCollection = db.collection('thresholds');
        thresholdCollection.updateOne(
            {target: target},
            {
              $set: {
                target: target,
                thresholds: {
                  over: thresholds.over,
                  under: thresholds.under,
                },
                timestamp: new Date(),
              }
            }, {upsert: true}); // En caso de que no exista le pedimos que se cree

        console.log("Se han actualizado los umbrales");

        // Lo actualizamos también en el server
        connectedSensors[target].over = thresholds.over;
        connectedSensors[target].under = thresholds.under;

        broadcastStatus();
      });
    })


    // Lanzamos el servidor en el puerto
    server.listen(PORT, () => {
      console.log(`Server started on port ${PORT}`);
      launchAgent();
    })
  }
);

