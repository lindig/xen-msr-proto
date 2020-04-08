type blob

type t = { cpu : blob; msr : blob; version : int }

val init : unit -> unit

val to_string : blob -> string

val get : unit -> t

val set : t -> unit
