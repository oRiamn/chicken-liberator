const { startFakeApi } = require("./fakeapi-starter.js");
const { extractIp } = require("./vars.helper.js");

module.exports = async () => {
    if (process.env.NODE_ENV === "ci") {
        await startFakeApi();
        process.env.CPPH = 'ESP8266/libraries/LibConstants/LibConstants.template.h';
    } else {
        process.env.CPPH = 'ESP8266/libraries/LibConstants/LibConstants.h';
    }
    process.env.ENDPOINTIP = await extractIp();
    console.log(process.env.ENDPOINTIP)
};