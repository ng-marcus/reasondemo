let component = ReasonReact.statelessComponent "Server";

let make server::(server: Types.server) ::showBackupJobs children => {
  ...component,
  render: fun _self => {
    let clickServer event => {
      ReactEventRe.Mouse.preventDefault event;
      showBackupJobs server.id
    };
    <li>
      <a href="#" onClick=clickServer>
        (
          ReasonReact.stringToElement (server.name ^ " (" ^ server.status ^ ")")
        )
      </a>
    </li>
  }
};
