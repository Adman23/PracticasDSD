import { io } from 'https://cdn.jsdelivr.net/npm/socket.io-client/+esm';

// Conexión al servidor
const socket = io(window.location.origin);

// Contenedor de los actuators
let currentActuators = {};

// --------------------------- FUNCIONES -----------------------------------------
// Mandar comando manual al actuador
function sendCommand(target, action) {
  socket.emit('command', {
    target: target,
    state: action
  });
}

// Renderizar actuadores dinámicamente
function renderActuators() {
  const container = document.getElementById("actuator-list");
  container.innerHTML = "";

  for (const act of Object.entries(currentActuators)) {
    // Elementos básicos del actuador
    const li = document.createElement("li");

    const title = document.createElement("h3");
    title.textContent = act[0];

    const state = document.createElement("p");
    state.textContent = act[1].state;
    state.id = act[0];

    // Controles para mandar comandos
    const controls = document.createElement("div");

    const btnOn = document.createElement("button");
    btnOn.textContent = "Encender";
    btnOn.addEventListener("click", () => sendCommand(act[0], true));

    const btnOff = document.createElement("button");
    btnOff.textContent = "Apagar";
    btnOff.addEventListener("click", () => sendCommand(act[0], false));

    controls.appendChild(btnOn);
    controls.appendChild(btnOff);

    li.appendChild(title);
    li.appendChild(state);
    li.appendChild(controls);

    container.appendChild(li);
  }
}

function addEventToHistory(event) {
  const historyList = document.getElementById("event-history");
  const li = document.createElement("li");
  li.textContent = `${new Date(event.timestamp).toLocaleString()} - ${event.target}: ${event.value}`;
  if (event.over) {
    li.textContent = `${li.textContent} - over: ${event.over}`;
  }
  if (event.under) {
    li.textContent = `${li.textContent} - under: ${event.under}`;
  }
  historyList.prepend(li);
}


document.getElementById("threshold-form").addEventListener("submit", (e) => {
  e.preventDefault();

  const nameInput   = document.getElementById("sensor-name");
  const overInput   = document.getElementById("value-over");
  const underInput  = document.getElementById("value-under");

  const sensorName  = nameInput.value.trim().toLowerCase();
  const overValue   = parseFloat(overInput.value);
  const underValue  = parseFloat(underInput.value);

  if (!sensorName || isNaN(overValue) || isNaN(underValue)) {
    alert("Incomplete or wrong fields");
    return;
  }

  socket.emit('updateThreshold', {
    target: sensorName,
    thresholds: {
        over: overValue,
        under: underValue
      }
  });

  console.log(`Usuario manda actualización de thresholds: ${sensorName} con over: ${overValue} y under: ${underValue}`);
});

// --------------------------- SOCKETS  -----------------------------------------
// Escuchar estado inicial del sistema
socket.on('systemStatus', (status) => {
  currentActuators = status.actuators || {};
  renderActuators();
});

// Escuchar cambios en actuadores
socket.on('actuatorChange', (change) => {
  const { target, state } = change;
  if (currentActuators[target]) {
    let stateP = document.getElementById(target);
    stateP.textContent = state;
  }
});

// Escuchar nuevos eventos (temperatura, luz...) y mostrarlos
socket.on('event', (event) => {
  addEventToHistory(event);
});

// Mostrar historial desde base de datos
socket.on('history', (events) => {
  const historyList = document.getElementById("event-history");
  historyList.innerHTML = ""; // Limpiar antes de cargar
  events.reverse().forEach(addEventToHistory); // Más reciente arriba
});

socket.on('alert', (alert) => {
  const historyList = document.getElementById("event-history");
  const li = document.createElement("li");
  li.textContent = `Alerta -> ${alert.message}`;
  historyList.prepend(li);
})

socket.emit('getHistory');





