const WebSocket = require('ws');

module.exports.wsClient = (url) => new WebSocket(url)

module.exports.closeWs = (ws) =>
  new Promise((resolve) => {
    if(ws.readyState ===3){
      resolve(ws);
    }
    ws.on('close', () => {
      resolve(ws);
    })
    ws.close();
  })


module.exports.connectedSocket = (url) =>
  new Promise((resolve) => {
    const ws = new WebSocket(url)
      .on('open', () => {
        resolve(ws)
      })
  })

module.exports.socketSpy = (url, spy) =>
  new Promise((resolve) => {
    const ws = new WebSocket(url)
      .on('open', () => {
        resolve(ws)
      })
      .on('message', (message) => {
        try {
          const json = JSON.parse(message)
          spy(json)
        } catch (error) {
          spy(message)
        }
      })
  })
