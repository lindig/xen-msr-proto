type blob = string

type t = { cpu : blob; msr : blob; version : int }

module Xen = struct
  external init : unit -> unit = "caml_msr_init"

  external get : unit -> t = "caml_msr_get"

  external set : t -> unit = "caml_msr_set"
end

let init = Xen.init

let get = Xen.get

let set = Xen.set

let to_string blob = String.escaped blob
