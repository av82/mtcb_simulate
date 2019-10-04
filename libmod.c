struct vectdata
{

    unsigned char vchar[20];

    unsigned int order;
};

typedef struct vectdata VDat;

unsigned char *mod_update2l(unsigned char *ls, VDat I_l[], int ilsize, unsigned char *ms, VDat I_m[], int imsize, unsigned char *modret2)

{
    unsigned char temp_left[20] = {0}, temp_right[20] = {0}, rethash[20] = {0}, tempstore[40] = {0};

    int aind = 0, m, kind;

    memcpy(temp_left, ls, 20);
    memcpy(temp_right, ms, 20);

    if (ilsize >= 1)
    {
        while (aind < ilsize)
        {

            tempstore[0] = '\0';
            if (I_l[aind].order == 1)
            {
                memcpy(tempstore, I_l[aind].vchar, 20);

                memcpy(tempstore + 20, temp_left, 20);

                shafunction(tempstore, 40, rethash);
                memcpy(temp_left, rethash, 20);
            }
            else if (I_l[aind].order == 0)

            {
                memcpy(tempstore, temp_left, 20);

                memcpy(tempstore + 20, I_l[aind].vchar, 20);

                shafunction(tempstore, 40, rethash);
                memcpy(temp_left, rethash, 20);
            }
            aind++;
        }
        aind = 0;
        memcpy(temp_right, ms, 20);
        while (aind < imsize)
        {

            tempstore[0] = '\0';

            if (I_m[aind].order == 1)
            {

                memcpy(tempstore, I_m[aind].vchar, 20);

                memcpy(tempstore + 20, temp_right, 20);

                shafunction(tempstore, 40, rethash);
                memcpy(temp_right, rethash, 20);
            }
            else if (I_m[aind].order == 0)

            {
                memcpy(tempstore, temp_right, 20);

                memcpy(tempstore + 20, I_m[aind].vchar, 20);

                shafunction(tempstore, 40, rethash);
                memcpy(temp_right, rethash, 20);
            }
            aind++;
        }

        memcpy(tempstore, temp_left, 20);
        memcpy(tempstore + 20, temp_right, 20);

        shafunction(tempstore, 40, rethash);
    }
    else
    {

        memcpy(tempstore, temp_left, 20);
        memcpy(tempstore + 20, temp_right, 20);

        shafunction(tempstore, 40, rethash);
    }

    memcpy(modret2, rethash, 20);
}

unsigned char *mod_update1(unsigned char *ls, VDat I_l[], int indl, unsigned char *modret1) //indl is size of vecor
{
    unsigned char temp_l[20] = {0}, rethash1[20] = {0}, tempstore[40] = {0};
    int i = 0, m;

    memcpy(temp_l, ls, 20);

    while (i < indl)
    {

        if (I_l[i].order == 1)
        {
            memcpy(tempstore, I_l[i].vchar, 20);

            memcpy(tempstore + 20, temp_l, 20);

            shafunction(tempstore, 40, rethash1);
            memcpy(temp_l, rethash1, 20);
        }
        else if (I_l[i].order == 0)

        {
            memcpy(tempstore, temp_l, 20);

            memcpy(tempstore + 20, I_l[i].vchar, 20);

            shafunction(tempstore, 40, rethash1);

            memcpy(temp_l, rethash1, 20);
        }

        i++;
    }

    memcpy(modret1, temp_l, 20);
}
