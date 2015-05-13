/*********LZW_Decompression Algorithm************************/


#ifndef _SYSM_
#include"sysm.h"
#endif

#ifndef _GIFLZW_
#include"gif_lzw.h"
#endif

#ifndef _READ_
#include"gif_read.h"
#endif

static long CodeBitOffset;
static unsigned int CodeMask;
static unsigned int index=0;
static unsigned int CODE_SIZE;
StringTable *Dictionary=NULL;
int *SuffixStack=NULL;
Byte* PackBytes=NULL;
int SuffixStackIndex=0;









char *myitoa(int i, char *s)
{
char *p = s;
char *q = s;

if (i >= 0)
{
do
{
*q++ = '0' + (i % 10);
}
while (i /= 10);
}
else if (-1 % 2 < 0)
{
*q++ = '-';
p++;

do
{
*q++ = '0' - i % 10;
}
while (i /= 10);
}
else
{
*q++ = '-';
p++;

do
{
int d = i % 10;
i = i / 10;
if (d) { i++; d = 10 - d; }
*q++ = '0' + d;
}
while (i);
}

for (*q = 0; p < --q; p++)
{
char c = *p;
*p = *q;
*q = c;
}

return s;
}



void Output_Code(unsigned int DictIndex,unsigned int LZW_ENDCODE)
{
	while(1)
	{
		if((Dictionary+DictIndex)->Prefix<LZW_ENDCODE)
		{
			PackBytes[index++]=(Dictionary+DictIndex)->Prefix;
			if(*((Dictionary+DictIndex)->SuffixByte)!='\0')
				PackBytes[index++] =(Byte)atoi((Dictionary+DictIndex)->SuffixByte); 
			break;
			
		}
		else
		{	
			SuffixStackIndex++;
			SuffixStack	=realloc(SuffixStack,(SuffixStackIndex)*sizeof(int));
			
			SuffixStack[SuffixStackIndex-1]=atoi((Dictionary+DictIndex)->SuffixByte); 
			DictIndex=(Dictionary+DictIndex)->Prefix;
			
		}
		
}	}


unsigned int ReadCode(Read_Data first_code)
{
	
	long LongCode=0;
	unsigned int tmpCode=0;
	
	LongCode=*((long*)(first_code.Data+CodeBitOffset/8));	// Get some bytes from bufIn
	LongCode>>=(CodeBitOffset&7);				// Discard too low bits
	tmpCode =(LongCode & ((1<<CODE_SIZE)-1) );	// Discard too high bits
	CodeBitOffset += CODE_SIZE;					// Increase Bit Offset
	return tmpCode;
	
}

void InitializeDictionary(unsigned int LZW_ENDCODE,StringTable *Dictionary)
{
	volatile unsigned int temp=0;
	long Dict_Index=0;
	while(temp<=LZW_ENDCODE)
	{
		(Dictionary+Dict_Index)->Index=temp;
		(Dictionary+Dict_Index)->Prefix=temp;
		myitoa(((Dictionary+Dict_Index)->Prefix)&255,(Dictionary+Dict_Index)->FirstByte);
		temp++;
		Dict_Index++;
	}
	
	
}


Byte * Lzw_Decode(Read_Data RawData,ImageDesc_Info Image_Desc)
{
	unsigned int code=0;
	unsigned int OldCode=0;
	unsigned int LZW_ClearCode=0;
	unsigned int LZW_ENDCODE=0;
	unsigned int END_StringTable=0;
	
	index=0;
	
	/***************PackBytes used for output******************/
	PackBytes=(Byte*)create_memory(Image_Desc.ImageHeight*Image_Desc.ImageWidth*3);
	memset(PackBytes,0,Image_Desc.ImageHeight*Image_Desc.ImageWidth*3);
	Dictionary=(StringTable *)create_memory(sizeof(StringTable)*4096);
	memset(Dictionary,0,sizeof(StringTable)*4096);
	
	LZW_ClearCode=1<<RawData.LZW_CODESIZE;
    LZW_ENDCODE=LZW_ClearCode+1;
	END_StringTable=LZW_ENDCODE;
	END_StringTable++;
	CODE_SIZE=RawData.LZW_CODESIZE+1;
	CodeMask=(1<<CODE_SIZE)-1;
	CodeBitOffset=0;
	InitializeDictionary(LZW_ENDCODE,Dictionary);
	code=ReadCode(RawData);
	if(code!=LZW_ClearCode)
		perror("Invalid Data");
	OldCode=code=ReadCode(RawData);
	PackBytes[index]=(Byte)code;
	index++;
	do
	{
		code=ReadCode(RawData);
	
		if(code==LZW_ENDCODE)
			break;
		if(code==LZW_ClearCode)
		{
			CODE_SIZE=RawData.LZW_CODESIZE+1;
			CodeMask=(1<<CODE_SIZE)-1;
			memset(Dictionary,0,sizeof(StringTable)*4096);
			InitializeDictionary(LZW_ENDCODE,Dictionary);
			END_StringTable=LZW_ENDCODE;
			END_StringTable++;
			OldCode=code=ReadCode(RawData);
			PackBytes[index]=(Byte)code;
			index++;
		}
		else
		{
			if(code<END_StringTable)
			{
				(Dictionary+END_StringTable)->Index=END_StringTable;
				(Dictionary+END_StringTable)->Prefix=OldCode;
				strcpy((Dictionary+END_StringTable)->SuffixByte,(Dictionary+code)->FirstByte);
				strcpy((Dictionary+END_StringTable)->FirstByte,(Dictionary+OldCode)->FirstByte);
				END_StringTable++;
				Output_Code(code,LZW_ENDCODE);
				while(SuffixStackIndex>0)
				{
					SuffixStackIndex--;
					PackBytes[index++]=SuffixStack[SuffixStackIndex];
					
				}
				SuffixStackIndex=0;
			}
			else
			{
				(Dictionary+END_StringTable)->Index=END_StringTable;
				(Dictionary+END_StringTable)->Prefix=OldCode;
				strcpy((Dictionary+END_StringTable)->SuffixByte,(Dictionary+OldCode)->FirstByte);
				strcpy((Dictionary+END_StringTable)->FirstByte,(Dictionary+OldCode)->FirstByte);
				Output_Code(END_StringTable,LZW_ENDCODE);
				while(SuffixStackIndex>0)
				{
				
					SuffixStackIndex--;
					PackBytes[index++]=SuffixStack[SuffixStackIndex];
						
					
				}	
				SuffixStack=realloc(SuffixStack,sizeof(Byte));
				memset(SuffixStack,0,sizeof(Byte));
				SuffixStackIndex=0;
				END_StringTable++;
				
			}
			
			OldCode=code;
		}
		if(END_StringTable>CodeMask)
		{
			
			if(CODE_SIZE<12)
			{
				CODE_SIZE++;
				CodeMask = (1 << CODE_SIZE) - 1;
			}
			
		}
		
		
	}while(code!=LZW_ENDCODE); 
	free(SuffixStack);
	SuffixStack=NULL;
	memset(RawData.Data,0,RawData.Blocksize);
	free(RawData.Data);
    RawData.Data=NULL;
	memset(Dictionary,0,sizeof(StringTable)*4096);
	free(Dictionary);
	Dictionary=NULL;
	
	

	return(PackBytes);
}


