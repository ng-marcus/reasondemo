[%bs.raw {|require('./index.css')|}];

external register_service_worker : unit => unit =
  "default" [@@bs.module "./registerServiceWorker"];

let router =
  DirectorRe.makeRouter {
    "/login": "login",
    "/": "clients",
    "/clients/:client_id/servers": "servers",
    "/servers/:server_id/backup_jobs": "backup_jobs",
    "/backup_jobs/:backup_job_id/snapshots": "snapshots",
    "/folders/:snapshot_id": "folders"
  };

let renderForRoute (route: Types.routes) => {
  let element = <App route router />;
  ReactDOMRe.renderToElementWithId element "root";
  ()
};

let handlers = {
  "login": fun () => renderForRoute LoginRoute,
  "clients": fun () => renderForRoute ClientsRoute,
  "servers": fun (client_id: string) =>
    renderForRoute (ServersRoute (int_of_string client_id)),
  "backup_jobs": fun (server_id: string) =>
    renderForRoute (BackupJobsRoute (int_of_string server_id)),
  "snapshots": fun (backup_job_id: string) =>
    renderForRoute (SnapshotsRoute (int_of_string backup_job_id)),
  "folders": fun (snapshot_id: string) =>
    renderForRoute (FoldersRoute (int_of_string snapshot_id))
};

DirectorRe.configure router {"html5history": true, "resource": handlers};

DirectorRe.init router "/login";

register_service_worker ();
