#include "loi.h"


int loi(int a, int b)
{
	int a0,a1,a2,b0,b1,b2;
    a0=a%2;
    a1=(a/2)%4; // Pour éléminer la composante a0 on dévise a par 2 (la dévision carrespond au modulo précédent)
    a2=a/8; // On dévise par 8 car 2*4=8 on dévise par tout ceux d'avant par 2 et par 4 donc par 8

    b0=b%2;
    b1=(b/2)%4;
    b2=b/8;
    // calculer les trois composantes de la somme 
    a0=(a0+b0)%2;
    a1=(a1+b1)%4;
    a2=(a2+b2)%8;

    return a0+2*a1+8*a2;
}

int group_sub(int a, int b)
{
	int a0,a1,a2,b0,b1,b2;
    a0=a%2;
    a1=(a/2)%4;
    a2=a/8;

    b0=b%2;
    b1=(b/2)%4;
    b2=b/8;
    // calculer les trois composantes de la somme 
    a0=(a0+2-b0)%2;
    a1=(a1+4-b1)%4;
    a2=(a2+8-b2)%8;

    return a0+2*a1+8*a2;
}

int ordre = 64;

int exposant = 8; 


