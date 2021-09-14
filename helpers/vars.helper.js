const fs = require('fs');
const { exec } = require("child_process");

const logFile = "/tmp/esp8266-received.log";

module.exports.extractLog = async (keyword) => (
    new Promise((solve) => {
        exec(`tail -n 3 ${logFile} | grep "${keyword}" | tail -1`, (_error, stdout) => {
            solve(stdout.trim());
        })
    })
)

module.exports.extractIp = async () => (
    new Promise((solve) => {
        exec(`head -10 ${logFile} | grep -a "SETUP Wifi" | tail -1`, (_error, stdout) => {
            solve(stdout.trim().replace(/^.*http:\/\//, ''));
        })
    })
)

module.exports.extractCppH = (hfile) => {
    const constantsH = fs.readFileSync(hfile, { encoding: 'utf8' })
    const newLines = []
    const regex = /^\#define ([a-z_]+) +(.*)$/gim
    let m = regex.exec(constantsH)
    while (m !== null) {
        if (m.index === regex.lastIndex) {
            regex.lastIndex++
        }
        const [, variable, value] = m
        const washed = value
            .trim()
            .replace(/;$/, "") // remove ; in endline
            .replace(/^{(.*)}$/, (_m, elements) => `[${elements}]`) // transform array {...} to [...]

        newLines.push(`"${variable}":${washed}`)
        m = regex.exec(constantsH)
    }
    try {
        const content = JSON.parse(`{${newLines.join(',\n')}}`);
        return content;
    } catch (err) {
        // An error occurred
        console.error(err)
    }

}

