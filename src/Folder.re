let component = ReasonReact.statelessComponent "Folder";

let make entity::(entity: Types.entity) children => {
  ...component,
  render: fun _self => {
    let clickServer event => ReactEventRe.Mouse.preventDefault event;
    <li>
      <a href="#">
        (ReasonReact.stringToElement ("Folder: " ^ entity.name))
      </a>
    </li>
  }
};
