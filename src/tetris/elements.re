module Header = {
  let component = ReasonReact.statelessComponent "Header";
  let make ::text _children => {
    ...component,
    render: fun _ => <h1> (ReasonReact.stringToElement text) </h1>
  };
};

module Section = {
  let component = ReasonReact.statelessComponent "Section";
  let make children => {
    ...component,
    render: fun _ => ReasonReact.createDomElement "section" props::(Js.Obj.empty ()) children
  };
};
