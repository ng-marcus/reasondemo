let component = ReasonReact.statelessComponent "UserFile";

let make file::(file: Types.entity) children => {
  ...component,
  render: fun _self => {
    let clickServer event => ReactEventRe.Mouse.preventDefault event;
    <li>
      <a href="#"> (ReasonReact.stringToElement ("File: " ^ file.name)) </a>
    </li>
  }
};
