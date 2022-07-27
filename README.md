# wtf.js
wtf.js is a [JSFuck](https://en.wikipedia.org/wiki/JSFuck) compiler written in C.

## Building

    make

## Usage

    ./wtf [OPTIONS] in.js out.js

## How does this work?
wtf.js uses type coercion to produce working, yet unreadable javascript.

I recommed checking out [this video by Low Level Javascript](https://www.youtube.com/watch?v=sRWE5tnaxlI) if you're interested in learning more.

## Runtimes
### Node.js
Use the `-m` flag to disable wrapping the final code in an eval statement, this allows compiled code to be used with `import` or `require()`

## Support
Support for wtf.js is available on [IRC](https://webchat.ephasic.org/?join=ephasic)
