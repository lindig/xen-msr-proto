
module Xen = struct
  external init : unit -> unit = "xen_init"
end

let init () = Xen.init ()

