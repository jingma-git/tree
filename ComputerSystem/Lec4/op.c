#include <stdio.h>
// address computation instruction
// leaq Src, dst
int multi12(int x)
{
    // rdi holds the first integer argument to a function
    // rsi holds the 2nd integer argument to a function
    // rax holds the return value
    asm("leaq (%rdi, %rdi, 2), %rax;" // x + 2 * x
        "salq $2, %rax");             // return t << 2, shift address left, q: 64 bits
}

int op(int x, int y, int z)
{
    // rdi holds the 1st integer argument to a function
    // rsi holds the 2nd integer argument to a function
    // rdx holds the 3rd integer argument to a function
    // rax holds the return value
    asm("leaq (%rdi, %rsi), %rax;"    // t1 = x + y , t1 = 1 + 2
        "addq %rdx, %rax;"            // t2 = z + t1, t2 = 3 + t1 --> 6
        "leaq (%rsi, %rsi, 2), %rdx;" // z = y + 2 * y = 3 * y
        "salq $4, %rdx;"              // z = z << 4 = z * 2^4 = 48 * y = 96
        "leaq 4(%rdi, %rdx), %rcx;"   // t5 = x + z + 4 = 96 + 1 + 4 = 101
        "imulq %rcx, %rax");          // ret = 101 * 6
}

int main()
{
    // printf("%d\n", multi12(2));
    printf("%d\n", op(1, 2, 3));
    return 0;
}