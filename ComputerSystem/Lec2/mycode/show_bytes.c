#include <stdio.h>
#include <string.h>

typedef unsigned char *byte_pointer;

void show_bytes(byte_pointer start, size_t len)
{
    size_t i;
    for (i = 0; i < len; ++i)
        printf("%p\t0x%.2x\n", &start[i], start[i]); // usnsigned hexidecimal
    printf("\n");
}

void show_int(int x)
{
    show_bytes((byte_pointer)&x, sizeof(int));
}

void show_float(float x)
{
    show_bytes((byte_pointer)&x, sizeof(float));
}

void show_pointer(void *x)
{
    show_bytes((byte_pointer)&x, sizeof(void *));
}

void test_show_bytes(int val)
{
    int ival = val;
    float fval = (float)val;
    int *pval = &ival;
    printf("Stack ival=%d\n", ival);
    printf("int ival:\n");
    show_int(ival);
    printf("float ival:\n");
    show_float(fval);
    printf("&ival:\n");
    show_pointer(pval);
}

void simple_show_a()
{
    int val = 0x87654321;
    byte_pointer valp = (byte_pointer)&val;
    show_bytes(valp, 1); /* A. */
    show_bytes(valp, 2); /* B. */
    show_bytes(valp, 3); /* C. */
}

void simple_show_b()
{
    int val = 0x12345678;
    byte_pointer valp = (byte_pointer)&val;
    show_bytes(valp, 1); /* A. */
    show_bytes(valp, 2); /* B. */
    show_bytes(valp, 3); /* C. */
}

void float_eg()
{
    int x = 3490593;
    float f = (float)x;
    printf("For x = %d\n", x);
    show_int(x);
    show_float(f);

    x = 3510593;
    f = (float)x;
    printf("For x = %d\n", x);
    show_int(x);
    show_float(f);
}

void string_ueg()
{
    const char *s = "ABCDEFG";
    show_bytes((byte_pointer)s, strlen(s));
}

void string_leg()
{
    const char *s = "abcdef";
    show_bytes((byte_pointer)s, strlen(s));
}

void show_twocomp()
{
    short x = 12345;
    short mx = -x;

    show_bytes((byte_pointer)&x, sizeof(short));
    show_bytes((byte_pointer)&mx, sizeof(short));
}

int main()
{
    // int val = 12345;
    // test_show_bytes(val);
    // show_twocomp();
    // simple_show_a();
    // simple_show_b();
    // float_eg();
    // string_ueg();
    string_leg();
}