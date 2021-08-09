
const express = require('express');
const url = require('url');

const app = express();
const port = 3000;

app.use(express.json());
app.use(function (req, res, next) {
  res.setHeader('Access-Control-Allow-Origin', 'http://localhost:8080');
  res.setHeader('Access-Control-Allow-Methods', 'GET, POST, OPTIONS, PUT, PATCH, DELETE');
  res.setHeader('Access-Control-Allow-Headers', 'X-Requested-With,content-type');
  res.setHeader('Access-Control-Allow-Credentials', true);
  console.log(url.format({
    protocol: req.protocol,
    host: req.get('host'),
    pathname: req.originalUrl
  }))
  next();
});



const pins = [ 2, 4, 5, 15].map((pin)=> ({pin, state:0 }))

app.get('/api/gpio/out/:pin/:state', (req, res) => {
  const pin = req.params.pin;
  const state = req.params.state;

  const oPin = pins.find((p)=> `${p.pin}` === `${pin}`);
  if(oPin){
    oPin.state = parseInt(state);
  }

  res.json(oPin || {});
});

app.get('/api/gpio/out', (req, res) => {
  res.json(pins);
});

app.listen(port, () => {
  console.log(`Fake API listening at http://localhost:${port}`)
});