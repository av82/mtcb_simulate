#include <stdio.h>
#include <stdlib.h>
#include <time.h>
// 122 bytes per block per leaf - leaf size 5 sub ids - each of 13 bytes + 57 bytes of Sid data- total 65 + 57 = 122 bytes

#define BLOCK1 0x00000000 // address of block 1
#define BLOCK2 0x00000080 // from 128th byte to next block 3
#define BLOCK3 0x00000100 // from 256th byte to next block 4
#define BLOCK4 0x00000180 // from  384 th byte
#define temp 0x00000200   // for temporary value obtained after OP_2 8 bytes

#define ROOT 0x0000208 // root - 20 bytes .

typedef unsigned char byte;
\\8 bits typedef long long int byte_8;
\\64 bits struct timespec tp;

typedef struct
{
        byte ADR_1;
        byte ADR_2;
        byte OP_2;
        byte ADR_3;
        byte OP_3;
        byte_8 VAL;
} Op_Val_lst;

typedef struct
{
        byte_8 S_ID;
        byte_8 MOD_ID;
        byte_8 OFFSET;
        byte_8 LIFE_T;

        byte_8 CURR_VAL;
        byte_8 PRE_VAL;
        byte_8 EXP_T;
        byte OP_1;

        Op_Val_lst OP_LST[5];
} leaf;

leaf *const BLK1 = (leaf *)BLOCK1;
leaf *const BLK2 = (leaf *)BLOCK2;
leaf *const BLK3 = (leaf *)BLOCK3;
leaf *const BLK4 = (leaf *)BLOCK4;

void main()
{

        byte_8 a, b, c, res;
        leaf L;
        Op_Val_lst OP;

        printf("size of leaf is:%d %d %d", sizeof(L), sizeof(OP), sizeof(a));
        scanf("%llu", &a);
        scanf("%d\n", &b);

        int msb_count(byte_8 num)
        {
                int count = 0;
                while (num)
                {
                        num = (byte_8)num >> 1;
                        count++;
                }
                printf("\n msb position:%d\n", count);
                return count;
        }

        byte comp(byte_8 a)
        {
                byte_8 b = (byte_8)(a);
                int msb_pos = msb_count(b);
                int i, k, mask;

                for (i = msb_pos; i >= 0; i--)
                {
                        mask = 1 << i;
                        k = b & mask;
                        if (k == 0)
                        {
                                //  printf("0");
                                b = b | (1 << i);
                        }
                        else

                        {
                                //printf("1");
                                b = b & (~(1 << i));
                        }
                }
                //printf("complemented value of b:%d",(b));
                return b;
        }

        int com = comp(a);
/*
scanf("%d\n",&c);

*/}

//opcodes_eval takes inputs a,b,current value's addresses, opcode value. a and b could be constants in few cases where range is checked. else could be address of any other reference.
byte_8 Opcodes_eval(byte *a, byte *b, byte *cur, byte opval)
{
        switch (opval)
        {
        case 1:
                return (*a + *b);
                printf("\n Addition:%d", res);
                break;
        case 2:
                return (*a - *b);
                ;
                printf("\n Subtraction:%d", res);
                break;
        case 3:
                return (*a & *b);
                ;
                printf("\n AND %d", res);
                break;
        case 4:
                return (*a | *b);
                ;
                printf("\n OR %d", res);
                break;
        case 5:
                return (*a ^ *b);
                ;
                printf("\n XOR %d", res);
                break;
        case 6:
                if (*a > *b)
                        return 1;
                else
                        return 0;

                printf("\n Greater %d", res);
                break;

        case 7:
                if (*a >= *b)
                        return 1;
                else
                        return 0;

                printf("\n GEQ %d", res);
                break;
        case 8:
                if (*a == *b)
                        return 1;
                else
                        return 0;

                printf("\n EQ %d", res);
                break;

        case 9:
                return (~(*a & *b));

                printf("\n NAND %d", res);
                break;
        case 10:
                return (~(*a | *b));

                printf("\n NOR %d", res);
                break;
        case 11:
                return (~(*a ^ *b));

                printf("\n XNOR %d", res);
                break;

        case 12:
                if ((*a < *cur) & (*cur < *b))
                        return 1;
                else
                        return 0;
                printf("\n LL %d".res);
                break;

        case 13:
                if ((*a <= *cur) & (*cur <= *b))
                        return 1;
                else
                        return 0;
                printf("\n LLE %d".res);
                break;

        case 14:
                if ((*a > *cur) & (*cur < *b))
                        return 1;
                else
                        return 0;
                printf("\n GL %d".res);
                break;

        case 15:
                if ((*a >= *cur) & (*cur <= *b))
                        return 1;
                else
                        return 0;
                printf("\n GLE %d".res);
                break;

        case 16:
                if ((*a < *cur) & (*cur > *b))
                        return 1;
                else
                        return 0;
                printf("\n GG %d".res);
                break;

        case 17:
                if ((*a <= *cur) & (*cur >= *b))
                        return 1;
                else
                        return 0;
                printf("\n GGE %d".res);
                break;

        case 0:
                printf("\n Choice Terminated");
                exit(1);
                break;
        default:
                printf("\n Invalid Choice");
        }
}

int parse(leaf L, int FLG_LOADED)
{
        \\flag_loaded is set to one upon hmac verification for 3 leaves incase of syn state else for only 1;
        only after which parse is done.

            int sub_id;
        clock_gettime(CLOCK_REALTIME, &tp);
\\ for nano seconds tp.tv_nsec
	byte_8  CurrentTickCount = (byte_8)tp.tv_sec;
\\ to save result of OP_2 to operate on OP_3 unsigned long long temp_val;
if (FLG_LOADED == 1)
{
        if (msb_count(L.S_ID) == 64)
        {
        }
        else if (msb_count(L.S_ID) != 64)
        {

                if ((L.EXP_T + L.OFFSET) >= CurrentTickCount)
                {
                        L.PRE_VAL = Opcodes_eval(L.CURR_VAL, L.PRE_VAL, L.OP_1);
                        while ((L.OP_LST[sub_id].OP_2 != 0) && (sub_id <= 6))
                        {
                                temp_val = Opcodes_eval(&L.OP_LST[sub_id].ADR_1, &L.OP_LST[sub_id].ADR_2, &L.CURR_VAL, L.OP_LST[sub_id].OP_2);

                                if (L.OP_LST[sub_id].OP_3 != 0)
                                {
                                        *L.OP_LST[sub_id].VAL = Opcodes_eval(&temp_val, &L.OP_LST[sub_id].ADR_3, &L.CURR_VAL, L.OP_LST[sub_id].OP_3); // compute the VALUE for this subid when operation involves 3 ops.
                                }
                                else
                                {
                                        *L.OP_LST[sub_id].VAL = temp_val; // if secondary OPCode is 0 then temp value is the value for that subid
                                }

                                sub_id++; // goto next subid
                        }
                }
                else
                        return 0;
        }
}
}

// load leaves into . memory in the order
// order determines the block to be assigned, final determines wait for final leaf/ not.
//passes a pointer to public address space where a struct is copied by the monitor

int Ld_rcv_recd(leaf *l, int order, int final)
{

        if (order == 1)
        {
                memcpy(BLK1, &l, sizeof(BLK1))
        }
        else
                iff(order == 2)
                {
                        memcpy(BLK1, &l, sizeof(BLK1))
                }
        else if (order == 3)
        {
                memcpy(BLK1, &l, sizeof(BLK1))
        }
        else if (order == 4)
        {
                memcpy(BLK1, &l, sizeof(BLK1))
        }

        if (final == 0)
        {
                memlock(BLK1, 0x000021C)
        }
}