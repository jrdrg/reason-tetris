module Container = {
  let component = ReasonReact.statelessComponent("Container");
  let make = (~header, _children) => {
    ...component,
    render: (_) =>
      <Elements.Section> <Elements.Header text=header /> <GameBoard /> </Elements.Section>
  };
};

ReactDOMRe.renderToElementWithId(<Container header="tetris" />, "index");