#include "loi.h"


int loi(int a, int b)
{
	int a1, b1, a2, b2;
	a1= a/5;
	a2= a%5;
	b1= b/5;
	b2= b%5;
	a1 = (a1 + b1)%5;
	a2 = (a2 + b2)%5;
	return (a1*5) + a2;
}


int group_sub(int a,int b)
{
    int a1,a2;
    int b1,b2;
    a1= a/5;
    a2= a%5;
    b1= b/5;
    b2= b%5;
    a1=(a1+5-b1)%5;
    a2 = (a2 + 5-b2)%5;
    return (a1*5) + a2;

}

int ordre = 25;

int exposant = 5;
