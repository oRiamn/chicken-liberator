const axios = require('axios').default;
const { socketSpy, connectedSocket, closeWs } = require('../helpers/websocket.helper')
const { extractCppH, extractLog } = require('../helpers/vars.helper');

const CCPH = extractCppH(process.env.CPPH);

describe("ESP8266", () => {
    const ip = process.env.ENDPOINTIP;
    const apiuri = `api/gpio/out`;
    const apihost = `http://${ip}`;
    const wsHost = `ws://${ip}`;
    const wsApi = "api/gpio";

    const outpoutPins = CCPH.OUTPUT_PINS;

    const options = {
        method: 'GET',
    }

    describe.each(outpoutPins)
        ('for pin %s', (pin) => {
            describe.each([
                [`${apiuri}/${pin}/0`, 0],
                [`${apiuri}/${pin}/1`, 1],
                [`${apiuri}/${pin}/0/`, 0],
                [`${apiuri}/${pin}/1/`, 1],

            ])('GET /%s', (u, state) => {
                let resp, wsList, spies;
                beforeAll(async () => {
                    spies = [
                        jest.fn(),
                        jest.fn(),
                        jest.fn()
                    ]
                    wsList = await Promise.all(spies.map(spy => socketSpy(`${wsHost}/${wsApi}`, spy)))
                    resp = await axios({
                        ...options,
                        url: `${apihost}/${u}`
                    });
                })

                afterAll(async () => {
                    const l = await Promise.all(wsList)
                    await Promise.all(l.map(ws => closeWs(ws)))
                })

                it("should respond with 200 status", () => {
                    expect(resp.status).toBe(200);
                })


                it("should log HTTP request", async () => {
                    const log = await extractLog('HTTP');
                    expect(log).toBe(`HTTP GET /${u}`);
                })

                it("should respond with application/json content", () => {
                    expect(resp.headers['content-type']).toBe('application/json; charset=utf-8');
                })

                it("should respond with current state", () => {
                    expect(resp.data).toStrictEqual({
                        pin,
                        state
                    })
                })

                it("should log current state", async () => {
                    const log = await extractLog('PIN SET');
                    expect(log).toBe(`PIN SET ${JSON.stringify({ pin, state })}`);
                })

                it("should trigger all websocket clients with current state", () => {
                    for (const spy of spies) {
                        expect(spy).toHaveBeenCalledTimes(1);
                        expect(spy).toHaveBeenCalledWith({
                            pin,
                            state
                        });
                    }
                })
            })

        });

    describe("WS", () => {

        const openWs = (wsList) => {
            const ws = connectedSocket(`${wsHost}/${wsApi}`);
            wsList.push(ws);
            return ws;
        }
        const wsList = [];
        afterAll(async () => {
            const l = await Promise.all(wsList)
            await Promise.all(l.map(ws => closeWs(ws)))
        })

        it("should log ws client connection", async () => {
            await openWs(wsList);
            const log = await extractLog('WS');
            expect(log).toMatch(/^WS \#[0-9]* (?:[0-9]{1,3}\.){3}[0-9]{1,3} connected$/);
        })

        it("should log ws client disconnection", async () => {
            const ws = await openWs(wsList);
            await closeWs(ws);
            const log = await extractLog('WS');

            expect(log).toMatch(/^WS \#[0-9]* \(IP unset\) disconnected$/);
        })


    })

    describe.each([
        `${apiuri}`,
        `${apiuri}/`
    ])("GET /%s", (u) => {
        let resp;
        beforeAll(async () => {
            resp = await axios({
                ...options,
                url: `${apihost}/${u}`
            });
        })

        it("should respond with 200 status", () => {
            expect(resp.status).toBe(200);
        })

        it("should respond with application/json content", () => {
            expect(resp.headers['content-type']).toBe('application/json; charset=utf-8');
        })

        it("should respond all pins with state", () => {
            expect(resp.data).toStrictEqual(outpoutPins.map(p => ({
                pin: p,
                state: expect.any(Number)
            })))
        })
    })
})