module C = Cmdliner

let run () =
  Xen.Msr.init ();
  let x = Xen.Msr.get () in
  Printf.printf "cpu: %s\n" (String.escaped x.Xen.Msr.cpu);
  Printf.printf "msr: %s\n" (String.escaped x.Xen.Msr.msr);
  Printf.printf "ver: %d\n" x.Xen.Msr.version;
  Xen.Msr.set { x with version = 200 }

module Command = struct
  let help =
    [
      `P "These options are common to all commands."
    ; `S "MORE HELP"
    ; `P "Use `$(mname) $(i,COMMAND) --help' for help on a single command."
    ; `S "BUGS"
    ; `P "Check bug reports at https://github.com/lindig/hello/issues"
    ]

  let main =
    let doc = "Just run the command" in
    C.Term.(const run $ const (), info "hello" ~doc ~man:help)
end

let main () = C.Term.(exit @@ eval Command.main)

let () = if !Sys.interactive then () else main ()
