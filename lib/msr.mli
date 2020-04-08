
type t = { cpu : string; msr : string; version : int }

val init: unit -> unit
val get: unit -> t
val set: t -> unit

