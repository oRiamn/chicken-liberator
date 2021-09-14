# Chicken Liberator

[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](https://opensource.org/licenses/MIT)

Some frontend for ESP8266 project

## Installation

Clone this repo and yarn install.

```bash
yarn i
```

Follow this guide for installing arduino-cli and your ESP Board

https://arduino.github.io/arduino-cli/latest/installation/

https://create.arduino.cc/projecthub/B45i/getting-started-with-arduino-cli-7652a5

Or using docker:

```bash
docker build -t chicken docker --label chicken.liberator=true
docker run -it --device=/dev/ttyUSB0 -v $PWD/ESP8266:/root/ESP8266 -v /tmp:/tmp chicken bash
```

Rename `ESP8266/libraries/LibConstants/LibConstants.template.h` to `ESP8266/libraries/LibConstants/LibConstants.h` and fill constants values

## Usage
### ESP8266 side

First go to the ino project (not for docker usage):
```bash
cd ESP826
```
Compile the ino program

```bash
arduino-cli compile -v --fqbn esp8266:esp8266:nodemcuv2 --libraries libraries
```
or 
```bash
chicken-compile
```
Allow your user to read and write your COM port (not for docker usage):

```bash
sudo chmod a+rw /dev/ttyUSB0
```

Upload the compiled program to the board :

```bash
arduino-cli upload -p /dev/ttyUSB0 --fqbn esp8266:esp8266:nodemcuv2
```
or 
```bash
chicken-upload
```

For listennig COM port (ESP8266 log server) :

```bash
tail -f /tmp/esp8266-received.log | xargs -IL date +"%Y-%m-%d %H:%M:%S L"
```

```bash
tail -f /tmp/esp8266-received.log | grep -a --line-buffered "FREEHEAP"
```

if the file `/tmp/esp8266-received.log` does not exist or you do not read any log, then run the command : 

```bash
chicken-logtty
```

### Development server

```bash
yarn start
```

You can view the development server at `localhost:8080`.

Use fake API for simulate ESP8266 responses: 

```bash
yarn fakeapi
```

### Production build

```bash
yarn run build
```

You can view the deploy by creating a server in `dist`.

## Features

- [webpack](https://webpack.js.org/)
- [Babel](https://babeljs.io/)
- [Sass](https://sass-lang.com/)
- [PostCSS](https://postcss.org/)
- [RxJS](https://rxjs.dev/)

## Dependencies

### webpack

- [`webpack`](https://github.com/webpack/webpack) - Module and asset bundler.
- [`webpack-cli`](https://github.com/webpack/webpack-cli) - Command line interface for webpack
- [`webpack-dev-server`](https://github.com/webpack/webpack-dev-server) - Development server for webpack
- [`webpack-merge`](https://github.com/survivejs/webpack-merge) - Simplify development/production configuration
- [`cross-env`](https://github.com/kentcdodds/cross-env) - Cross platform configuration

### Babel

- [`@babel/core`](https://www.yarnjs.com/package/@babel/core) - Transpile ES6+ to backwards compatible JavaScript
- [`@babel/plugin-proposal-class-properties`](https://babeljs.io/docs/en/babel-plugin-proposal-class-properties) - Use properties directly on a class (an example Babel config)
- [`@babel/preset-env`](https://babeljs.io/docs/en/babel-preset-env) - Smart defaults for Babel

### Loaders

- [`babel-loader`](https://webpack.js.org/loaders/babel-loader/) - Transpile files with Babel and webpack
- [`sass-loader`](https://webpack.js.org/loaders/sass-loader/) - Load SCSS and compile to CSS
  - [`node-sass`](https://github.com/sass/node-sass) - Node Sass
- [`postcss-loader`](https://webpack.js.org/loaders/postcss-loader/) - Process CSS with PostCSS
  - [`postcss-preset-env`](https://www.yarnjs.com/package/postcss-preset-env) - Sensible defaults for PostCSS
- [`css-loader`](https://webpack.js.org/loaders/css-loader/) - Resolve CSS imports
- [`style-loader`](https://webpack.js.org/loaders/style-loader/) - Inject CSS into the DOM

### Plugins

- [`clean-webpack-plugin`](https://github.com/johnagan/clean-webpack-plugin) - Remove/clean build folders
- [`copy-webpack-plugin`](https://github.com/webpack-contrib/copy-webpack-plugin) - Copy files to build directory
- [`html-webpack-plugin`](https://github.com/jantimon/html-webpack-plugin) - Generate HTML files from template
- [`mini-css-extract-plugin`](https://github.com/webpack-contrib/mini-css-extract-plugin) - Extract CSS into separate files
- [`css-minimizer-webpack-plugin`](https://webpack.js.org/plugins/css-minimizer-webpack-plugin/) - Optimize and minimize CSS assets

### Linters

- [`eslint`](https://github.com/eslint/eslint) - Enforce styleguide across application
- [`eslint-config-airbnb-base`](https://github.com/airbnb/javascript/tree/master/packages/eslint-config-airbnb-base) - Base styleguide to enforce rules
- [`eslint-config-prettier`](https://github.com/prettier/eslint-config-prettier) - Implment prettier rules
- [`eslint-plugin-import`](https://github.com/benmosher/eslint-plugin-import) - Implment import rules
- [`eslint-plugin-prettier`](https://github.com/prettier/eslint-plugin-prettier) - Dependency for prettier usage with ESLint
- [`eslint-import-resolver-webpack`](https://github.com/benmosher/eslint-plugin-import/tree/master/resolvers/webpack) - Throw exceptions for import/export in webpack
- [`eslint-webpack-plugin`](https://github.com/webpack-contrib/eslint-webpack-plugin) - ESLint configuration for webpack
- [`prettier`](https://github.com/prettier/prettier) - Dependency for `prettier-webpack-plugin` plugin
- [`prettier-webpack-plugin`](https://github.com/hawkins/prettier-webpack-plugin) - Prettier configuration for webpack

## License

This project is open source and available under the [MIT License](LICENSE).
