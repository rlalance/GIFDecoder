
#ifndef _SYSM_
#include"sysm.h"
#endif




void * create_memory(unsigned int size)
{
	return((void*)malloc(size));
}