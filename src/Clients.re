let component = ReasonReact.statelessComponent "Clients";

let make ::clients ::showServers children => {
  ...component,
  didMount: fun self => {
    Js.log "Clients did mount";
    ReasonReact.NoUpdate
  },
  render: fun _self =>
    switch clients {
    | None =>
      <div>
        <h1> (ReasonReact.stringToElement "Clients ") </h1>
        (ReasonReact.stringToElement "Loading... ")
      </div>
    | Some loaded_client_list =>
      let client_list =
        Array.of_list (
          List.map
            (fun client => <Client client showServers />) loaded_client_list
        );
      <div>
        <h1> (ReasonReact.stringToElement "Clients ") </h1>
        <div>
          (
            ReasonReact.stringToElement (
              "Number of Clients "
              ^ string_of_int (List.length loaded_client_list)
            )
          )
        </div>
        (ReasonReact.createDomElement "ul" props::{"foo": "a"} client_list)
      </div>
    }
};
