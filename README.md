# weird.js
weird.js is a javascript obfuscation compiler written in C, it produces working JavaScript using characters `[()]!+-/*=>`.

## Building

    make

## Usage

    ./weird [OPTIONS] in.js out.js
    
    Use the `-m` flag to disable wrapping the final code in an eval statement.

## How does this work?
weird.js uses type coercion to produce working, yet unreadable javascript.

I recommed checking out [this video by Low Level Javascript](https://www.youtube.com/watch?v=sRWE5tnaxlI) if you're interested in learning more.

## Runtimes
### Node.js
To use `require` you must define it, like so:

    const require = new Function("var require = global.require || global.process.mainModule.constructor._load; return require")();


## Support
Support for weird.js is available on [IRC](https://webchat.ephasic.org/?join=ephasic)
