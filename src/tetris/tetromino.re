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

let component = ReasonReact.statelessComponent "Tetromino";

let make _children => {
  ...component,
  render: fun _ => 
    <div> (ReasonReact.stringToElement "test") </div>
};
