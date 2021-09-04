import { Subject } from "rxjs";
import { emitErrorNotification } from "./notification";

const apiUrl =
  API_URL ||
  `http://${window.location.hostname}${
    window.location.port ? `:${window.location.port}` : ""
  }`;
const wsEndpoint =
  WS_ENDPOINT ||
  `ws://${window.location.hostname}${
    window.location.port ? `:${window.location.port}` : ""
  }`;

export const pinSubject = new Subject();

const ws = new WebSocket(`${wsEndpoint}/api/gpio`);
ws.onmessage = (evt) => {
  try {
    const pin = JSON.parse(evt.data);
    pinSubject.next(pin);
  } catch (e) {
    emitErrorNotification("Oops", "Une erreur de communication", e);
  }
};

export const getOutpoutPins = async () => {
  const resp = await fetch(`${apiUrl}/api/gpio/out`);
  return resp.json();
};

export const setOutPinState = async (pin, state) => {
  const resp = await fetch(`${apiUrl}/api/gpio/out/${pin}/${state}`);
  pinSubject.next(await resp.json());
};
