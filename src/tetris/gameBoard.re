let seed: int = [%bs.raw "parseInt(Math.random() * Number.MAX_SAFE_INTEGER)"];
Js.log ("Seed", seed);

Random.init(seed);

type action =
  | Tick
  | Move Types.direction
  | Drop
  | RotateLeft
  | RotateRight
  | GameOver
  | Restart;

type currentPieceState =
  | None
  | Moving Types.direction
  | Dropping;
  
type state = {
    tick: int,
    board: list Types.gridSquare,
    activeShape: option (Tetromino.tetrominoShape, currentPieceState)
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
    switch state.activeShape {
    | Some shape => {
        Js.log ("Active shape", shape);
        ReasonReact.Update {...state, tick: state.tick + 1}
    }
    | None => {
        let shape = newShape ();
        let activeShape = Some (shape, None);
        Js.log ("Creating active shape", activeShape);
        ReasonReact.Update {...state, tick: state.tick + 2, activeShape}
    }
    }
};

let handleMove direction state => {
    Types.(
        switch direction {
        | Up => ReasonReact.Update state
        | Down => ReasonReact.Update state
        | Left => ReasonReact.Update state
        | Right => ReasonReact.Update state
        }
    )
};


/* Component */
let component = ReasonReact.reducerComponent "GameBoard";

let make _children => {
    ...component,
    initialState: fun () => {tick: 0, board: [], activeShape: None},
    reducer: fun action state => {
        switch action {
        | Move direction => handleMove direction state
        | Tick => handleTick state
        | _ => ReasonReact.Update state
        }
    },
    didMount: fun self => {
        let _id = Js.Global.setInterval(self.reduce (fun _ => Tick)) 1000;
        ReasonReact.NoUpdate;
    },
    render: fun {state} => {
        <div style={boardStyle}>
            {state.tick |> string_of_int |> ReasonReact.stringToElement}
        </div>
    }
};