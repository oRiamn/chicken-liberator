#include <Regexp.h>
#include <ESP8266WiFi.h>
#include <LibConstants.h>

#define outputPinsBufferSize 5
int outputPins[] = {
    2,
    4,
    5,
    0,
    15};
int outputPinValues[outputPinsBufferSize] = {};

const char *ssid = LibConstants::ssid;
const char *password = LibConstants::password;

WiFiServer server(80);
WiFiClient client;
bool response;

void setup()
{
  // initialisation de la communication série
  Serial.begin(115200);

  delay(100);

  // initialisation de la sortie pour la led
  for (byte i = 0; i < outputPinsBufferSize; i = i + 1)
  {
    pinMode(outputPins[i], OUTPUT);
    changeStateOutputPin(outputPins[i], LOW);
  }

  // Connexion wifi
  Serial.println("Connecting to " + String(ssid));

  WiFi.begin(ssid, password);

  // connection  en cours ...
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  // Wifi connecter
  Serial.println("WiFi connecter");

  // Démmarrage du serveur.
  server.begin();
  Serial.println("Serveur demarrer !");

  // Affichage de l'adresse IP
  Serial.print("Utiliser cette adresse URL pour la connexion : http://");
  Serial.println(WiFi.localIP());
}

bool changeStateOutputPin(int pin, int state)
{
  for (byte i = 0; i < outputPinsBufferSize; i = i + 1)
  {
    if (outputPins[i] == pin)
    {
      digitalWrite(pin, state);
      outputPinValues[i] = state;
      return true;
    }
  }
  return false;
}

void printPinToJSON(int iPin, int iState)
{
  client.print("{\"pin\":" + String(iPin) + ",\"state\":" + String(iState) + "}");
}

void digitalWritePin(const char *match,
                     const unsigned int length,
                     const MatchState &ms)
{
  char pin[2];
  char state[1];

  ms.GetCapture(pin, 0);
  int iPin = atoi(pin);

  ms.GetCapture(state, 1);
  int iState = atoi(state);

  if (changeStateOutputPin(iPin, iState))
  {
    printPinToJSON(iPin, iState);
    response = true;
  }
}

void getOutputPinValues(const char *match,
                        const unsigned int length,
                        const MatchState &ms)
{
  client.print('[');
  for (byte i = 0; i < outputPinsBufferSize; i = i + 1)
  {
    printPinToJSON(outputPins[i], outputPinValues[i]);
    if (i + 1 < outputPinsBufferSize)
    {
      client.print(',');
    }
  }
  client.print(']');
  response = true;
}

void loop()
{
  // Vérification si le client est connecter.
  client = server.available();
  if (!client)
  {
    return;
  }

  // Attendre si le client envoie des données ...
  Serial.println("Nouveau client");
  while (!client.available())
  {
    delay(1);
  }

  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  response = false;

  // Réponse
  client.println("HTTP/1.1 200 OK");

  if (request.indexOf("/api") != -1)
  {
    char buff[100];
    request.toCharArray(buff, 50);
    MatchState router(buff);

    client.println("Content-Type: application/json");
    client.println("");

    router.GlobalMatch("^GET \/api\/gpio\/out\/([0-9][0-9]?)\/([0-1])\/? HTTP\/1.1$", digitalWritePin);
    router.GlobalMatch("^GET \/api\/gpio\/out\/? HTTP\/1.1$", getOutputPinValues);

    if (!response)
    {
      client.print("{}");
    }

    client.println("");
  }
  else
  {
    client.println("Content-Type: text/html");
    client.println("");
    client.println("<!doctype html><html lang=\"en\"><head><meta charset=\"utf-8\"/><meta name=\"viewport\" content=\"width=device-width,initial-scale=1\"/><meta http-equiv=\"x-ua-compatible\" content=\"ie=edge\"/><script src=\"https://cdn.jsdelivr.net/npm/vue@2/dist/vue.js\"></script><title>Chicken Liberator</title><link rel=\"icon\" href=\"https://oriamn.github.io/chicken-liberator/favicon.png\"><script defer=\"defer\" src=\"https://oriamn.github.io/chicken-liberator/js/runtime.bundle.js\"></script><script defer=\"defer\" src=\"https://oriamn.github.io/chicken-liberator/js/main.bundle.js\"></script><link href=\"https://oriamn.github.io/chicken-liberator/styles/main.css\" rel=\"stylesheet\"></head><body><div id=\"root\"></div></body></html>");
  }

  delay(1);
  Serial.println("Client deconnecter");
  Serial.println("");
}
