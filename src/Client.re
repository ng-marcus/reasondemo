let component = ReasonReact.statelessComponent "Client";

let make client::(client: Types.client) ::showServers children => {
  ...component,
  render: fun _self => {
    let clickClient event => {
      ReactEventRe.Mouse.preventDefault event;
      showServers client.id
    };
    <li>
      <a href="#" onClick=clickClient>
        (
          ReasonReact.stringToElement (client.name ^ " (" ^ client.status ^ ")")
        )
      </a>
    </li>
  }
};
