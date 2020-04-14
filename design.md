
# Passing Binary Data between Xen and OCaml

Xen is introducing a new representation for CPU capabilities. Its
internal structure is hidden to outside code but there is a need to
associate these CPU capabilities with VMs beyond the lifetime of a Xen
domain. This creates the problem how to pass this representation between
Xen and the Toolstack (implemented in OCaml). This document and the code
in this repository aim to explore an interface.

This repository does not rely on Xen but uses some somplistic C code to
make it self contained.  A CPU's capabilities in Xen are represented by
a CPU policy and a set of model-specific registers (MSR), each
represented by a C struct. The capabiities of a host are represented by
two arrays of such values. From an outside perspective each array is a
binary object (blob) that needs to be passed between Xen and OCaml.

## The Xen Interface

The code below represents a simplified interface similar to the real Xen
interface.


    struct cpu_policy {
            int             n[5];
            char            c[4];
    };

    struct msr_member {
            int             n[2];
            char            c[4];
    };

    typedef struct cpu_policy cpu_policy_t;
    typedef struct msr_member msr_member_t;

    void            xen_init(void);
    void            xen_get_max_sizes(uint32_t * nr_cpu, uint32_t * nr_msr);
    int             xen_get_cpu(uint32_t index, cpu_policy_t * out);
    int             xen_get_msr(uint32_t index, msr_member_t * out);


To represent CPU policies and an MSR we define two arbitrary structs.
The interesting bits are the functions:

* `xen_get_max_sizes` returns the maximum number _i_ of policies and MSR
  entries, respectively.
* `xen_get_cpu` copies struct _index_ to `out` where `index` must be in
  the range previously obtained by `xen_get_max_sizes`.
* `xen_get_msr` copies struct _index_ to `out`, analogous to above.

## The OCaml Side

Central ideas for the OCaml representation are:

* A value of type `Msr.t` represents CPU capabilities.

* Such a value can be retrieved from and passed to Xen.

* The value is mostly abstract but can be serialised to 
  strings. (This should probably by a single string - here is is a pair
  to keep the implementation simple.)

* Missing: creating a `Msr.t` value from a string.

    type t
    val init : unit -> unit
    val get : unit -> t
    val set : t -> unit
    val version : t -> int
    val to_string : t -> string * string

## The OCaml Implementation 

The OCaml implemenation relies on C bindings but also has access to the
internal structure of `t`:

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
    let version t = t.version
    let to_string t = (String.escaped t.cpu, String.escaped t.msr)

## The C Bindings

The central piece of C is the interface between the OCaml runtime
interface and Xen. The C code allocates an OCaml string to hold the
contents of a C array and uses the Xen functions to fill it one element
at a time.


    CAMLprim        value
    caml_msr_get(value val_unit)
    {
            CAMLparam1(val_unit);
            CAMLlocal1(ret);

            uint32_t        nr_cpu, nr_msr;
            cpu_policy_t   *cpu;
            msr_member_t   *msr;
            int             i;

            xen_get_max_sizes(&nr_cpu, &nr_msr);

            /* allocate Msr.t record */
            ret = caml_alloc_tuple(3);
            Store_field(ret, 0, caml_alloc_string(nr_cpu * sizeof(cpu_policy_t)));
            Store_field(ret, 1, caml_alloc_string(nr_msr * sizeof(msr_member_t)));
            Store_field(ret, 2, Val_int(100));      /* version */


            cpu = (cpu_policy_t *) String_val(Field(ret, 0));
            for (i = 0; i < nr_cpu; i++) {
                    xen_get_cpu(i, cpu);
                    cpu++;
            }

            msr = (msr_member_t *) String_val(Field(ret, 1));
            for (i = 0; i < nr_msr; i++) {
                    xen_get_msr(i, msr);
                    msr++;
            }

            CAMLreturn(ret);
    }
