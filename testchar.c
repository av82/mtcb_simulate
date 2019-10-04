#include <stdio.h>
#include <stdlib.h>
#include <math.h>
typedef struct
{
        unsigned char ADR_1;
        unsigned char ADR_2;
        unsigned char OP_2;
        unsigned char ADR_3;
        unsigned char OP_3;
} Op_Val_lst;

typedef struct
{
        unsigned long long S_ID;
        unsigned long long MOD_ID;
        unsigned long long OFFSET;
        unsigned long long LIFE_T;

        unsigned long long CURR_VAL;
        unsigned long long PRE_VAL;
        unsigned long long EXP_T;
        unsigned char OP_1;

        Op_Val_lst OP_LST[6];
} leaf;

void main()
{
        typedef unsigned char byte;
        unsigned long long a, b;
        byte c;
        unsigned long long res;

        scanf("%d", &c);
        if (c < 4 && c >= 1)
        {
                printf("enter a,b\n");
        }
        scanf("%llu%llu", &a, &b);
        switch (c)
        {
        case 1:
                res = (a + b);
                printf("\n Addition:%llu", res);
                break;
        case 2:
                res = llabs(a - b);
                printf("\n Subtraction:%llu", (res));
                break;
        case 3:
                res = a * b;
                printf("\n Multiplication:%llu", res);
                break;
        case 4:
                res = a / b;
                printf("\n Division:%f", res);
                break;
        case 0:
                printf("\n Choice Terminated");
                exit(1);
                break;
        default:
                printf("\n Invalid Choice");
        }
}
