
#include <stdlib.h>
#include <stdio.h>
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

CAMLprim        value
caml_msr_init(value val_unit)
{
        CAMLparam1(val_unit);
        xen_init();
        CAMLreturn(Val_unit);
}

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

        ret = caml_alloc_tuple(2);
        Store_field(ret, 0, caml_alloc_string(nr_cpu * sizeof(cpu_policy_t)));
        Store_field(ret, 1, caml_alloc_string(nr_msr * sizeof(msr_member_t)));


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
