#include "loi.h"


int loi(int a, int b)
{
	return a^b;
	/*
	int a0,a1,a2,a3,a4,b0,b1,b2,b3,b4;
    a0=a%2;
    a1=(a/2)%2; // Pour éléminer la composante a0 on dévise a par 2 (la dévision carrespond au modulo précédent)
    a2=(a/4)%2; // On dévise par 8 car 2*4=8 on dévise par tout ceux d'avant par 2 et par 4 donc par 8
	a3=(a/8)%2;
    a4=(a/16);
    b0=b%2;
    b1=(b/2)%2;
    b2=(b/4)%2;
	b3=(b/8)%2;
    b4=(b/16);
    // calculer les trois composantes de la somme 
    a0=(a0^b0);
    a1=(a1^b1);
    a2=(a2^b2);
	a3=(a3^b3);
    a4=(a4^b4);
    return a0+2*a1+4*a2+8*a3+16*a4;
    */
}

int group_sub(int a, int b)
{
	return a^b;
/*
	int a0,a1,a2,a3,a4,b0,b1,b2,b3,b4;
    a0=a%2;
    a1=(a/2)%2; // Pour éléminer la composante a0 on dévise a par 2 (la dévision carrespond au modulo précédent)
    a2=(a/4)%2; // On dévise par 8 car 2*4=8 on dévise par tout ceux d'avant par 2 et par 4 donc par 8
	a3=(a/8)%2;
    a4=(a/16);
    b0=b%2;
    b1=(b/2)%2;
    b2=(b/4)%2;
	b3=(b/8)%2;
    b4=(b/16);
    // calculer les trois composantes de la somme 
    a0=(a0^b0);
    a1=(a1^b1);
    a2=(a2^b2);
	a3=(a3^b3);
    a4=(a4^b4);
    return a0+2*a1+4*a2+8*a3+16*a4;
  */
}

int ordre = 32;

int exposant = 2; 


