// Generated by BUCKLESCRIPT VERSION 2.0.0, PLEASE EDIT WITH CARE
'use strict';

var React            = require("react");
var Caml_int32       = require("bs-platform/lib/js/caml_int32.js");
var Pervasives       = require("bs-platform/lib/js/pervasives.js");
var ReasonReact      = require("reason-react/lib/js/src/reasonReact.js");
var Constants$Tetris = require("./constants.js");

function shapeToGrid(shape) {
  switch (shape) {
    case 0 : 
        return /* array */[
                /* Block */0,
                /* Block */0,
                /* Empty */1,
                /* Empty */1,
                /* Block */0,
                /* Block */0,
                /* Empty */1,
                /* Empty */1,
                /* Empty */1,
                /* Empty */1,
                /* Empty */1,
                /* Empty */1,
                /* Empty */1,
                /* Empty */1,
                /* Empty */1,
                /* Empty */1
              ];
    case 1 : 
        return /* array */[
                /* Block */0,
                /* Block */0,
                /* Empty */1,
                /* Empty */1,
                /* Empty */1,
                /* Block */0,
                /* Block */0,
                /* Empty */1,
                /* Empty */1,
                /* Empty */1,
                /* Empty */1,
                /* Empty */1,
                /* Empty */1,
                /* Empty */1,
                /* Empty */1,
                /* Empty */1
              ];
    case 2 : 
        return /* array */[
                /* Empty */1,
                /* Empty */1,
                /* Block */0,
                /* Block */0,
                /* Empty */1,
                /* Block */0,
                /* Block */0,
                /* Empty */1,
                /* Empty */1,
                /* Empty */1,
                /* Empty */1,
                /* Empty */1,
                /* Empty */1,
                /* Empty */1,
                /* Empty */1,
                /* Empty */1
              ];
    case 3 : 
    case 4 : 
    case 5 : 
    case 6 : 
        return /* int array */[];
    
  }
}

function toPx(num) {
  return Pervasives.string_of_int(num) + "px";
}

var component = ReasonReact.statelessComponent("Tetromino");

function make(shape, offsetX, offsetY, _) {
  var px = Caml_int32.imul(offsetX, Constants$Tetris.tileSize);
  var py = Caml_int32.imul(offsetY, Constants$Tetris.tileSize);
  var tilePositionStyle = function (x, y) {
    return {
            backgroundColor: "red",
            border: "1px solid black",
            height: Pervasives.string_of_int(Constants$Tetris.tileSize) + "px",
            left: Pervasives.string_of_int(x + px | 0) + "px",
            position: "absolute",
            top: Pervasives.string_of_int(y + py | 0) + "px",
            width: Pervasives.string_of_int(Constants$Tetris.tileSize) + "px",
            boxSizing: "border-box"
          };
  };
  shapeToGrid(shape);
  var newrecord = component.slice();
  newrecord[/* render */9] = (function () {
      return React.createElement("div", {
                  style: tilePositionStyle(0, 0)
                });
    });
  return newrecord;
}

exports.shapeToGrid = shapeToGrid;
exports.toPx        = toPx;
exports.component   = component;
exports.make        = make;
/* component Not a pure module */
