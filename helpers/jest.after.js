const { stopFakeApi } = require("./fakeapi-starter.js");

module.exports = async () => {
    if (process.env.NODE_ENV === "ci") {
        await stopFakeApi();
    }
};