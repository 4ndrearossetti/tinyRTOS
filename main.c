#include <ucontext.h>
#include <stdio.h>

void yield(void (*f));
void task_a();
void task_b();

typedef struct {
        ucontext_t exec_state;
        char stack[16384];
} task_control_board_t;

void task_a() {
        while (1) {
                printf("task_a\n");
                yield(task_b);
        }
}

void task_b() {
        while (1) {
                printf("task_b\n");
                yield(task_a);
        }
}

int main() {

        task_a();

        return 0;
}

