// Generated by BUCKLESCRIPT VERSION 2.0.0, PLEASE EDIT WITH CARE
'use strict';

var Block            = require("bs-platform/lib/js/block.js");
var Curry            = require("bs-platform/lib/js/curry.js");
var React            = require("react");
var Random           = require("bs-platform/lib/js/random.js");
var Caml_int32       = require("bs-platform/lib/js/caml_int32.js");
var Pervasives       = require("bs-platform/lib/js/pervasives.js");
var ReasonReact      = require("reason-react/lib/js/src/reasonReact.js");
var Constants$Tetris = require("./constants.js");
var Tetromino$Tetris = require("./tetromino.js");

var seed = (parseInt(Math.random() * Number.MAX_SAFE_INTEGER));

console.log("Seed" + Pervasives.string_of_int(seed));

Random.init(seed);

var boardStyle = {
  border: "1px solid black",
  height: Pervasives.string_of_int(Caml_int32.imul(Constants$Tetris.boardSize[/* height */0], Constants$Tetris.tileSize)) + "px",
  margin: "auto",
  position: "relative",
  width: Pervasives.string_of_int(Caml_int32.imul(Constants$Tetris.boardSize[/* width */1], Constants$Tetris.tileSize)) + "px"
};

function newShape() {
  var rnd = Random.$$int(3);
  if (rnd > 2 || rnd < 0) {
    return /* Square */0;
  } else {
    return rnd;
  }
}

function updateOffsets($staropt$star, $staropt$star$1, piece) {
  var x = $staropt$star ? $staropt$star[0] : 0;
  var y = $staropt$star$1 ? $staropt$star$1[0] : 0;
  return /* record */[
          /* shape */piece[/* shape */0],
          /* blocks */piece[/* blocks */1],
          /* state */piece[/* state */2],
          /* offsetX */piece[/* offsetX */3] + x | 0,
          /* offsetY */piece[/* offsetY */4] + y | 0
        ];
}

function stayWithinBounds(piece) {
  var offsetX = Pervasives.max(0, Pervasives.min(Constants$Tetris.boardSize[/* width */1] - 1 | 0, piece[/* offsetX */3]));
  var offsetY = Pervasives.max(0, Pervasives.min(Constants$Tetris.boardSize[/* height */0] - 1 | 0, piece[/* offsetY */4]));
  return /* record */[
          /* shape */piece[/* shape */0],
          /* blocks */piece[/* blocks */1],
          /* state */piece[/* state */2],
          /* offsetX */offsetX,
          /* offsetY */offsetY
        ];
}

function withDefault($$default, maybe) {
  if (maybe) {
    return maybe[0];
  } else {
    return $$default;
  }
}

function maybe(func, a) {
  if (a) {
    return /* Some */[Curry._1(func, a[0])];
  } else {
    return /* None */0;
  }
}

function handleTick(state) {
  var shouldDrop = +(state[/* tick */0] % 10 === 0);
  var match = state[/* activePiece */2];
  if (match) {
    var piece = match[0];
    var match$1 = piece[/* state */2];
    var match$2;
    if (typeof match$1 === "number") {
      match$2 = match$1 !== 1 ? /* tuple */[
          piece,
          0
        ] : /* tuple */[
          piece,
          1
        ];
    } else {
      var direction = match$1[0];
      if (direction !== 0) {
        switch (direction - 1 | 0) {
          case 0 : 
              var partial_arg = /* Some */[-1];
              match$2 = /* tuple */[
                (function (eta) {
                      var param = /* None */0;
                      var param$1 = eta;
                      return updateOffsets(partial_arg, param, param$1);
                    })(piece),
                0
              ];
              break;
          case 1 : 
              match$2 = /* tuple */[
                piece,
                0
              ];
              break;
          case 2 : 
              var partial_arg$1 = /* Some */[1];
              match$2 = /* tuple */[
                (function (eta) {
                      var param = /* None */0;
                      var param$1 = eta;
                      return updateOffsets(partial_arg$1, param, param$1);
                    })(piece),
                0
              ];
              break;
          
        }
      } else {
        match$2 = /* tuple */[
          piece,
          0
        ];
      }
    }
    var yMove = match$2[1];
    var match$3 = shouldDrop && +(yMove === 0);
    var arg = /* Some */[match$3 !== 0 ? 1 : yMove];
    return /* Update */Block.__(0, [/* record */[
                /* tick */state[/* tick */0] + 1 | 0,
                /* board */state[/* board */1],
                /* activePiece : Some */[stayWithinBounds((function (eta) {
                            return updateOffsets(/* None */0, arg, eta);
                          })(match$2[0]))]
              ]]);
  } else {
    var shape = newShape(/* () */0);
    var activePiece = /* Some */[/* record */[
        /* shape */shape,
        /* blocks */Tetromino$Tetris.shapeToGrid(shape),
        /* state : NoInput */0,
        /* offsetX */0,
        /* offsetY */0
      ]];
    console.log("Creating active shape");
    return /* Update */Block.__(0, [/* record */[
                /* tick */state[/* tick */0] + 2 | 0,
                /* board */state[/* board */1],
                /* activePiece */activePiece
              ]]);
  }
}

function updateActivePieceState(state, pieceState) {
  console.log("Updating active piece state");
  return withDefault(state, maybe((function (piece) {
                    return /* record */[
                            /* tick */state[/* tick */0],
                            /* board */state[/* board */1],
                            /* activePiece : Some */[/* record */[
                                /* shape */piece[/* shape */0],
                                /* blocks */piece[/* blocks */1],
                                /* state */pieceState,
                                /* offsetX */piece[/* offsetX */3],
                                /* offsetY */piece[/* offsetY */4]
                              ]]
                          ];
                  }), state[/* activePiece */2]));
}

function handleMove(direction, state) {
  if (direction !== 0) {
    switch (direction - 1 | 0) {
      case 0 : 
          return /* Update */Block.__(0, [updateActivePieceState(state, /* Moving */[/* Left */1])]);
      case 1 : 
          return /* Update */Block.__(0, [state]);
      case 2 : 
          return /* Update */Block.__(0, [updateActivePieceState(state, /* Moving */[/* Right */3])]);
      
    }
  } else {
    return /* Update */Block.__(0, [state]);
  }
}

function handleDrop(state) {
  console.log("Dropping active piece");
  return /* Update */Block.__(0, [updateActivePieceState(state, /* Dropping */1)]);
}

function handleKeyUp(state) {
  return /* Update */Block.__(0, [updateActivePieceState(state, /* NoInput */0)]);
}

var addEventListener = (
    function(event, handler) {
        window.addEventListener(event, handler);
    }
);

var removeEventListener = (
    function(event, handler) {
        window.removeEventListener(event, handler);
    }
);

var component = ReasonReact.reducerComponent("GameBoard");

function make() {
  var newrecord = component.slice();
  newrecord[/* didMount */4] = (function (self) {
      setInterval(Curry._1(self[/* reduce */3], (function () {
                  return /* Tick */1;
                })), 100);
      Curry._2(addEventListener, "keydown", (function ($$event) {
              var keyCode = $$event.keyCode;
              console.log("Keydown" + Pervasives.string_of_int(keyCode));
              return Curry._2(self[/* reduce */3], (function () {
                            var switcher = keyCode - 37 | 0;
                            if (switcher > 3 || switcher < 0) {
                              return /* NoOp */0;
                            } else {
                              switch (switcher) {
                                case 0 : 
                                    return /* Move */[/* Left */1];
                                case 1 : 
                                    return /* NoOp */0;
                                case 2 : 
                                    return /* Move */[/* Right */3];
                                case 3 : 
                                    return /* Drop */2;
                                
                              }
                            }
                          }), /* () */0);
            }));
      Curry._2(addEventListener, "keyup", (function () {
              return Curry._2(self[/* reduce */3], (function () {
                            return /* KeyUp */3;
                          }), /* () */0);
            }));
      return /* NoUpdate */0;
    });
  newrecord[/* render */9] = (function (param) {
      var activePiece = param[/* state */4][/* activePiece */2];
      var tmp;
      if (activePiece) {
        var match = activePiece[0];
        tmp = ReasonReact.element(/* None */0, /* None */0, Tetromino$Tetris.make(match[/* shape */0], match[/* offsetX */3], match[/* offsetY */4], /* array */[]));
      } else {
        tmp = null;
      }
      return React.createElement("div", {
                  style: boardStyle
                }, tmp);
    });
  newrecord[/* initialState */10] = (function () {
      return /* record */[
              /* tick */0,
              /* board : [] */0,
              /* activePiece : None */0
            ];
    });
  newrecord[/* reducer */12] = (function (action, state) {
      if (typeof action === "number") {
        switch (action) {
          case 1 : 
              return handleTick(state);
          case 2 : 
              return handleDrop(state);
          case 3 : 
              return /* Update */Block.__(0, [updateActivePieceState(state, /* NoInput */0)]);
          case 0 : 
          case 4 : 
          case 5 : 
          case 6 : 
          case 7 : 
              return /* Update */Block.__(0, [state]);
          
        }
      } else {
        return handleMove(action[0], state);
      }
    });
  return newrecord;
}

exports.seed                   = seed;
exports.boardStyle             = boardStyle;
exports.newShape               = newShape;
exports.updateOffsets          = updateOffsets;
exports.stayWithinBounds       = stayWithinBounds;
exports.withDefault            = withDefault;
exports.maybe                  = maybe;
exports.handleTick             = handleTick;
exports.updateActivePieceState = updateActivePieceState;
exports.handleMove             = handleMove;
exports.handleDrop             = handleDrop;
exports.handleKeyUp            = handleKeyUp;
exports.addEventListener       = addEventListener;
exports.removeEventListener    = removeEventListener;
exports.component              = component;
exports.make                   = make;
/* seed Not a pure module */
