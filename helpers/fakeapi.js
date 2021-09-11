
const express = require('express');
const expressWs = require('express-ws');
const fs = require('fs');
const { extractCppH } = require('./vars.helper');

const logFile = '/tmp/esp8266-received.log';

const app = express();
const port = 3000;
const ws = expressWs(app);

const CPPH = extractCppH('ESP8266/libraries/LibConstants/LibConstants.template.h');

process.send = process.send || function () { };

const log = (s) => {
  fs.appendFileSync(logFile, `\r\n${s}`);
  console.log(s)
}

let socketn = 0;
app.ws('/api/gpio', function (ws, req) {
  const number = socketn++;
  ws.on('close', function () {
    log(`WS #${number} (IP unset) disconnected`)
  });

  log(`WS #${number} ${req.socket.remoteAddress.replace(/(::.*:)/, "")} connected`)
});

app.use(express.json());
app.use(express.static('dist'));
app.use(function (req, res, next) {
  res.setHeader('Access-Control-Allow-Origin', 'http://localhost:8080');
  res.setHeader('Access-Control-Allow-Methods', 'GET, POST, OPTIONS, PUT, PATCH, DELETE');
  res.setHeader('Access-Control-Allow-Headers', 'X-Requested-With,content-type');
  res.setHeader('Access-Control-Allow-Credentials', true);
  next();
});


const logHttp = (req) => log(`HTTP ${req.method} ${req.url}`);

const pins = CPPH.OUTPUT_PINS.map((pin) => ({ pin, state: 0 }))

app.get('/api/gpio/out/:pin/:state', (req, res) => {

  logHttp(req);

  const pin = req.params.pin;
  const state = req.params.state;

  const oPin = pins.find((p) => `${p.pin}` === `${pin}`);
  if (oPin) {
    oPin.state = parseInt(state);
    ws.getWss().clients.forEach((client) => {
      client.send(JSON.stringify(oPin));
    });

    log(`PIN SET ${JSON.stringify(oPin)}`);

  }
  res.json(oPin || {});
});

app.get('/api/gpio/out', (req, res) => {
  logHttp(req);
  res.json(pins);
});

app.listen(port, () => {
  fs.writeFileSync(logFile, 'Start fakeapi', { flag: 'w' });
  log(`SETUP Connecting to ${CPPH.SSID}`);
  log(`.......`);
  log(`SETUP Wifi connected : http://127.0.0.1:${port}\r\n`)
  process.send('ready');
});
