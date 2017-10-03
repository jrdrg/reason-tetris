module Container = {
  let component = ReasonReact.statelessComponent "Stuff";

  let make ::header _children => {
    ...component,
    render: fun _ =>
      <Elements.Section> 
        <Elements.Header text=header /> 
        <GameBoard /> 
      </Elements.Section>
  };
};

ReactDOMRe.renderToElementWithId <Container header="tetris" /> "index";