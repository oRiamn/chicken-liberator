FROM debian:stretch-slim

RUN apt-get update && apt-get install -y curl python3
RUN curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | sh
RUN arduino-cli config init 
RUN sed -i 's/additional_urls\: \[\]/additional_urls\: [https:\/\/arduino.esp8266.com\/stable\/package_esp8266com_index.json\]/' ~/.arduino15/arduino-cli.yaml
RUN arduino-cli core update-index && arduino-cli core search esp8266 && arduino-cli core install esp8266:esp8266

WORKDIR /root/ESP8266

ADD helpers /usr/bin/