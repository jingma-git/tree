// asm: gcc -Og -S main.c // basic optimization -Og

int add(int a, int b)
{
    return a + b;
}
int main()
{
    return add(2, 3);
}