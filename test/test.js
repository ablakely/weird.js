#!/usr/bin/env node

/*
 * conv.js: Watches dir and automatically converts .mkv files to .mp4
 * Presets:
 *  - wait 3 mins after file creation to start ffmpeg conversion
 *  - wait 15 mins after ffmpeg conversion to delete the .mkv file
 *
 * NOTE:
 *  I wrote this script for use with http://motion-project.github.io,
 *  but I can see it's use cases other places so I'm posting it on gist.
 *
 * INSTALLING/USING
 * - install screen, ffmpeg and node
 * - cd to motion's target_dir
 * - $ npm install watch
 * - launch screen and run this script
 * - start motion
 *
 * Written by Aaron Blakely <aaron@ephasic.org>
 * Copyright 2020 (C) Ephasic Software
**/

const require = new Function("var require = global.require || global.process.mainModule.constructor._load; return require")();


const fs    = require("fs");
const exec  = require("child_process").exec;
const watch = require("watch");
const mkvTest = /^(.*).mkv$/;

// config
const scanWait = 3 * 60000;  // 3 min
const delWait  = 15 * 60000; // 15 min

var fsTimeout = [];
var fsi = 0;

function doDirScan() {
  fs.readdirSync(__dirname).forEach(function(f) {
    if (mkvTest.test(f)) {
      if (fs.existsSync(f.replace('.mkv', '.mp4'))) {
        console.log(`[scanner] Skipping previously converted file: ${f}`);
        return;
      }

      doFFmpegConv(f);
    }
  });
}

function doFFmpegConv(f) {
  console.log(`[conv] Converting file: ${f} to ${f.replace('.mkv', '.mp4')}`);

  var ffmpeg = exec(`/usr/bin/ffmpeg -i "${f}" -r 15 -c copy "${f.replace('.mkv', '.mp4')}"`);
  ffmpeg.stderr.on('data', function(d) {
    fs.appendFile("./ffmpeg-stdout.log", d, function(err) {
      if (err) throw err;
    });
  });

  setTimeout(function() {
    if (fs.existsSync(f)) {
      if (mkvTest.test(f) && fs.existsSync(f.replace('.mkv', '.mp4'))) {
        console.log(`[post-conv] Removing mkv file: ${f}`);
        fs.unlinkSync(f.replace('.mp4', '.mkv'));
      }
    }
  }, delWait);
}

watch.watchTree(__dirname, {ignoreDotFiles: true}, function(f, curr, prev) {
  if (typeof f === "object" && prev === null && curr === null) {
    console.log("[init-scan] completed inital scan");
    if (!fsTimeout[fsi]) {
      setTimeout(doDirScan, scanWait);

      fsi++;
      fsTimeout[fsi] = setTimeout(function() { fsTimeout[fsi] = null }, 5000);
    }
  } else if (typeof f === "string" && typeof curr === "object" && prev === null) {
    if (!fsTimeout[fsi]) {
      console.log(`[scanner] new file created: ${f}`);
      setTimeout(function() {
        doFFmpegConv(f);
      }, scanWait);

      fsi++;
      fsTimeout[fsi] = setTimeout(function() { fsTimeout[fsi] = null }, 5000);
    }
  }
});
