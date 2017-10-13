let component = ReasonReact.statelessComponent "BackupJobs";

let make ::backup_jobs ::server_id ::showSnapshots ::backToServers children => {
  ...component,
  didMount: fun self => {
    Js.log "BackupJobs did mount";
    ReasonReact.NoUpdate
  },
  render: fun _self =>
    switch backup_jobs {
    | None =>
      <div>
        <h1> (ReasonReact.stringToElement "Backup Jobs ") </h1>
        <div> (ReasonReact.stringToElement "Loading... ") </div>
      </div>
    | Some loaded_backup_jobs_list =>
      let clickBackToServers event => {
        ReactEventRe.Mouse.preventDefault event;
        Js.log "Back to servers";
        backToServers ()
      };
      let backup_job_list =
        Array.of_list (
          List.map
            (fun backup_job => <BackupJob backup_job showSnapshots />)
            loaded_backup_jobs_list
        );
      <div>
        <a href="#" onClick=clickBackToServers>
          (ReasonReact.stringToElement "Back to servers")
        </a>
        <h1> (ReasonReact.stringToElement "Backup Jobs ") </h1>
        <div>
          (
            ReasonReact.stringToElement (
              "Number of Backup Jobs "
              ^ string_of_int (List.length loaded_backup_jobs_list)
            )
          )
        </div>
        (ReasonReact.createDomElement "ul" props::{"foo": "a"} backup_job_list)
      </div>
    }
};
