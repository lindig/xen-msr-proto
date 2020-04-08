
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

#include "xen.h"

static cpu_policy_t cpu = {{1, 2, 3, 4, 5}, "cpu"};
static msr_member_t msr = {{1, 2}, "msr"};

static cpu_policy_t cpus[10];
static msr_member_t msrs[5];

static int      initialized = 0;

void
xen_get_max_sizes(uint32_t * nr_cpu, uint32_t * nr_msr)
{
        if (!initialized) {
                *nr_cpu = 0;
                *nr_msr = 0;
        } else {
                *nr_cpu = (uint32_t) (sizeof(cpus) / sizeof(cpu));
                *nr_msr = (uint32_t) (sizeof(msrs) / sizeof(msr));
        }
}

int
xen_get_cpu(uint32_t index, cpu_policy_t * out)
{
        memcpy(out, &cpus[index], sizeof(cpu_policy_t));
        return 0;
}

int
xen_get_msr(uint32_t index, msr_member_t * out)
{
        memcpy(out, &msrs[index], sizeof(msr_member_t));
        return 0;
}

void
xen_init(void)
{
        int             i;
        for (i = 0; i < sizeof(cpus) / sizeof(cpu); i++) {
                memcpy(&cpus[i], &cpu, sizeof cpu);
                cpus[i].n[0] = i;
        }
        for (i = 0; i < sizeof(msrs) / sizeof(msr); i++) {
                memcpy(&msrs[i], &msr, sizeof(msr));
                msrs[i].n[0] = i;
        }
        initialized = 1;
}
