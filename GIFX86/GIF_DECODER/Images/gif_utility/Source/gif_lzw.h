#ifndef _GIFLZW_
#define _GIFLZW_
#endif


typedef struct StringTable{
	unsigned int Index;
	unsigned int Prefix;
	Byte SuffixByte[1];
	unsigned long int padding[4];
    Byte FirstByte[1];
	/***********reserved to prevent memory overlap*********/

}StringTable;








