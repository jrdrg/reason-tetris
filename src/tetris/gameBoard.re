let seed: int = [%bs.raw "parseInt(Math.random() * Number.MAX_SAFE_INTEGER)"];

Js.log ("Seed", seed);

Random.init seed;

type action =
  | NoOp
  | Tick
  | Move Types.direction
  | Drop
  | KeyUp
  | RotateLeft
  | RotateRight
  | GameOver
  | Restart;

type currentPieceState =
  | NoInput
  | Moving Types.direction
  | Dropping;

type activePiece = {
  shape: Tetromino.tetrominoShape,
  state: currentPieceState,
  offsetX: int,
  offsetY: int
};

type state = {
  tick: int,
  board: list Types.gridSquare,
  activePiece: option activePiece
};

let boardStyle =
  ReactDOMRe.Style.make
    height::(string_of_int (Constants.boardSize.height * Constants.tileSize) ^ "px")
    width::(string_of_int (Constants.boardSize.width * Constants.tileSize) ^ "px")
    margin::"auto"
    border::"1px solid black"
    position::"relative"
    ();

let newShape () => {
  let rnd = Random.int 3;
  Tetromino.(
    switch rnd {
    | 0 => Square
    | 1 => LeftZ
    | 2 => RightZ
    | _ => Square
    }
  )
};

let updateOffsets x::(x: int)=0 y::(y: int)=0 piece => {
  let {offsetX, offsetY} = piece;
  {...piece, offsetX: offsetX + x, offsetY: offsetY + y}
};

let stayWithinBounds piece => {
    let offsetX = max 0 (min (Constants.boardSize.width - 1) piece.offsetX);
    let offsetY = max 0 (min (Constants.boardSize.height - 1) piece.offsetY);
    {...piece, offsetX, offsetY};
};

let handleTick state =>
  switch state.activePiece {
  | Some piece =>
    let movedPiece =
      switch piece.state {
      | Moving direction =>
        switch direction {
        | Left => piece |> updateOffsets x::(-1)
        | Right => piece |> updateOffsets x::1
        | _ => piece
        }
      | Dropping => piece |> updateOffsets y::1
      | _ => piece
      };
    ReasonReact.Update {
      ...state,
      tick: state.tick + 1,
      activePiece: Some (movedPiece |> updateOffsets y::1 |> stayWithinBounds)
    }
  | None =>
    let shape = newShape ();
    let activePiece = Some {shape, state: NoInput, offsetX: 0, offsetY: 0};
    Js.log ("Creating active shape", activePiece);
    ReasonReact.Update {...state, tick: state.tick + 2, activePiece}
  };

let updateActivePieceState state pieceState => {
  Js.log ("Updating active piece state", pieceState);
  let {activePiece} = state;
  switch activePiece {
  | None => state
  | Some piece => {...state, activePiece: Some {...piece, state: pieceState}}
  }
};

let handleMove direction state => {
  Js.log ("Moving ", direction);
  Types.(
    switch direction {
    | Left => ReasonReact.Update (updateActivePieceState state (Moving Left))
    | Right => ReasonReact.Update (updateActivePieceState state (Moving Right))
    | _ => ReasonReact.Update state
    }
  )
};

let handleDrop state => {
  Js.log "Dropping active piece";
  ReasonReact.Update (updateActivePieceState state Dropping)
};

let handleKeyUp state => {
  Js.log "Key up";
  ReasonReact.Update (updateActivePieceState state NoInput)
};

/*let handleKeyDown (event: ReactEventRe.Keyboard.t) => {
      Js.log ("KEYDOWN", event |> ReactEventRe.Keyboard.keyCode)
  };*/
let addEventListener: string => (ReactEventRe.Keyboard.t => unit) => unit = [%bs.raw
  {|
    function(event, handler) {
        window.addEventListener(event, handler);
    }
|}
];

let removeEventListener: string => (ReactEventRe.Keyboard.t => unit) => unit = [%bs.raw
  {|
    function(event, handler) {
        window.removeEventListener(event, handler);
    }
|}
];

/* Component */
let component = ReasonReact.reducerComponent "GameBoard";

let make _children => {
  ...component,
  initialState: fun () => {tick: 0, board: [], activePiece: None},
  reducer: fun action state =>
    switch action {
    | Move direction => handleMove direction state
    | Drop => handleDrop state
    | KeyUp => handleKeyUp state
    | Tick => handleTick state
    | _ => ReasonReact.Update state
    },
  didMount: fun self => {
    let _id = Js.Global.setInterval (self.reduce (fun _ => Tick)) 500;
    addEventListener
      "keydown"
      (
        fun event => {
          let keyCode = event |> ReactEventRe.Keyboard.keyCode;
          Js.log ("Keydown", keyCode);
          self.reduce
            (
              fun _ =>
                switch keyCode {
                | 37 => Move Left
                | 39 => Move Right
                | 40 => Drop
                | _ => NoOp
                }
            )
            ()
        }
      );
    addEventListener "keyup" (fun _ => self.reduce (fun _ => KeyUp) ());
    ReasonReact.NoUpdate
  },
  /*willUnmount: fun _ => {
        removeEventListener "keydown" handleKeyDown
    },*/
  render: fun {state: {activePiece, tick}} =>
    <div style=boardStyle>
      (tick |> string_of_int |> ReasonReact.stringToElement)
      (
        switch activePiece {
        | Some {shape, offsetX, offsetY} => <Tetromino shape offsetX offsetY />
        | None => ReasonReact.nullElement
        }
      )
    </div>
};