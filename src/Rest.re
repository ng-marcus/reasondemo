let base_url = "https://my.ironfile.com";

let make_auth_headers auth_token =>
  Bs_fetch.HeadersInit.make {
    "Content-Type": "application/json",
    "Authorization": "Bearer " ^ auth_token
  };

let parseClientJson json :Types.client =>
  Json.Decode.{
    id: field "id" int json,
    short_name: field "short_name" string json,
    name: field "name" string json,
    status: field "status" string json
  };

let parseServerJson json :Types.server =>
  Json.Decode.{
    id: field "id" int json,
    name: field "name" string json,
    status: field "status" string json
  };

let parseBackupJobJson json :Types.backup_job =>
  Json.Decode.{
    id: field "id" int json,
    name: field "name" string json,
    status: field "status" string json
  };

let parseSnapshotJson json :Types.snapshot =>
  Json.Decode.{
    id: field "id" int json,
    description: field "description" string json,
    status: field "status" string json,
    file_count: field "file_count" int json,
    start_time: field "start_time" string json,
    end_time: optional (field "end_time" string) json
  };

let parseFolderJson json :Types.entity =>
  Json.Decode.{
    name: "",
    s_id: field "s_id" int json,
    size: field "size" int json,
    folder: field "folder" bool json
  };

/* let parseClients json :list client => Json.Decode.(list parseClientJson); */
let decodeListOfStrings json :list string => Json.Decode.(json |> list string);

let decodeListOfClients json :list Types.client =>
  Json.Decode.(json |> list parseClientJson);

let decodeListOfServers json :list Types.server =>
  Json.Decode.(json |> list parseServerJson);

let decodeListOfBackupJobs json :list Types.backup_job =>
  Json.Decode.(json |> list parseBackupJobJson);

let decodeListOfSnapshots json :list Types.snapshot =>
  Json.Decode.(json |> list parseSnapshotJson);

/* let decodeHashOfFolders json :list Types.folder =>
   Json.Decode.(json |> list parseFolderJson |> Js.Option.getExn); */
let getClients auth_token => {
  let headers = make_auth_headers auth_token;
  let clients_url = base_url ^ "/clients.json";
  Bs_fetch.fetchWithInit
    clients_url (Bs_fetch.RequestInit.make method_::Get ::headers ())
  |> Js.Promise.then_ Bs_fetch.Response.text
  |> Js.Promise.then_ (
       fun jsonText =>
         /* Js.log ("received server response " ^ jsonText); */
         Js.Promise.resolve (decodeListOfClients (Js.Json.parseExn jsonText))
     )
};

let getServers auth_token client_id => {
  let headers = make_auth_headers auth_token;
  let servers_url = base_url ^ "/clients/" ^ client_id ^ "/servers";
  Bs_fetch.fetchWithInit
    servers_url (Bs_fetch.RequestInit.make method_::Get ::headers ())
  |> Js.Promise.then_ Bs_fetch.Response.text
  |> Js.Promise.then_ (
       fun jsonText =>
         /* Js.log ("received server response " ^ jsonText); */
         Js.Promise.resolve (decodeListOfServers (Js.Json.parseExn jsonText))
     )
};

let getBackupJobs auth_token server_id => {
  let headers = make_auth_headers auth_token;
  let servers_url = base_url ^ "/servers/" ^ server_id ^ "/backup_jobs";
  Bs_fetch.fetchWithInit
    servers_url (Bs_fetch.RequestInit.make method_::Get ::headers ())
  |> Js.Promise.then_ Bs_fetch.Response.text
  |> Js.Promise.then_ (
       fun jsonText =>
         /* Js.log ("received server response " ^ jsonText); */
         Js.Promise.resolve (
           decodeListOfBackupJobs (Js.Json.parseExn jsonText)
         )
     )
};

let getSnapshots auth_token backup_job_id => {
  let headers = make_auth_headers auth_token;
  let snapshots_url =
    base_url ^ "/backup_jobs/" ^ backup_job_id ^ "/snapshots";
  Bs_fetch.fetchWithInit
    snapshots_url (Bs_fetch.RequestInit.make method_::Get ::headers ())
  |> Js.Promise.then_ Bs_fetch.Response.text
  |> Js.Promise.then_ (
       fun jsonText =>
         /* Js.log ("received server response " ^ jsonText); */
         Js.Promise.resolve (decodeListOfSnapshots (Js.Json.parseExn jsonText))
     )
};

let getFolderMetaData auth_token snapshot_id => {
  let headers = make_auth_headers auth_token;
  let folder_metadata_url =
    base_url ^ "/entities/" ^ snapshot_id ^ "/show_folder?path=/";
  Bs_fetch.fetchWithInit
    folder_metadata_url (Bs_fetch.RequestInit.make method_::Get ::headers ())
  |> Js.Promise.then_ Bs_fetch.Response.text
  |> Js.Promise.then_ (
       fun jsonText =>
         {
           Js.log ("received server response " ^ jsonText);
           /* Js.Promise.resolve "string"; */
           /* Js.Promise.resolve (decodeHashOfFolders (Js.Json.parseExn jsonText)); */
           let first_parse =
             Js.Json.parseExn jsonText
             |> Js.Json.decodeObject
             |> Js.Option.getExn;
           let keys = Js.Dict.keys first_parse;
           let remapped =
             Js.Array.map
               (
                 fun key => {
                   let json = Js.Dict.get first_parse key;
                   let decoded =
                     Json.Decode.(json |> Js.Option.getExn |> parseFolderJson);
                   let entry: Types.entity = {
                     name: key,
                     s_id: decoded.s_id,
                     size: decoded.size,
                     folder: decoded.folder
                   };
                   entry
                 }
               )
               keys;
           Js.log remapped;
           Js.Promise.resolve (Array.to_list remapped)
         }
         /* Js.Promise.resolve (decodeListOfSnapshots (Js.Json.parseExn jsonText)) */
     )
};
