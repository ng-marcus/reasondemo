let component = ReasonReact.statelessComponent "BackupJob";

let make backup_job::(backup_job: Types.backup_job) ::showSnapshots children => {
  ...component,
  render: fun _self => {
    let clickBackupJob event => {
      ReactEventRe.Mouse.preventDefault event;
      showSnapshots (string_of_int backup_job.id)
    };
    <li>
      <a href="#" onClick=clickBackupJob>
        (
          ReasonReact.stringToElement (
            backup_job.name ^ " (" ^ backup_job.status ^ ")"
          )
        )
      </a>
    </li>
  }
};
