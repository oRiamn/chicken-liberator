const pm2 = require('pm2');

module.exports.startFakeApi = async () => new Promise((r) => {
    pm2.start({
        name: 'fakeapi',
        script: 'helpers/fakeapi.js',
        wait_ready: true,
        listen_timeout: 10000
    }, function () {
        r();
    });
})

module.exports.stopFakeApi = () => new Promise((r) => {
    pm2.delete('all', function () {
        pm2.disconnect();
        r();
    });
});