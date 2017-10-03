let seed: int = [%bs.raw "parseInt(Math.random() * Number.MAX_SAFE_INTEGER)"];
Js.log ("Seed", seed);

Random.init(seed);

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


let boardStyle = ReactDOMRe.Style.make
    height::(string_of_int (Constants.boardSize.height * Constants.tileSize) ^ "px")
    width::(string_of_int (Constants.boardSize.width * Constants.tileSize) ^ "px")
    margin::"auto"
    border::"1px solid black"
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

let handleTick state => {
    switch state.activePiece {
    | Some _ => 
        ReasonReact.Update {...state, tick: state.tick + 1}
    | None => 
        let shape = newShape ();
        let activePiece = Some {shape, state: NoInput, offsetX: 0, offsetY: 0};
        Js.log ("Creating active shape", activePiece);
        ReasonReact.Update {...state, tick: state.tick + 2, activePiece}
    }
};

let handleMove direction state => {
    Js.log ("Moving ", direction);
    Types.(
        switch direction {
        | Up => ReasonReact.Update state
        | Down => ReasonReact.Update state
        | Left => ReasonReact.Update state
        | Right => ReasonReact.Update state
        }
    )
};

let handleDrop state => {
    Js.log "Dropping active piece";
    switch state.activePiece {
    | Some activePiece =>
        ReasonReact.Update {...state, activePiece: Some {...activePiece, state: Dropping}}
    | None =>
        ReasonReact.Update state
    }
};

let handleKeyUp state => {
    Js.log "Key up";
    switch state.activePiece {
    | Some activePiece =>
        ReasonReact.Update {...state, activePiece: Some {...activePiece, state: NoInput}}
    | None =>
        ReasonReact.Update state
    }
};

let addEventListener: string => (ReactEventRe.Keyboard.t => unit) => unit = [%bs.raw {|
    function(event, handler) {
        window.addEventListener(event, handler);
    }
|}];

let removeEventListener: string => (ReactEventRe.Keyboard.t => unit) => unit = [%bs.raw {|
    function(event, handler) {
        window.removeEventListener(event, handler);
    }    
|}];


let handleKeyDown (event: ReactEventRe.Keyboard.t) => {
    Js.log ("KEYDOWN", event |> ReactEventRe.Keyboard.keyCode)
};

/* Component */
let component = ReasonReact.reducerComponent "GameBoard";

let make _children => {
    ...component,
    initialState: fun () => {tick: 0, board: [], activePiece: None},
    reducer: fun action state => {
        switch action {
        | Move direction => handleMove direction state
        | KeyUp => handleKeyUp state
        | Tick => handleTick state
        | _ => ReasonReact.Update state
        }
    },
    didMount: fun self => {
        let _id = Js.Global.setInterval(self.reduce (fun _ => Tick)) 1000;

        addEventListener "keydown" (fun event => {
            let keyCode = event |> ReactEventRe.Keyboard.keyCode;
            Js.log ("Keydown", keyCode);
            self.reduce(fun _ => {
                switch keyCode {
                | 37 => Move Left
                | 39 => Move Right
                | 40 => Drop
                | _ => NoOp
                }
            }) ();
        });

        addEventListener "keyup" (fun _ => {
            self.reduce(fun _ => KeyUp) ()
        });

        ReasonReact.NoUpdate;
    },
    /*willUnmount: fun _ => {
        removeEventListener "keydown" handleKeyDown
    },*/
    render: fun {state} => {
        <div style={boardStyle}>
            {state.tick |> string_of_int |> ReasonReact.stringToElement}
        </div>
    }
};