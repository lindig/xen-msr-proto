module Xen = struct
  external init : unit -> unit = "caml_msr_init"

  external get : unit -> string * string = "caml_msr_get" (* cpu, msr *)
end

let init () = Xen.init ()

let get () = Xen.get ()
