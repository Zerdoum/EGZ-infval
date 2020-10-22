#include "loi.h"


int loi(int a, int b)
{
    int a0,a1,a2,b0,b1,b2;

    a0=a%3;
    a1=(a/3)%3;
    a2=a/9;

    b0=b%3;
    b1=(b/3)%3;
    b2=b/9;
    // calculer les trois composantes de la somme 
    a0=(a0+b0)%3;
    a1=(a1+b1)%3;
    a2=(a2+b2)%6;

    return a0+3*a1+9*a2;
}



int group_sub(int a,int b)
{
 
    int a0,a1,a2,b0,b1,b2;

    a0=a%3;
    a1=(a/3)%3;
    a2=a/9;

    b0=b%3;
    b1=(b/3)%3;
    b2=b/9;

    a0=(a0+3-b0)%3;
    a1=(a1+3-b1)%3;
    a2=(a2+6-b2)%6;

    return a0+3*a1+9*a2;
}

int ordre = 54;

int exposant = 6;
