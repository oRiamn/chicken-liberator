#!/usr/bin/env bash
arduino-cli compile -v --fqbn esp8266:esp8266:nodemcuv2 --libraries libraries --build-property 'compiler.cpp.extra_flags=-DASYNCWEBSERVER_REGEX=1'