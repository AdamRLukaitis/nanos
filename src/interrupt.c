#include "interrupt.h"
#include "kernel.h"
#include "timer.h"
#include "schedule.h"
#include "intel_8042_nanos.h"
#include "intel_8254_nanos.h"
#include "syscall.h"
#include "asm.h"
#include "intel_8259.h"

void enable_irq(int irqno) {
    intel_8259_enable_irq(irqno);
}

void disable_irq(int irqno) {
    intel_8259_disable_irq(irqno);
}

void handle_irq(system_state_t *state, irq_e code) {

    // IRQ from kernel mode => was HLT
    bool is_halt = IS_KERNEL_STATE(state);

    if (!is_halt) {
        // HLT => not in schedule
        return_from_schedule(state);
    }

    switch (code) {
        case IRQ0:

            intel_8254_nanos_irq0();

            if (!is_halt) {
                schedule();
            }

            break;

        case IRQ1:
            intel_8042_nanos_handle_irq1();
            break;

        default:
            break;
    }

    EOI();
}

void handle_cpu_exception(system_state_t *state, cpu_exception_e code) {
    kprintf("CPU Exception %d received\n", code);

    while (true) {}
}
