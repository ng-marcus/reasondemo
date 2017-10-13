open Bs_fetch;

type loginresponse = {
  id: int,
  auth_token: string
};

type state = {
  email: string,
  password: string
};

type actions =
  | ChangeEmail string
  | ChangePassword string;

let component = ReasonReact.reducerComponent "Login";

let loginUrl = "https://my.ironfile.com/sessions.json";

let headers = Bs_fetch.HeadersInit.make {"Content-Type": "application/json"};

let parseResponseJson json :loginresponse =>
  Json.Decode.{
    id: field "id" int json,
    auth_token: field "auth_token" string json
  };

let doLogin email password => {
  let body =
    BodyInit.make (
      "{\"email\":\"" ^ email ^ "\",\"password\":\"" ^ password ^ "\"}"
    );
  fetchWithInit loginUrl (RequestInit.make method_::Post ::body ::headers ())
  |> Js.Promise.then_ Bs_fetch.Response.text
  |> Js.Promise.then_ (
       fun jsonText =>
         /* Js.log ("received response " ^ jsonText); */
         Js.Promise.resolve (parseResponseJson (Js.Json.parseExn jsonText))
     )
};

let add x y => x + y;

let add1 = add 1;

let make ::loggedIn _children => {
  ...component,
  initialState: fun () => {
    email: "",
    password: ""
  },
  reducer: fun action state =>
    switch action {
    | ChangeEmail text => ReasonReact.Update {...state, email: text}
    | ChangePassword text => ReasonReact.Update {...state, password: text}
    },
  didMount: fun self => {
    Js.log "partial ";
    Js.log (string_of_int (add1 4));
    ReasonReact.NoUpdate
  },
  render: fun self => {
    let emailChange event =>
      ChangeEmail (ReactDOMRe.domElementToObj (ReactEventRe.Form.target event))##value;
    let passwordChange event =>
      ChangePassword (
                       ReactDOMRe.domElementToObj (
                         ReactEventRe.Form.target event
                       )
                     )##value;
    let clickLogin event => {
      ReactEventRe.Mouse.preventDefault event;
      doLogin self.state.email self.state.password
      |> Js.Promise.then_ (
           fun response => {
             /* Js.log response.auth_token; */
             loggedIn response.auth_token;
             Js.Promise.resolve ()
           }
         );
      ()
    };
    <div>
      <h1> (ReasonReact.stringToElement "IronFile Login ") </h1>
      <div>
        <input
          className="login-field"
          placeholder="email address"
          value=self.state.email
          onChange=(self.reduce emailChange)
          autoFocus=Js.true_
        />
        <br />
        <input
          className="password-field"
          placeholder="password"
          value=self.state.password
          onChange=(self.reduce passwordChange)
        />
      </div>
      <a href="#" onClick=clickLogin>
        (ReasonReact.stringToElement "Login ")
      </a>
    </div>
  }
};
