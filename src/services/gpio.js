export const getOutpoutPins = async () => {
  const resp = await fetch("/api/gpio/out");
  return resp.json();
};

export const setOutPinState = async (pin, state) => {
  const resp = await fetch(`/api/gpio/out/${pin}/${state}`);
  return resp.json();
};
