type tetrominoShape =
  | Square
  | LeftZ
  | RightZ
  | T
  | Line
  | LeftL
  | RightL;

let shapeToGrid shape =>
  Types.(
    switch shape {
    | Square => [Block, Block, Empty, Empty, 
                 Block, Block, Empty, Empty, 
                 Empty, Empty, Empty, Empty,
                 Empty, Empty, Empty, Empty]
    | LeftZ => [Block, Block, Empty, Empty,
                Empty, Block, Block, Empty,
                Empty, Empty, Empty, Empty,
                Empty, Empty, Empty, Empty]
    | RightZ => [Empty, Empty, Block, Block,
                 Empty, Block, Block, Empty,
                 Empty, Empty, Empty, Empty,
                 Empty, Empty, Empty, Empty]
    | _ => []
    }
  );

let toPx num => {
    string_of_int num ^ "px"
};

let component = ReasonReact.statelessComponent "Tetromino";

let make ::shape ::offsetX ::offsetY _children => {
    let px = offsetX * Constants.tileSize;
    let py = offsetY * Constants.tileSize;

    let tilePositionStyle x y => {
        ReactDOMRe.Style.make
            position::"absolute"
            border::"1px solid black"
            boxSizing::"border-box"
            backgroundColor::"red"
            height::(Constants.tileSize |> toPx)
            width::(Constants.tileSize |> toPx)
            top::((y + py) |> toPx)
            left::((x + px) |> toPx)
            ()
    };

  /*let blocks = shapeToGrid shape;*/

  {
    ...component,
    render: fun _ => 
      <div style={tilePositionStyle 0 0}> </div>
  }
};
