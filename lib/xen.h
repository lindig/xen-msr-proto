
#include <stdint.h>

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
