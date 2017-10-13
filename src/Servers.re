let component = ReasonReact.statelessComponent "Servers";

let make ::servers ::client_id ::showBackupJobs ::backToClients _children => {
  ...component,
  didMount: fun _self => {
    Js.log "Servers did mount";
    ReasonReact.NoUpdate
  },
  render: fun _self =>
    switch servers {
    | None =>
      <div>
        <h1> (ReasonReact.stringToElement "Servers ") </h1>
        <div> (ReasonReact.stringToElement "Loading... ") </div>
      </div>
    | Some loaded_server_list =>
      let clickBackToClients event => {
        ReactEventRe.Mouse.preventDefault event;
        backToClients ()
      };
      let server_list =
        Array.of_list (
          List.map
            (fun server => <Server server showBackupJobs />) loaded_server_list
        );
      /* DirectorRe.setRoute router "/user/1" */
      <div>
        <a href="#" onClick=clickBackToClients>
          (ReasonReact.stringToElement "Back to clients")
        </a>
        <h1> (ReasonReact.stringToElement "Servers ") </h1>
        <div>
          (
            ReasonReact.stringToElement (
              "Number of Servers "
              ^ string_of_int (List.length loaded_server_list)
            )
          )
        </div>
        (ReasonReact.createDomElement "ul" props::{"foo": "a"} server_list)
      </div>
    }
};
