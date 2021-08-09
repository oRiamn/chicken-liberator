module.exports = {
    development: {
        API_URL: 'http://localhost:3000/api/gpio',
        WS_ENDPOINT: 'ws://localhost:3000/api/gpio'
    },
    production: {
        API_URL: '/api/gpio',
        WS_ENDPOINT: undefined
    }
}