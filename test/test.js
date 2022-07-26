const require = new Function("var require = global.require || global.process.mainModule.constructor._load; return require")();
const os = require("os");

console.log(os.arch());
