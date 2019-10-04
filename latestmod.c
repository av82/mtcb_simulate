/* 
 * File:   tshav6vectsupd.c
 * Author: Arun Velagapalli
 *
 * Created on May 6, 2011, 11:51 AM
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "shanew.c"

#define swap(x, y) \
    do             \
    {              \
        int tmp;   \
        tmp = x;   \
        x = y;     \
        y = tmp;   \
    } while (0)

extern unsigned char *shafunction();
int Init();
char *buildroot();
void getvector1();
char *dohash();
int *getvector2();
int msb_count();
char *update3l();
int update1();
void display();
void update2l();

//structure for the vector
struct vector
{
    unsigned int vindex;
    unsigned int order;
};

typedef struct vector VT;

struct vectdata
{

    unsigned char vchar[20];

    unsigned int order;
};

typedef struct vectdata VDat;

// below are the array of structs (vectors) to be passed to module/within the monitor to update three nodes in a tree
VT *lvector;   // vector for left child of pair
VT *mvector;   // vector for right  child of pair
VT *cpvector;  // vector from common parent of pair lm to a node of cp of lm , r
VT *rvector;   // vector for third node to a node at height of cp of lm
VT *cprvector; // vector for r node to cp of lm, r
VT *trvector;  // vector for common parent of three nodes to the root

// declaring vector data arrays;

VDat *ldata;
VDat *mdata;
VDat *cpdata;
VDat *rdata;
VDat *cprdata;
VDat *trdata;

int flag = 0;
unsigned char pointcp[20], rpoint[20];
int lvsize, mvsize, cpvsize, rvsize, trvsize, cprvsize;
int globlmph = 0, globlmpind = 0, globrph = 0, globrpind = 0;

unsigned char retvalupd2[20] = {0}, retvalupd1[20] = {0}, retvalupd3[20] = {0};
unsigned char upd1ret[20] = {0}, upd2ret[20] = {0};

unsigned char updata1[40] = {0};
char updata2[40] = {0};
char updata3[40] = {0};
unsigned char hupdata1[20] = {0}, hupdata2[20] = {0}, hupdata3[20] = {0};

int indpr;

struct node
{
    unsigned char data[256];
    int link;
};

typedef struct node N;
N **array; //declare array of structures of nodes.

int main()
{

    unsigned char *root; //pointer to the root
    unsigned char mod_root[20];

    int ht, n, ncolumns, i, j, k = 1;      // denotes height, size of leaves, lenght of array at each level,... respectively
    int indx, indy, indz, cp, ret[2], uno; //index x, y, z, common parent of a pair, a pointer ret to point to return values of function (Common parent/getvector2) , uno denotes the number of updates specified by user
    printf("enter the size of leaves\n");
    scanf("%d", &n);   // enter the number of leaves required
    ht = (int)log2(n); //calculate the height (ht) of the tree

    int l, m, r;
    int lmrorder;

    array = malloc(ht * sizeof(N)); // malloc array of pointers as many as the height

    if (array == NULL)
    {
        fprintf(stderr, "out of memory\n"); //error handling for out of memory when malloc.
        return 0;
    }

    /*Init() function takes the size , pointer to array of pointers, height
 * to initialize the first level of nodes ... (here with dummy values 'AB'
 * and dynamically allocate memory for each level (n/2) each time through loop
 */

    Init(n, array, ht);

    /*
 buildroot returns the root of the tree after creation of the  Merkle tree (root) is a char pointer here points to the return
 * value of the buildroot , again inputs to buildroot are pointer to array of pointers (array), size of leaves at 0 level - n
 * height (ht) of the tree calculated
 */
    root = buildroot(array, n, ht);
    printf("The Root of the Tree is \n");

    for (i = 0; i < 20; i++)
    {
        printf("%02x", root[i]);
    }

    printf("\nEnter the number of updates\n");
    scanf("%d", &uno);
    if (uno <= 3)
    {
        if (uno == 1)
        {
            printf("Enter the index: x\n");
            scanf("%d", &indx);
            getvector1(array, n, ht, indx, 0);
        }

        if (uno == 2)
        {
            printf("Enter the indices x ,y\n");
            scanf("%d", &indx);
            scanf("%d", &indy);
            if (indx < indy)
            {
                getvector2(array, n, indx, indy, 0, ret);
                //printf("common parent of %d, %d is %d at height %d", indx,indy,ret[0],ret[1]);

                if (ret[1] < ht)
                    getvector1(array, n, ht, ret[0], ret[1]);
            }
            else
            {
                getvector2(array, n, indy, indx, ht, ret);
                //printf("common parent of %d, %d is %d at height %d", indy,indx,ret[0],ret[1]);

                if (ret[1] < ht)
                    getvector1(array, n, ht, ret[0], ret[1]);
            }
        }

        if (uno == 3)
        {
            printf("Enter the indices :x,y,z\n");
            scanf("%d", &indx);
            scanf("%d", &indy);
            scanf("%d", &indz);

            if (indy < indx)
                swap(indy, indx);
            if (indz < indy)
                swap(indz, indy);
            if (indy < indx)
                swap(indy, indx);
            unsigned int ind12 = indx ^ indy;
            unsigned int ind23 = indz ^ indy;

            //printf("%d,%d\n", ind12,ind23);
            int i, j;

            i = msb_count(ind12);
            j = msb_count(ind23);
            //printf("%d,%d\n",i,j);

            if (i > j)
            {
                l = indy;
                m = indz;
                r = indx;
                lmrorder = 1;
                getvector2(array, n, indy, indz, 0, ret);
                //printf("common parent of %d, %d is %d at height %d\n", indy,indz,ret[0],ret[1]);

                cp = ret[0];
                int cph = ret[1];
                int l2 = getvector3c(array, n, indx, cph);
                flag = 1;
                getvector2(array, n, l2, cp, cph, ret);

                // printf("common parent of nodes at height %d,index %d and %d is:%d at height %d\n",cph,cp,l2,ret[0],ret[1]);
                cp = ret[0];
                cph = ret[1];
                //printf("height:h %d\n" ,cph);

                if (cph < ht)
                    getvector1(array, n, ht, cp, cph);
            }

            else
            {

                l = indx;
                m = indy;
                r = indz;
                lmrorder = 0;
                getvector2(array, n, indx, indy, 0, ret);
                //printf("\ncommon parent of %d, %d is %d at height %d\n", indx,indy,ret[0],ret[1]);

                cp = ret[0];
                int cph = ret[1];
                int l2 = getvector3c(array, n, indz, cph);
                //printf("%d,%d,%d,%d\n",n,cp,l2,cph);
                flag = 1;

                getvector2(array, n, l2, cp, cph, ret);
                //	printf("%d\n",ret[0]);
                //printf("\ncommon parent of nodes at height %d,index %d and %d is:%d at height %d\n",cph,cp,l2,ret[0],ret[1]);

                cp = ret[0];
                cph = ret[1];

                if (cph < ht)
                {
                    getvector1(array, n, ht, cp, cph);
                }
            }

            printf(" Enter the data at index: %d\n", indx);
            scanf("%s", &updata1);
            printf(" Enter the data at index: %d\n", indy);
            scanf("%s", &updata2);
            printf(" Enter the data at index: %d\n", indz);
            scanf("%s", &updata3);

            shafunction(updata1, strlen(updata1), hupdata1);
            shafunction(updata2, strlen(updata2), hupdata2);
            shafunction(updata3, strlen(updata3), hupdata3);

            /****************** --- Testing purpose only
  
int kind=0;

printf("ls:");
for(kind=0; kind<20;kind++)
{
            printf("%02x",hupdata1[kind]);
            }
            printf("\n");
            

printf("ms:");
for(kind=0; kind<20;kind++)
{
            printf("%02x",hupdata2[kind]);
            }
            printf("\n");
            

printf("rs:");
for(kind=0; kind<20;kind++)
{
            printf("%02x",hupdata3[kind]);
            }
            printf("\n");
            


printf ("lvsise %d,mvsize %d,rvsize %d,cpvsize %d, cprvsize %d, trvsize %d\n", lvsize, mvsize, rvsize, cpvsize, cprvsize,trvsize);



printf("\nl: %d, m %d",l,m);

************/

            if (lmrorder == 0)
            {
                // printf("\nretvalupd1:%s,\nretvalupd2:%s,\nretvalupd3:%s",retvalupd1,retvalupd2,retvalupd3);

                update1(array, indz, hupdata3, rvector, rvsize, 0, upd1ret);
                mod_update1(hupdata3, rdata, rvsize, retvalupd1);

                update2l(array, indx, hupdata1, lvector, lvsize, indy, hupdata2, mvector, mvsize, 0, upd2ret);
                mod_update2l(hupdata1, ldata, lvsize, hupdata2, mdata, mvsize, retvalupd2);

                /*******Testing purpose only ******
printf("calcing root with module functions is %s\n,%d\n  ", retvalupd2, strlen(retvalupd2));

printf("\n%d,%d\n %d,%d \nrpoint: %s\n%s\n%d\nretvalupd1: %s\n", globlmph, globlmpind, globrph,globrpind,rpoint,pointcp,lmrorder, retvalupd1);
printf("calcing root with module functions is %s,\n  ", retvalupd1);
printf("rpoint: before call");
for(kind=0; kind<20;kind++)
{
            printf("%02x",rpoint[kind]);
            }
            printf("\n");


***********************************/

                update2l(array, globlmpind, pointcp, cpvector, cpvsize, globrpind, rpoint, cprvector, cprvsize, globrph, upd2ret);

                /******************************
printf("rpoint: after call");
for(kind=0; kind<20;kind++)
{
            printf("%02x",rpoint[kind]);
            }
            printf("\n");
 ******************************/

                mod_update2l(retvalupd2, cpdata, cpvsize, retvalupd1, cprdata, cprvsize, retvalupd3);
                if (globlmph >= ht)
                {
                    memcpy(mod_root, retvalupd3, 20);
                    //printf("Final root with module functions when to root is 0 is \n%s\n%d\n ", mod_root, globlmph);
                }
            }

            else if (lmrorder == 1)
            {
                update2l(array, indy, hupdata2, lvector, lvsize, indz, hupdata3, mvector, mvsize, 0, upd2ret);
                mod_update2l(hupdata2, ldata, lvsize, hupdata3, mdata, mvsize, retvalupd2);
                //printf("calcing root with module functions is %s,\n  ", retvalupd2);
                update1(array, indx, hupdata1, rvector, rvsize, 0, upd1ret);
                mod_update1(hupdata1, rdata, rvsize, retvalupd1);

                //printf("\n%d,%d\n %d,%d \n%s\n%s\n%d\n", globlmph, globlmpind, globrph,globrpind,rpoint,pointcp,lmrorder);
                update2l(array, globrpind, rpoint, cprvector, cprvsize, globlmpind, pointcp, cpvector, cpvsize, globlmph, upd2ret);
                mod_update2l(retvalupd1, cprdata, cprvsize, retvalupd2, cpdata, cpvsize, retvalupd3);
                if (globlmph >= ht)
                {
                    memcpy(mod_root, retvalupd3, 20);
                    //printf("Final root with module functions when to root is 0 \n%s\n %d\n ", mod_root, globlmph);
                }
            }

            if (globlmph < ht)
            {
                update1(array, globlmpind, pointcp, trvector, trvsize, globlmph, upd1ret);
                mod_update1(retvalupd3, trdata, trvsize, mod_root);
                //printf("Final root calcing root with module functions when to root is >0 is \n%s  ", mod_root);
            }

            //printf("\n%d,%d\n %d,%d \n", globlmph, globlmpind, globrph,globrpind);

            //printf("the root calculated with module functions is %s,\n %s ", retvalupd1,retvalupd2);
        }
    }

    display(array, n, ht);
    printf("Final root calcing root with module : \n  ");

    for (indpr = 0; indpr < 20; indpr++)
    {
        printf("%02x", mod_root[indpr]);
    }

    printf("\n");
}

/*msb_count function takes an unsigned integer and gives the position of msb from left to right starting from 1
 this function is used to calculate the height at which two nodes have  a CP, the msb gives the height after an XOR between the indices of nodes
 *
 */
int msb_count(unsigned int num)
{
    int count = 0;
    while (num)
    {
        num = (unsigned int)num >> 1;
        count++;
    }
    return count;
}

Init(int n, N *array[], int h)
{

    int i, j, k = 1, ht = h;

    unsigned char dstring[10] = "ABAB";
    unsigned char hashinit[20];
    int ncolumns = n;
    //printf("\n strlen: %d\n ",strlen(dstring));

    shafunction(dstring, strlen(dstring), hashinit);

    for (i = 0; i <= ht; i++)
    {
        array[i] = malloc(ncolumns * sizeof(N));
        if (array[i] == NULL)
        {
            fprintf(stderr, "out of memory2\n");
            return 0;
        }
        ncolumns = ncolumns / 2;
    }
    ncolumns = n;

    i = 0;
    for (j = 0; j < ncolumns; j++)
    {
        memcpy(array[i][j].data, hashinit, 20);

        for (indpr = 0; indpr < 20; indpr++)
        {
            printf("%02x", array[i][j].data[indpr]);
        }

        printf("\n");
    }

    printf("\n");
    return 1;
}

char *buildroot(N *array[], int n, int h)
{
    int m, i, j, ncol = n / 2, ht = h, k;
    unsigned char tempstr2[40] = {0};
    char hashout[20];

    for (i = 1; i <= ht; i++)
    {

        for (j = 0, k = 0; k < ncol; j + 2, k++)
        {

            memcpy(tempstr2, array[i - 1][j].data, 20);
            memcpy(tempstr2 + 20, array[i - 1][j + 1].data, 20);

            shafunction(tempstr2, 40, hashout);

            memcpy(array[i][k].data, hashout, 20);

            for (indpr = 0; indpr < 20; indpr++)
            {
                printf("%02x", array[i][k].data[indpr]);
            }

            printf("\n");
        }
        ncol = ncol / 2;
        printf("\n");
    }

    return array[ht][0].data;
}

void getvector1(N **array, int size, int ht, int x, int ht_x)
{
    unsigned int ind1 = x, temp1, maxh = ht, ind_ht = ht_x;
    unsigned int vx[size / 2], ix = 0, vind = 0, vorder[size / 2];
    int pind;
    if (x < (size))
    {
        while (ind_ht < maxh)
        {
            if ((ind1 & 1) == 0)
            {
                vx[ix] = ind1 + 1;
                vorder[ix] = 0;
            }
            else
            {
                vx[ix] = ind1 - 1;
                vorder[ix] = 1;
            }
            ind1 = (int)(ind1 / 2);
            ind_ht++;
            ix++;
        }

        //printf("\nlisting vector indices for %d upto root: \n",x);

        int vectsize = ix;
        trvector = malloc(vectsize * sizeof(VT));

        trdata = malloc(vectsize * sizeof(VDat));
        int index_x = 0, ind_ht;
        for (vind = 0; vind < ix; vind++)
        {
            //printf("h,i,o: %d,%d,%d\n",vind+ht_x,vx[vind],vorder[vind]);
            trvector[vind].vindex = vx[vind];
            index_x = vx[vind];
            trvector[vind].order = vorder[vind];
            trdata[vind].vchar[0] = '\0';
            ind_ht = vind + ht_x;
            //printf("%d\n%s",ind_ht,array[ind_ht][index_x].data);
            memcpy(trdata[vind].vchar, array[ind_ht][index_x].data, 20);
            //printf("\n%s",trdata[vind].vchar);
            trdata[vind].order = vorder[vind];
        }

        trvsize = vectsize;
    }
    else
    {
        printf("Thers no node at %d", ind1);
    }
}

int *getvector2(N *array[], unsigned int n, unsigned int x, unsigned int y, unsigned int ht, unsigned int ret[])
{
    unsigned int ind1 = x, ind2 = y, temp1, temp2, height = ht, size = n;
    unsigned int vx[size / 2], vy[size / 2], ix = 0, iy = 0, vind = 0, vxo[size / 2], vyo[size / 2];
    if (!(x > (size - 1)) | (y > (size - 1)))
    {
        temp1 = ind1;
        temp2 = ind2;
        if (temp1 == temp2)
            return 0;
        if ((int)temp1 >> 1 == (int)temp2 >> 1)
        {
            ret[0] = (int)temp1 >> 1;
            ret[1] = height + 1;
            return ret;
        }
        while (((int)(temp1 >> 1)) != ((int)(temp2 >> 1)))
        {

            if ((temp1 & 1) == 0)
            {
                vx[ix] = temp1 + 1;
                vxo[ix] = 0;
            }
            else
            {
                vx[ix] = temp1 - 1;
                vxo[ix] = 1;
            }

            if ((temp2 & 1) == 0)
            {
                vy[iy] = temp2 + 1;
                vyo[iy] = 0;
            }
            else
            {
                vy[iy] = temp2 - 1;
                vyo[iy] = 1;
            }
            temp1 = (int)(temp1 / 2);
            temp2 = (int)(temp2 / 2);
            height++;
            ix++;
            iy++;
        }

        int lvectsize = ix;
        int mvectsize = iy;
        int index_x = 0, index_y = 0;
        if (flag == 0)
        {
            lvector = malloc(lvectsize * sizeof(VT));
            mvector = malloc(mvectsize * sizeof(VT));
            ldata = malloc(lvectsize * sizeof(VDat));
            mdata = malloc(mvectsize * sizeof(VDat));

            //printf("\nfor x  %d: \n",x);
            for (vind = 0; vind < ix; vind++)
            {
                //printf("h,i,o: %d,%d, %d\n",vind+ht,vx[vind],vxo[vind]);
                lvector[vind].vindex = vx[vind];
                lvector[vind].order = vxo[vind];
                index_x = vx[vind];
                ldata[vind].vchar[0] = '\0';
                memcpy(ldata[vind].vchar, array[vind + ht][index_x].data, 20);
                ldata[vind].order = vxo[vind];
                //printf("%s",ldata[vind].vchar);
            }

            lvsize = lvectsize;

            //printf("\nfor y  %d: \n",y);

            for (vind = 0; vind < iy; vind++)
            {
                // printf("h,i,o: %d,%d, %d\n",vind+ht,vy[vind],vyo[vind]);
                mvector[vind].vindex = vy[vind];
                mvector[vind].order = vyo[vind];
                index_y = vy[vind];
                mdata[vind].vchar[0] = '\0';
                memcpy(mdata[vind].vchar, array[vind + ht][index_y].data, 20);
                mdata[vind].order = vyo[vind];
                //printf("%s",mdata[vind].vchar);
            }

            mvsize = mvectsize;
        }

        else
        {
            // printf("executing");

            cpvector = malloc(lvectsize * sizeof(VT));
            cprvector = malloc(mvectsize * sizeof(VT));
            cpdata = malloc(lvectsize * sizeof(VDat));
            cprdata = malloc(mvectsize * sizeof(VDat));

            //printf("\nfor x  %d: \n",x);
            for (vind = 0; vind < ix; vind++)
            {
                cprvector[vind].vindex = vx[vind];
                cprvector[vind].order = vxo[vind];

                index_x = vx[vind];

                cprdata[vind].vchar[0] = '\0';
                memcpy(cprdata[vind].vchar, array[vind + ht][index_x].data, 20);
                cprdata[vind].order = vxo[vind];
                //printf("h,i,o: %d,%d, %d\n",vind+ht,cpvector[vind].vindex,cpvector[vind].order);

                //printf("%s",cprdata[vind].vchar);
            }

            cprvsize = lvectsize;
            //printf("cpvsize %d",cpvsize);
            //printf("\nfor y  %d: \n",y);

            for (vind = 0; vind < iy; vind++)
            {
                // printf("h,i,o: %d,%d, %d\n",vind+ht,vy[vind],vyo[vind]);
                cpvector[vind].vindex = vy[vind];
                cpvector[vind].order = vyo[vind];

                index_y = vy[vind];
                cpdata[vind].vchar[0] = '\0';

                memcpy(cpdata[vind].vchar, array[vind + ht][index_y].data, 20);

                cpdata[vind].order = vyo[vind];
                //printf("%s\n",cpdata[vind].vchar);
            }

            cpvsize = mvectsize;
        }
    }
    ret[0] = (int)(temp1 / 2);
    ret[1] = height + 1;

    return ret;
}

int getvector3c(N *array[], int n, int x, int cph)
{
    int ind1 = x, temp1, height = 0, cht, size;
    cht = cph;
    size = n;
    int vx[size / 2], ix = 0, vind = 0, vxo[size / 2], arret[2];

    while (height < cph)
    {
        if ((ind1 & 1) == 0)
        {
            vx[ix] = ind1 + 1;
            vxo[ix] = 0;
        }
        else
        {
            vx[ix] = ind1 - 1;
            vxo[ix] = 1;
        }
        ind1 = (int)(ind1 / 2);
        height++;
        ix++;
    }

    //printf("listing vector indices for %d: \n",x);

    int rvectsize = ix;
    rvector = malloc(rvectsize * sizeof(VT));
    rdata = malloc(rvectsize * sizeof(VDat));

    int index_x;

    for (vind = 0; vind < ix; vind++)
    {

        rvector[vind].vindex = vx[vind];
        rvector[vind].order = vxo[vind];
        index_x = vx[vind];
        //printf("rdata size:%d, h,i,o: %d,%d,%d\n",(int)(sizeof(rdata)/sizeof(VT)),vind,rvector[vind].vindex,rvector[vind].order);
        rdata[vind].vchar[0] = '\0';
        memcpy(rdata[vind].vchar, array[vind][index_x].data, 20);
        rdata[vind].order = vxo[vind];
        //printf("%s\n",rdata[vind].vchar);
    }
    rvsize = rvectsize;
    return ind1;
}

update1(N *array[], int l, char *ls, VT I_l[], int indl, int levheight1, unsigned char *upd1ret) //indl is size of vecor
{
    unsigned char temp_l[20], tempstr2[40] = {0};
    int i = 0, u_index, l_index, m;

    memcpy(array[levheight1][l].data, ls, 20);
    memcpy(temp_l, array[levheight1][l].data, 20);

    while (i < indl)
    {
        u_index = I_l[i].vindex / 2;
        l_index = I_l[i].vindex;

        tempstr2[0] = '\0';

        if (I_l[i].order == 1)
        {
            memcpy(tempstr2, array[levheight1][l_index].data, 20);
            memcpy(tempstr2 + 20, temp_l, 20);

            //printf("tempstr:%d,",strlen(tempstr));
            shafunction(tempstr2, 40, upd1ret);
            memcpy(array[levheight1 + 1][u_index].data, upd1ret, 20);
            array[levheight1 + 1][u_index].data[20] = '\0';
        }
        else if (I_l[i].order == 0)
        {
            memcpy(tempstr2, temp_l, 20);
            memcpy(tempstr2 + 20, array[levheight1][l_index].data, 20);

            shafunction(tempstr2, 40, upd1ret);
            memcpy(array[levheight1 + 1][u_index].data, upd1ret, 20);
            array[levheight1 + 1][u_index].data[20] = '\0';
        }
        memcpy(temp_l, array[levheight1 + 1][u_index].data, 20);
        i++;
        levheight1++;
    }

    memcpy(rpoint, array[levheight1][u_index].data, 20);

    globrph = levheight1;
    globrpind = u_index;
}

void display(N *array[], int size, int ht)

{

    int i, j;

    for (i = 0; i <= ht; i++)

    {
        for (j = 0; j < size; j++)
        {

            for (indpr = 0; indpr < 20; indpr++)
            {
                printf("%02x", array[i][j].data[indpr]);
            }

            printf("\n");
        }
        size = size / 2;
        printf("\n");
    }
}

void update2l(N *array[], int l, char *ls, VT I_l[], int ilsize, int m, char *ms, VT I_m[], int imsize, int levheight, unsigned char *upd2ret)

{
    unsigned char temp_l[20], temp_m[20], *temp_cplm, tempstr2[40] = {0};
    int i = levheight, j = levheight, aind = 0, u_index, l_index, p;
    //
    int kind;
    memcpy(array[levheight][l].data, ls, 20);
    memcpy(array[levheight][m].data, ms, 20);

    /***********
     printf("  In functionls:");
for(kind=0; kind<20;kind++)
{
            printf("%02x",array[levheight][l].data[kind]);
            }
            printf("\n");
            
 printf("In function ls:");
for(kind=0; kind<20;kind++)
{
            printf("%02x",array[levheight][m].data[kind]);
            }
            printf("\n");
************/

    memcpy(temp_l, array[levheight][l].data, 20);
    memcpy(temp_m, array[levheight][m].data, 20);

    if (ilsize >= 1)
    {
        while (aind < ilsize)
        {
            u_index = I_l[aind].vindex / 2;
            l_index = I_l[aind].vindex;

            tempstr2[0] = '\0';

            if (I_l[aind].order == 1)
            {
                memcpy(tempstr2, array[i][l_index].data, 20);
                memcpy(tempstr2 + 20, temp_l, 20);

                shafunction(tempstr2, 40, upd2ret);
                memcpy(array[i + 1][u_index].data, upd2ret, 20);
                array[i + 1][u_index].data[20] = '\0';
            }
            else if (I_l[aind].order == 0)

            {
                memcpy(tempstr2, temp_l, 20);
                memcpy(tempstr2 + 20, array[i][l_index].data, 20);

                shafunction(tempstr2, 40, upd2ret);
                memcpy(array[i + 1][u_index].data, upd2ret, 20);
                array[i + 1][u_index].data[20] = '\0';
            }
            memcpy(temp_l, array[i + 1][u_index].data, 20);
            i++;
            aind++;
        }
        aind = 0;

        while (aind < imsize)
        {
            u_index = I_m[aind].vindex / 2;
            l_index = I_m[aind].vindex;
            tempstr2[0] = '\0';

            if (I_m[aind].order == 1)
            {
                memcpy(tempstr2, array[j][l_index].data, 20);
                memcpy(tempstr2 + 20, temp_m, 20);

                shafunction(tempstr2, 40, upd2ret);
                memcpy(array[j + 1][u_index].data, upd2ret, 20);
                array[j + 1][u_index].data[20] = '\0';
            }
            else if (I_m[aind].order == 0)

            {
                memcpy(tempstr2, temp_m, 20);
                memcpy(tempstr2 + 20, array[j][l_index].data, 20);

                shafunction(tempstr2, 40, upd2ret);
                memcpy(array[j + 1][u_index].data, upd2ret, 20);
                array[j + 1][u_index].data[20] = '\0';
            }
            memcpy(temp_m, array[j + 1][u_index].data, 20);
            j++;
            aind++;
        }

        tempstr2[0] = '\0';

        //printf("\n temp_left");

        memcpy(tempstr2, temp_l, 20);
        memcpy(tempstr2 + 20, temp_m, 20);

        shafunction(tempstr2, 40, upd2ret);
        u_index = u_index / 2;
        memcpy(array[j + 1][u_index].data, upd2ret, 20);
        array[j + 1][u_index].data[20] = '\0';
        //temp_cplm = array[j+1][u_index].data;
        memcpy(pointcp, array[j + 1][u_index].data, 20);

        globlmph = j + 1;
        globlmpind = u_index;
    }
    else
    {
        tempstr2[0] = '\0';

        memcpy(tempstr2, array[levheight][l].data, 20);
        memcpy(tempstr2 + 20, array[levheight][m].data, 20);

        shafunction(tempstr2, 40, upd2ret);

        memcpy(array[levheight + 1][l / 2].data, upd2ret, 20);
        array[levheight + 1][l / 2].data[20] = '\0';
        globlmph = levheight + 1;
        globlmpind = l / 2;

        memcpy(pointcp, array[levheight + 1][l / 2].data, 20);
    }
}
