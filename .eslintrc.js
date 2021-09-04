const vars = require("./src/front/config/vars");

module.exports = {
  env: {
    browser: true,
    commonjs: true,
    es2021: true,
    jest: true,
  },
  extends: ["standard", "prettier", "plugin:vue/recommended"],
  plugins: ["prettier", "vue"],
  parser: "babel-eslint",
  globals: {
    Vue: "readonly",
    ...Object.keys(vars.production).reduce((acc, key) => {
      acc[key] = "readonly";
      return acc;
    }, {}),
  },
  parserOptions: {
    ecmaVersion: 2018,
    sourceType: "module",
    parser: "babel-eslint",
  },
  rules: {
    "prefer-const": "error",
    "no-unused-vars": [
      "error",
      {
        args: "all",
        argsIgnorePattern: "^_",
      },
    ],
    "prettier/prettier": "error",
    "prefer-arrow-callback": "error",
    "prefer-template": "error",
    "prefer-spread": "error",
  },
};
