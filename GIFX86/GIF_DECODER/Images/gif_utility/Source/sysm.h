
#ifndef _SYSM_
#define _SYSM_
#endif





#include "stdio.h"
#include"windows.h"
#include "math.h"
#include "stdlib.h"
#include "new.h"
#include "Winbase.h"
/***********Data Structure Definitions************/

#define Byte unsigned char

/*********Names For the Lables used to Identify Various Blocks*****/

#define ApplicationExtension 0xFF
#define CommentExtension     0xFE
#define GraphicControlExtension 0xF9
#define ImageBlockEND NULL
#define GIF_Trailer 0x3B
#define ImageDescriptorIdentifier  0x2c
#define gTypeImageBlock 0x2c
#define gTypeTerminator 0x3b
#define gTypeExtension 0x21


/**************Prototypes**************************/
void * create_memory(unsigned int size);
