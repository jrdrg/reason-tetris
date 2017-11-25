open Jest;

describe(
  "game",
  () => {
    open Expect;
    open Types;
    let piece = [|
    Empty, Block, Empty, 
    Empty, Block, Empty, 
    Block, Block, Empty
    |];

    test(
      "#range",
      () => {
        let range = Game.range(0, 3);
        expect(range) |> toEqual([0, 1, 2])
      }
    );
    test(
      "#blockAt",
      () => {
        let block = Game.blockAt(2, 2, 1, 0, piece);
        expect(block) |> toEqual(Some(Block))
      }
    );
    test(
      "#rotateRight",
      () => {
        let expected = [|
        Block, Empty, Empty, 
        Block, Block, Block, 
        Block, Empty, Empty
        |];
        let rotated = piece |> Game.rotateRight;
        expect(rotated) |> toBe(expected)
      }
    );
    test(
      "#rotateLeft",
      () => {
        let expected = [|
        Block, Block, 
        Empty, Empty
        |];
        let rotated = piece |> Game.rotateLeft;
        expect(rotated) |> toBe(expected)
      }
    );
    test("#isColliding", () => expect(true) |> toBe(true))
  }
);