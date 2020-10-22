#include "loi.h"


int loi(int a, int b)
{
	return ((a^b)&7) + (((a&0xf8)+(b&0xf8))%48);
}

int group_sub(int a, int b)
{
	return ((a^b)&7) + (((a&0xf8)+48-(b&0xf8))%48);

}


int ordre = 48;

int exposant = 6; 


