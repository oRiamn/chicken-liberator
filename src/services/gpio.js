const apiUrl = API_URL;

export const getOutpoutPins = async () => {
  const resp = await fetch(`${apiUrl}/out`);
  return resp.json();
};

export const setOutPinState = async (pin, state) => {
  const resp = await fetch(`${apiUrl}/out/${pin}/${state}`);
  return resp.json();
};
