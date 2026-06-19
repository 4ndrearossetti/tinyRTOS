# tinyRTOS

A minimal preemptive RTOS for ARM Cortex-M, built from scratch to understand schedulers, context switching, TCBs, and interrupts at the register and stack level. A learning project — not a FreeRTOS replacement.

## Why two phases

The interesting parts of an RTOS — the scheduler, the run queue, the blocking primitives, the tick — are independent of *how* the context switch is implemented. So the project splits in two:

- **Phase B (current):** the scheduler logic, running on Linux. Context switching uses `ucontext.h` (POSIX), and the tick is driven by a `SIGALRM` handler armed via `setitimer`. The point of this phase is to get the scheduler correct in a debuggable environment, with nothing but plain `gdb`, before adding the complexity of bare-metal hardware.
- **Phase A (next, when hardware arrives):** the same scheduler, running on an STM32F103 Blue Pill via SWD with an ST-Link V2 clone. Bare metal — no HAL, hand-written linker script and startup, hand-written context switch in ARMv7-M assembly, SysTick driving the tick, PendSV doing the switch.

Concretely, the migration replaces two files (`context.c` and `timer.c`) and leaves the rest of the scheduler unchanged. SIGALRM is the laptop's stand-in for SysTick; `ucontext`'s `swapcontext` is the stand-in for hand-written register save/restore. Same model, different mechanism.

## What's built (Phase B)

- TCB with `ucontext_t`, private 16 KB stack, state (`READY`/`BLOCKED`), and `wake_time`.
- Cooperative scheduler: `yield()` scans for blocked tasks whose wake_time has elapsed, marks them ready, then switches to the next ready task in round-robin order.
- `task_delay(ticks)` as the blocking primitive: marks the current task BLOCKED with `wake_time = system_ticks + ticks`, then yields.
- Idle task always READY, runs when nothing else can, keeps the run queue non-empty.
- 10 ms tick driven from outside the scheduler by `SIGALRM`, registered with `sigaction` and armed with `setitimer(ITIMER_REAL, ...)`. The handler does nothing but bump `system_ticks`.

## File layout

```
rtos.h        public types and the scheduler API
scheduler.c   TCBs, current_idx, yield, task_delay
context.c    init_tcb and ucontext setup (replaced in Phase A)
timer.c       SIGALRM handler and setitimer arming (replaced in Phase A)
main.c        task definitions and main()
```

The boundary between "scheduler logic" and "context/timer mechanism" is the boundary between what stays and what gets rewritten on the Cortex-M.

## Build and run

```
make
make run
```

Two tasks print at different rates (task_a every 3 ticks, task_b every 5 ticks). With a 10 ms tick that's roughly 33 Hz and 20 Hz. Output is paced by wall-clock time — readable, not a blur. Ctrl-C to stop.

## Scope and non-goals

This is deliberately small. Things explicitly not implemented:

- **Preemption.** Cooperative only. Tasks switch on `yield` or `task_delay`. The handler bumps the tick but never forces a switch.
- **Synchronization primitives.** No mutexes, semaphores, queues, or notifications.
- **Dynamic task creation.** TCBs are file-scope and fixed.
- **Stack overflow detection.**
- **Priorities.** Round-robin among all READY tasks.

These are interesting next steps once the Cortex-M target is up, but they're not the current focus. Phase A's goal is to get the existing scheduler running on bare metal with a real hardware context switch; everything else can be layered on after.

## Hardware target

STM32F103C8T6 (Blue Pill), ST-Link V2 clone, SWD. ARMv7-M, Cortex-M3. No vendor HAL or CubeMX — own linker script, own startup, own SysTick + PendSV handlers, own context switch in `.s` files.

