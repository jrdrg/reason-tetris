const path = require('path');

module.exports = {
  entry: {
    tetris: './lib/js/src/tetris/main.js',
  },
  output: {
    path: path.join(__dirname, "bundledOutputs"),
    filename: '[name].js',
  },
};
