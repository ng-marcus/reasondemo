[%bs.raw {|require('./app.css')|}];

external logo : string = "./logo.svg" [@@bs.module];

type actions =
  | Login string
  | UpdateClients (list Types.client)
  | UpdateServers (list Types.server)
  | UpdateBackupJobs (list Types.backup_job)
  | UpdateSnapshots (list Types.snapshot)
  | UpdateEntities (list Types.entity)
  | SelectClient int
  | SelectServer int;

type state = {
  auth_token: string,
  clients: option (list Types.client),
  servers: option (list Types.server),
  backup_jobs: option (list Types.backup_job),
  snapshots: option (list Types.snapshot),
  entities: option (list Types.entity),
  selected_client: option int,
  selected_server: option int
};

let component = ReasonReact.reducerComponent "App";

/* DirectorRe.setRoute router "/" */
let make route::(route: Types.routes) ::router _children => {
  ...component,
  didMount: fun _self => {
    Js.log "App did mount";
    ReasonReact.NoUpdate
  },
  initialState: fun () => {
    let dt = [%bs.raw {| new Date().getTime() / 1000 |}];
    {
      auth_token: dt,
      clients: None,
      selected_client: None,
      servers: None,
      selected_server: None,
      backup_jobs: None,
      snapshots: None,
      entities: None
    }
  },
  reducer: fun action state =>
    switch action {
    | Login token => ReasonReact.Update {...state, auth_token: token}
    | UpdateClients clients =>
      ReasonReact.Update {
        ...state,
        clients: Some clients,
        selected_server: None,
        selected_client: None,
        servers: None,
        backup_jobs: None,
        snapshots: None,
        entities: None
      }
    | UpdateServers servers =>
      ReasonReact.Update {
        ...state,
        servers: Some servers,
        backup_jobs: None,
        snapshots: None,
        entities: None
      }
    | UpdateBackupJobs backup_jobs =>
      ReasonReact.Update {
        ...state,
        backup_jobs: Some backup_jobs,
        snapshots: None
      }
    | UpdateSnapshots snapshots =>
      ReasonReact.Update {...state, snapshots: Some snapshots}
    | UpdateEntities entities =>
      ReasonReact.Update {...state, entities: Some entities}
    | SelectClient client_id =>
      ReasonReact.Update {
        ...state,
        selected_client: Some client_id,
        selected_server: None,
        servers: None
      }
    | SelectServer server_id =>
      ReasonReact.Update {...state, selected_server: Some server_id}
    },
  render: fun self => {
    let loggedIn auth_token => {
      /* Js.log ("Logging in with token " ^ auth_token); */
      /* Js.log "calling reducer"; */
      self.reduce (fun _ => Login auth_token) ();
      Rest.getClients auth_token
      |> Js.Promise.then_ (
           fun clients => {
             self.reduce (fun _ => UpdateClients clients) ();  /*chenglou remove here for partial application */
             DirectorRe.setRoute router "/";
             Js.Promise.resolve ()
           }
         )
    };
    let showServers client_id => {
      let str_client_id = string_of_int client_id;
      Js.log ("Show servers for client " ^ str_client_id);
      self.reduce (fun _ => SelectClient client_id) ();
      Rest.getServers self.state.auth_token str_client_id
      |> Js.Promise.then_ (
           fun servers => {
             self.reduce (fun _ => UpdateServers servers) ();
             Js.Promise.resolve ()
           }
         );
      DirectorRe.setRoute router ("/clients/" ^ str_client_id ^ "/servers");
      ()
    };
    let showBackupJobs server_id => {
      let str_server_id = string_of_int server_id;
      Js.log ("Show jobs for server " ^ str_server_id);
      self.reduce (fun _ => SelectServer server_id) ();
      Rest.getBackupJobs self.state.auth_token str_server_id
      |> Js.Promise.then_ (
           fun backup_jobs => {
             self.reduce (fun _ => UpdateBackupJobs backup_jobs) ();
             Js.Promise.resolve ()
           }
         );
      DirectorRe.setRoute
        router ("/servers/" ^ str_server_id ^ "/backup_jobs");
      ()
    };
    let showSnapshots backup_job_id => {
      Js.log ("Show snapshots for backup job " ^ backup_job_id);
      Rest.getSnapshots self.state.auth_token backup_job_id
      |> Js.Promise.then_ (
           fun snapshots => {
             self.reduce (fun _ => UpdateSnapshots snapshots) ();
             Js.Promise.resolve ()
           }
         );
      DirectorRe.setRoute
        router ("/backup_jobs/" ^ backup_job_id ^ "/snapshots");
      ()
    };
    let getFolderMetaData snapshot_id => {
      Js.log ("get folder meta data for snapshot " ^ snapshot_id);
      Rest.getFolderMetaData self.state.auth_token snapshot_id
      |> Js.Promise.then_ (
           fun entities => {
             Js.log "ok"; /*("received metadata " ^ metadata);*/
             self.reduce (fun _ => UpdateEntities entities) ();
             Js.Promise.resolve ()
           }
         );
      /* DirectorRe.setRoute
         router ("/backup_jobs/" ^ backup_job_id ^ "/snapshots"); */
      ()
    };
    let backToServers () => {
      Js.log "App Back to servers";
      Js.log self.state.selected_client;
      switch self.state.selected_client {
      | None => ()
      | Some client_id =>
        Js.log "Set route";
        DirectorRe.setRoute
          router ("/clients/" ^ string_of_int client_id ^ "/servers");
        ()
      }
    };
    let backToBackupJobs () => {
      Js.log "App Back to backup jobs";
      switch self.state.selected_server {
      | None => ()
      | Some server_id =>
        Js.log "Set route";
        DirectorRe.setRoute
          router ("/servers/" ^ string_of_int server_id ^ "/backup_jobs");
        ()
      }
    };
    let backToClients () => {
      DirectorRe.setRoute router "/";
      ()
    };
    /* let token = self.state.auth_token; */
    let element =
      switch route {
      | LoginRoute => <Login loggedIn />
      | ClientsRoute => <Clients clients=self.state.clients showServers />
      | ServersRoute client_id =>
        <Servers
          servers=self.state.servers
          client_id
          showBackupJobs
          backToClients
        />
      | BackupJobsRoute server_id =>
        <BackupJobs
          backup_jobs=self.state.backup_jobs
          server_id
          showSnapshots
          backToServers
        />
      | SnapshotsRoute backup_job_id =>
        <Snapshots
          snapshots=self.state.snapshots
          backup_job_id
          backToBackupJobs
        />
      | FoldersRoute snapshot_id =>
        <FolderContent
          snapshot_id
          entities=self.state.entities
          getFolderMetaData
        />
      };
    <div className="App">
      <h2>
        (ReasonReact.stringToElement ("token:" ^ self.state.auth_token))
      </h2>
      element
    </div>
  }
};
