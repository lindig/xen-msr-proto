type t

val init : unit -> unit

val get : unit -> t

val set : t -> unit

val version : t -> int

val nr_cpu : t -> int

val nr_msr : t -> int

val to_string : t -> string * string
