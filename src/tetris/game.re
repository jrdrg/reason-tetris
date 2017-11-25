/**
 * Types
 */
type currentPieceState =
  | NoInput
  | Moving(Types.direction)
  | Dropping
  | RotatingLeft
  | RotatingRight;

type tetromino = {squares: array(Types.gridSquare)};

type board = {squares: array(Types.gridSquare)};

type activePiece = {
  shape: Tetromino.tetrominoShape,
  blocks: array(Types.gridSquare),
  state: currentPieceState,
  offsetX: int,
  offsetY: int
};

/**
 * Init
 */
let initBoard = () => {
  open Constants;
  let {height, width} = boardSize;
  Array.make(height * width, Types.Empty)
};

let blockAt = (width: int, height: int, x: int, y: int, array: array(Types.gridSquare)) => {
  let maxIndex = width * height;
  let index = y * width + x mod width;
  if (index < maxIndex) {
    Some(array[index])
  } else {
    None
  }
};

let blockPosition = blockAt(Constants.pieceSize, Constants.pieceSize);

let boardPosition = blockAt(Constants.boardSize.width, Constants.boardSize.height);

let range = (first, last) => {
  let rec _range = (curr, max, list) =>
    if (curr < max) {
      [curr, ..._range(curr + 1, max, list)]
    } else {
      list
    };
  _range(first, last, [])
};

/**
 * Movement/update
 */
let isColliding = (piece, board) => true;

let rotateRight = (blocks: array(Types.gridSquare)) => {
  let size = Constants.pieceSize;
  let xs = range(0, size) |> List.rev;
  let ys = range(0, size);
  let start = (Array.make(size * size, Types.Empty), 0);
  let f = ((list, index), next) => (list, index);
  let (rotated, _) = blocks |> Array.fold_left(f, start);
  rotated
};

let rotateLeft = (blocks) => blocks;

let movePieceDown = (piece: activePiece, velocity, board) => {
  let movedPiece = {...piece, offsetY: piece.offsetY + velocity};
  movedPiece
};

let updateTick = (tick, board) => board;