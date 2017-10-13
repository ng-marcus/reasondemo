let component = ReasonReact.statelessComponent "Snapshot";

let make snapshot::(snapshot: Types.snapshot) children => {
  ...component,
  render: fun _self => {
    let clickSnapshot event => ReactEventRe.Mouse.preventDefault event;
    let end_t =
      switch snapshot.end_time {
      | None => "No end time reported"
      | Some t => t
      };
    <li>
      <a href="#" onClick=clickSnapshot>
        (
          ReasonReact.stringToElement (
            snapshot.start_time
            ^ "  -  "
            ^ end_t
            ^ " ("
            ^ snapshot.status
            ^ ")"
          )
        )
      </a>
    </li>
  }
};
