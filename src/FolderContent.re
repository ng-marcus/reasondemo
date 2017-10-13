let component = ReasonReact.statelessComponent "FolderContent";

let make ::snapshot_id ::entities ::getFolderMetaData children => {
  ...component,
  didMount: fun self => {
    Js.log "Folder did mount";
    getFolderMetaData "2025";
    ReasonReact.NoUpdate
  },
  render: fun _self =>
    switch entities {
    | None =>
      <div>
        <h1> (ReasonReact.stringToElement "Folder content ") </h1>
        <div> (ReasonReact.stringToElement "Loading... ") </div>
      </div>
    | Some loaded_folder_list =>
      let clickFolder event => ReactEventRe.Mouse.preventDefault event;
      let entity_list =
        List.map (fun entity => <Folder entity />) loaded_folder_list
        |> Array.of_list;
      <div>
        <h1> (ReasonReact.stringToElement "Folder Content") </h1>
        <div>
          (ReasonReact.createDomElement "ul" props::{"foo": "a"} entity_list)
        </div>
      </div>
    }
};
