const require = new Function("var require = global.require || global.process.mainModule.constructor._load; return require")();
const os = require("os");

// single line comment test

/*
  multiline
  comment
  test
*/

console.log(os.arch());
