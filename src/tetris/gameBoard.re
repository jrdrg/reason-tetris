let seed: int = [%bs.raw "parseInt(Math.random() * Number.MAX_SAFE_INTEGER)"];

Js.log("Seed" ++ string_of_int(seed));

Random.init(seed);

type action =
  | NoOp
  | Tick
  | Move(Types.direction)
  | Drop
  | KeyUp
  | RotateLeft
  | RotateRight
  | GameOver
  | Restart;

type state = {
  tick: int,
  board: list(Types.gridSquare),
  activePiece: option(Game.activePiece)
};

let boardStyle =
  ReactDOMRe.Style.make(
    ~height=string_of_int(Constants.boardSize.height * Constants.tileSize) ++ "px",
    ~width=string_of_int(Constants.boardSize.width * Constants.tileSize) ++ "px",
    ~margin="auto",
    ~border="1px solid black",
    ~position="relative",
    ()
  );

let newShape = () => {
  let rnd = Random.int(3);
  Tetromino.(
    switch rnd {
    | 0 => Square
    | 1 => LeftZ
    | 2 => RightZ
    | _ => Square
    }
  )
};

let updateOffsets = (~x: int=0, ~y: int=0, piece) => {
  let {Game.offsetX, Game.offsetY} = piece;
  {...piece, offsetX: offsetX + x, offsetY: offsetY + y}
};

let stayWithinBounds = (piece) => {
  open Game;
  let offsetX = max(0, min(Constants.boardSize.width - 1, piece.offsetX));
  let offsetY = max(0, min(Constants.boardSize.height - 1, piece.offsetY));
  {...piece, offsetX, offsetY}
};

let withDefault = (default, maybe) =>
  switch maybe {
  | Some(v) => v
  | None => default
  };

let maybe = (func, a) =>
  switch a {
  | Some(v) => Some(func(v))
  | None => None
  };

let handleTick = (state) => {
  let shouldDrop = state.tick mod 10 === 0;
  Game.(
    switch state.activePiece {
    | Some(piece) =>
      let (movedPiece, yMove) =
        switch piece.state {
        | Moving(direction) =>
          switch direction {
          | Left => (piece |> updateOffsets(~x=(-1)), 0)
          | Right => (piece |> updateOffsets(~x=1), 0)
          | _ => (piece, 0)
          }
        | Dropping => (piece, 1)
        | _ => (piece, 0)
        };
      ReasonReact.Update({
        ...state,
        tick: state.tick + 1,
        activePiece:
          Some(
            movedPiece
            |> updateOffsets(~y=shouldDrop && yMove === 0 ? 1 : yMove)
            |> stayWithinBounds
          )
      })
    | None =>
      let shape = newShape();
      let activePiece =
        Some({
          shape,
          blocks: Tetromino.shapeToGrid(shape),
          state: NoInput,
          offsetX: 0,
          offsetY: 0
        });
      Js.log("Creating active shape");
      ReasonReact.Update({...state, tick: state.tick + 2, activePiece})
    }
  )
};

let updateActivePieceState = (state, pieceState) => {
  Js.log("Updating active piece state");
  let {activePiece} = state;
  activePiece
  |> maybe((piece) => {...state, activePiece: Some({...piece, state: pieceState})})
  |> withDefault(state)
};

let handleMove = (direction, state) =>
  Types.(
    switch direction {
    | Left => ReasonReact.Update(updateActivePieceState(state, Moving(Left)))
    | Right => ReasonReact.Update(updateActivePieceState(state, Moving(Right)))
    | _ => ReasonReact.Update(state)
    }
  );

let handleDrop = (state) => {
  Js.log("Dropping active piece");
  ReasonReact.Update(updateActivePieceState(state, Dropping))
};

let handleKeyUp = (state) => ReasonReact.Update(updateActivePieceState(state, NoInput));

/*let handleKeyDown (event: ReactEventRe.Keyboard.t) => {
      Js.log ("KEYDOWN", event |> ReactEventRe.Keyboard.keyCode)
  };*/
let addEventListener: (string, ReactEventRe.Keyboard.t => unit) => unit = [%bs.raw
  {|
    function(event, handler) {
        window.addEventListener(event, handler);
    }
|}
];

let removeEventListener: (string, ReactEventRe.Keyboard.t => unit) => unit = [%bs.raw
  {|
    function(event, handler) {
        window.removeEventListener(event, handler);
    }
|}
];

/* Component */
let component = ReasonReact.reducerComponent("GameBoard");

let make = (_children) => {
  ...component,
  initialState: () => {tick: 0, board: [], activePiece: None},
  reducer: (action, state) =>
    switch action {
    | Move(direction) => handleMove(direction, state)
    | Drop => handleDrop(state)
    | KeyUp => handleKeyUp(state)
    | Tick => handleTick(state)
    | _ => ReasonReact.Update(state)
    },
  didMount: (self) => {
    let _id = Js.Global.setInterval(self.reduce((_x) => Tick), 100);
    addEventListener(
      "keydown",
      (event) => {
        let keyCode = event |> ReactEventRe.Keyboard.keyCode;
        Js.log("Keydown" ++ string_of_int(keyCode));
        self.reduce(
          (_x) =>
            switch keyCode {
            | 37 => Move(Left)
            | 39 => Move(Right)
            | 40 => Drop
            | _ => NoOp
            },
          ()
        )
      }
    );
    addEventListener("keyup", (_) => self.reduce((_) => KeyUp, ()));
    ReasonReact.NoUpdate
  },
  /*willUnmount: fun _ => {
        removeEventListener "keydown" handleKeyDown
    },*/
  render: ({state: {activePiece}}) =>
    <div style=boardStyle>
      (
        switch activePiece {
        | Some({shape, offsetX, offsetY}) => <Tetromino shape offsetX offsetY />
        | None => ReasonReact.nullElement
        }
      )
    </div>
};