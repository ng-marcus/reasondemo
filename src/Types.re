type client = {
  id: int,
  short_name: string,
  name: string,
  status: string
};

type server = {
  id: int,
  name: string,
  status: string
};

type backup_job = {
  id: int,
  name: string,
  status: string
};

type snapshot = {
  id: int,
  description: string,
  status: string,
  file_count: int,
  start_time: string,
  end_time: option string
};

type entity = {
  s_id: int,
  size: int,
  name: string,
  folder: bool
};

type routes =
  | LoginRoute
  | ClientsRoute
  | ServersRoute int
  | BackupJobsRoute int
  | SnapshotsRoute int
  | FoldersRoute int;
