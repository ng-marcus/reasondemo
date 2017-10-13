let component = ReasonReact.statelessComponent "Snapshots";

let make
    snapshots::(snapshots: option (list Types.snapshot))
    ::backup_job_id
    ::backToBackupJobs
    _children => {
  ...component,
  didMount: fun _self => {
    Js.log "Snapshots did mount";
    ReasonReact.NoUpdate
  },
  render: fun _self =>
    switch snapshots {
    | None =>
      <div>
        <h1> (ReasonReact.stringToElement "Snapshots ") </h1>
        <div> (ReasonReact.stringToElement "Loading... ") </div>
      </div>
    | Some loaded_snapshots =>
      let clickBackToJobs event => {
        ReactEventRe.Mouse.preventDefault event;
        Js.log "Back to backup jobs";
        backToBackupJobs ()
      };
      let snapshot_list =
        List.sort
          (
            fun (a: Types.snapshot) (b: Types.snapshot) =>
              if (a.start_time < b.start_time) {
                1
              } else if (
                a.start_time > b.start_time
              ) {
                (-1)
              } else {
                0
              }
          )
          loaded_snapshots
        |> List.map (fun snapshot => <Snapshot snapshot />)
        |> Array.of_list;
      <div>
        <h1> (ReasonReact.stringToElement "Snapshots ") </h1>
        <a href="#" onClick=clickBackToJobs>
          (ReasonReact.stringToElement "Back to backup jobs")
        </a>
        <div>
          (
            ReasonReact.stringToElement (
              "Number of Snapshots  "
              ^ string_of_int (List.length loaded_snapshots)
            )
          )
        </div>
        (ReasonReact.createDomElement "ul" props::{"foo": "a"} snapshot_list)
      </div>
    }
};
