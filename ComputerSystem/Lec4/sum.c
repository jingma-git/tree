long plus(long x, long y)
{
    return x + y;
}

void sumstore(long x, long y, long *dst)
{
    // rdi holds the 1st integer argument to a function
    // rsi holds the 2nd integer argument to a function
    // rdx holds the 3rd integer argument to a function
    // rax holds the return value
    long t = plus(x, y); // pushq %rbx; movq %rdx, %rbx; call plus;
    *dst = t;            // moveq %rax, (%rbx)
    // popq %rbx;
}

int main()
{
    return 0;
}