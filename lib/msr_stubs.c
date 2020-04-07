
#include <errno.h>
#include <sys/resource.h>
#include <unistd.h>
#include <caml/mlvalues.h>
#include <caml/callback.h>
#include <caml/memory.h>
#include <caml/fail.h>
#include <caml/alloc.h>
#include <caml/signals.h>
#include <caml/unixsupport.h>

#include "xen.h"

CAMLprim value 
caml_msr_init(value val_unit)
{
	CAMLparam1(val_unit);
	xen_init();
	CAMLreturn(Val_unit);
}

/* vim: set ts=8 noet: */
