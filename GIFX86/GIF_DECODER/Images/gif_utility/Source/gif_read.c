
#ifndef _SYSM_
#include"sysm.h"
#endif

#ifndef _READ_
#include"gif_read.h"
#endif


void Display(FrameData *FrameInfo);
FILE *GIF_FILE;

Byte * Lzw_Decode(Read_Data RawData,ImageDesc_Info Image_Desc);





Read_Data  Read_Data_Block()
{
	unsigned int index=0;
	unsigned int Data_Block=0;
	unsigned int Total_Data_Block=0;
    Read_Data Raw_Data;
	unsigned int BlockSize=0;
	
	Byte *Data;
	memset(&Raw_Data,0,sizeof(Read_Data));
	fread(&Raw_Data.LZW_CODESIZE,1,1,GIF_FILE);
	fread(&Data_Block,1,1,GIF_FILE);
	Data=(Byte*)create_memory(Data_Block);
	Total_Data_Block=Data_Block;
	memset(Data,0,Total_Data_Block);
	while(Data_Block!=(unsigned int)ImageBlockEND)
	{
		while(index<Total_Data_Block)
		{
			fread(&Data[index],1,1,GIF_FILE);
			index++;
		}
		fread(&Data_Block,1,1,GIF_FILE);
		Total_Data_Block=Data_Block+Total_Data_Block;
		Data=realloc(Data,Total_Data_Block);
	}
	Raw_Data.Blocksize=Total_Data_Block;
	Raw_Data.Data=Data;
	return(Raw_Data);
	
}


int GIF_IDENTIFY(const char*Sign_Validation)
{
	return(strncmp(Sign_Validation,"GIF",3));
}



Header Image_Header_Read(Header Image_Header)
{
	
    fread(&Image_Header.signature[0],1,1,GIF_FILE);
	fread(&Image_Header.signature[1],1,1,GIF_FILE);
	fread(&Image_Header.signature[2],1,1,GIF_FILE);
    
	if(GIF_IDENTIFY(Image_Header.signature))
		
		MessageBox(NULL, "Image format is inaccurate ", "Error",
		MB_ICONEXCLAMATION | MB_OK);
	
	fread(&Image_Header.version[0],1,1,GIF_FILE);
	fread(&Image_Header.version[1],1,1,GIF_FILE);
	fread(&Image_Header.version[2],1,1,GIF_FILE);
	
	return(Image_Header);
}



Screen_Desp_Info Screen_Descrp_Read(Screen_Desp_Info Screen_Desp)
{
	fread(&Screen_Desp.logicalScreenWidth,1,sizeof(short int),GIF_FILE);
	
	
		  
	fread(&Screen_Desp.logicalScreenHeight,1,sizeof(short int),GIF_FILE);
	
	
	fread(&Screen_Desp.bitField,1,1,GIF_FILE);
	
	fread(&Screen_Desp.backgroundColor,1,1,GIF_FILE);
	
	fread(&Screen_Desp.pixelAspectRatio,1,1,GIF_FILE);
	
	return(Screen_Desp);
}




Bit_Field_Info BitField_Identify(Bit_Field_Info BitField,Screen_Desp_Info Screen_Desp)
{
	
	BitField.Global_Clr_Flag=Screen_Desp.bitField&0x80;
    BitField.Global_Clr_Flag=BitField.Global_Clr_Flag>>7;
	
	BitField.Color_Res[0]=(Screen_Desp.bitField&0x40)<<1;
    BitField.Color_Res[0]=BitField.Color_Res[0]>>7;
	
	BitField.Color_Res[1]=(Screen_Desp.bitField&0x20)<<2;
	BitField.Color_Res[1]=BitField.Color_Res[1]>>7;
	
	BitField.Color_Res[2]=(Screen_Desp.bitField&0x10)<<3;
	BitField.Color_Res[2]=BitField.Color_Res[2]>>7;
	
	BitField.Sort_Flag=(Screen_Desp.bitField&0x8)<<4;
	BitField.Sort_Flag=BitField.Sort_Flag>>7;
	
    BitField.Siz_GlbTable=(4*((Screen_Desp.bitField&0x4)<<5)>>7)+(2*((Screen_Desp.bitField&0x2)<<6)>>7)+(((Screen_Desp.bitField&1)<<7)>>7);
	
    BitField.ByteSiz_GlbTable=3*(int)(pow(2,(BitField.Siz_GlbTable+1)));
	
	return(BitField);
}

Grap_Rendblk Read_GraphicRenderBlk(Grap_Rendblk Graph_Renderblk)
{
	/* Initialise Render Block ********/
	
	memset(&Graph_Renderblk,0,sizeof(Grap_Rendblk));
	fread(&Graph_Renderblk.Block_Size,1,1,GIF_FILE);
	fread(&Graph_Renderblk.Packed_Field,1,1,GIF_FILE);
	fread(&Graph_Renderblk.Delay_Time,1,sizeof(Graph_Renderblk.Delay_Time),GIF_FILE);
	fread(&Graph_Renderblk.Transp_Clr_Index,1,sizeof(Graph_Renderblk.Transp_Clr_Index),GIF_FILE);
	fread(&Graph_Renderblk.BlockTerminator,1,sizeof(Graph_Renderblk.BlockTerminator),GIF_FILE);
	Graph_Renderblk.Reserved=0;
	Graph_Renderblk.DisposalMethod=0;
	Graph_Renderblk.UserInput=0;
	Graph_Renderblk.Tranparent_Clr_Flag=0;
	Graph_Renderblk.Reserved=(Graph_Renderblk.Packed_Field&0xE0)>>5;
	Graph_Renderblk.DisposalMethod=(Graph_Renderblk.Packed_Field&0x1c)>>2;
	Graph_Renderblk.UserInput=(Graph_Renderblk.Packed_Field&0x02)>>1;
	Graph_Renderblk.Tranparent_Clr_Flag=Graph_Renderblk.Packed_Field&0x01;
	
	
	
	return(Graph_Renderblk);
}

ImageDesc_Info Read_ImageDescriptor(ImageDesc_Info Image_Desc)
{
	
	fread(&Image_Desc.LeftPosition,1,sizeof(Image_Desc.LeftPosition),GIF_FILE);
	fread(&Image_Desc.TopPosition,1,sizeof(Image_Desc.TopPosition),GIF_FILE);
	fread(&Image_Desc.ImageWidth,1,sizeof(Image_Desc.ImageWidth),GIF_FILE);
	fread(&Image_Desc.ImageHeight,1,sizeof(Image_Desc.ImageHeight),GIF_FILE);
	fread(&Image_Desc.PackedField,1,sizeof(Image_Desc.PackedField),GIF_FILE);
	Image_Desc.local_Clr_Flag=(Image_Desc.PackedField&0x80)>>7;
	Image_Desc.Interlace_Flag=(Image_Desc.PackedField&0x40)>>6;
	Image_Desc.Sort_Flag=(Image_Desc.PackedField&0x20)>>5;
	Image_Desc.reserved_bits=(Image_Desc.PackedField&0x18)>>3;
	Image_Desc.local_clr_Size=(Image_Desc.PackedField&0x7);
	return(Image_Desc);
}



void Gif_Read()
{
	
		Byte *Data;/*temp*/
	Read_Data  RawData;
	

	
	Header Image_Header;
	
	
	
	Screen_Desp_Info Screen_Desp;
	
	
	Bit_Field_Info BitField;
	
	
	ImageDesc_Info Image_Desc;
	
	FrameData *FrameInfo;

	FrameData * FrameStarter;
	
	
	GlobalTable_RGB_Color *CLMAP=NULL;

	GlobalTable_RGB_Color *LocalTable=NULL;
	
	
	Grap_Rendblk Graph_Renderblk;
	
	Byte Data_Block;
	
	
	memset(&Image_Desc,0,sizeof(ImageDesc_Info));
	
	memset(&RawData,0,sizeof(Read_Data));
	
	/*Initialise Image_Header with zero */
	memset(&Image_Header,0,sizeof(Image_Header));
	
	memset(&BitField,0,sizeof(Bit_Field_Info));
	
	/* Initialise Screen_Desp with zero */
	
	memset(&Screen_Desp,0,sizeof(Screen_Desp));
	
	FrameInfo=(FrameData*)create_memory(sizeof(FrameData));
	memset(FrameInfo,0,sizeof(FrameData));
	
	FrameStarter=FrameInfo;
	
	
	GIF_FILE=fopen("./SnowFall.gif","rb");
	
	/***********FOR WINDOWS ONLY**************/
	
	if(GIF_FILE==NULL)
		perror("Image File Not Found");
		
		
	
	/** Read Image_Header **/
	
	Image_Header = Image_Header_Read(Image_Header);
	
	
	
	Screen_Desp = Screen_Descrp_Read(Screen_Desp);
	
	
	
	BitField = BitField_Identify(BitField,Screen_Desp);
	
	if(BitField.Global_Clr_Flag)
	{
		unsigned int INDEX=0;
		
		CLMAP=(GlobalTable_RGB_Color*)create_memory(sizeof(GlobalTable_RGB_Color)*(BitField.ByteSiz_GlbTable)/3);
		
		/* Initialising Global Table */

		memset(CLMAP,0,sizeof(GlobalTable_RGB_Color)*(BitField.ByteSiz_GlbTable)/3);
		
		
		BitField.ByteSiz_GlbTable=BitField.ByteSiz_GlbTable/3;
		
		/* Reading Glb_ColorTable ****/
		while(BitField.ByteSiz_GlbTable)
		{
			
			fread(&((CLMAP+INDEX)->Red),1,1,GIF_FILE);
			fread(&((CLMAP+INDEX)->Green),1,1,GIF_FILE);
			fread(&((CLMAP+INDEX)->Blue),1,1,GIF_FILE);
			INDEX++;
			BitField.ByteSiz_GlbTable--;
			
		}
		
	}		  
	/*****Read Extension **************/
	fread(&Data_Block,1,1,GIF_FILE);
	
	fread(&Data_Block,1,1,GIF_FILE);
	while(Data_Block!=GIF_Trailer)
	{
		
		switch(Data_Block)
			
		{
			
		case GraphicControlExtension:
			Graph_Renderblk=Read_GraphicRenderBlk(Graph_Renderblk);
			fread(&Data_Block,1,1,GIF_FILE);
			break;
			
		case ImageDescriptorIdentifier:
		
			
			Image_Desc=Read_ImageDescriptor(Image_Desc);
			if(Image_Desc.local_Clr_Flag)
			{
				unsigned int LOCALINDEX=0;
		
				LocalTable=(GlobalTable_RGB_Color*)create_memory(sizeof(GlobalTable_RGB_Color)*(Image_Desc.local_clr_Size)/3);
				memset(LocalTable,0,sizeof(GlobalTable_RGB_Color)*(Image_Desc.local_clr_Size)/3);
				/* Initialising Local Table */
				Image_Desc.local_clr_Size=Image_Desc.local_clr_Size/3;
		
				/***** Reading LocalColorTable ****/
				while(Image_Desc.local_clr_Size)
				{
			
					fread(&((LocalTable+LOCALINDEX)->Red),1,1,GIF_FILE);
					fread(&((LocalTable+LOCALINDEX)->Green),1,1,GIF_FILE);
					fread(&((LocalTable+LOCALINDEX)->Blue),1,1,GIF_FILE);
					LOCALINDEX++;
					Image_Desc.local_clr_Size--;
			
				}
			}
						
			RawData=Read_Data_Block();
			
			Data=Lzw_Decode(RawData,Image_Desc);
			if(FrameInfo->Next==NULL)
			{

			FrameInfo->frameScreenInfo=Image_Desc;
            FrameInfo->InterFrameDelay=Graph_Renderblk.Delay_Time;
			FrameInfo->transperencyflag=Graph_Renderblk.Tranparent_Clr_Flag;
			FrameInfo->DisposalMethod=Graph_Renderblk.DisposalMethod;
			if(FrameInfo->frameScreenInfo.local_Clr_Flag)
				FrameInfo->CMAP=LocalTable;
			else
				FrameInfo->CMAP=CLMAP;
			if(FrameInfo->transperencyflag)
				FrameInfo->transperencyindex=Graph_Renderblk.Transp_Clr_Index;
			else
				FrameInfo->transperencyindex='\0';
            (FrameInfo)->frame=(Byte*)create_memory(Image_Desc.ImageHeight*Image_Desc.ImageWidth*3);
			memset((FrameInfo)->frame,0,Image_Desc.ImageHeight*Image_Desc.ImageWidth*3);
            memcpy((FrameInfo)->frame,Data,Image_Desc.ImageHeight*Image_Desc.ImageWidth*3);
            free(Data);
			Data=NULL;
            (FrameInfo)->Next=FrameStarter;
            }
			else
			{
            (FrameInfo)->Next=(FrameData*)create_memory(sizeof(FrameData));
			FrameInfo=FrameInfo->Next;
			(FrameInfo)->frameScreenInfo=Image_Desc;
            (FrameInfo)->InterFrameDelay=Graph_Renderblk.Delay_Time;
            (FrameInfo)->frame=(Byte*)create_memory(Image_Desc.ImageHeight*Image_Desc.ImageWidth*3);
			memset((FrameInfo)->frame,0,Image_Desc.ImageHeight*Image_Desc.ImageWidth*3);
            memcpy((FrameInfo)->frame,Data,Image_Desc.ImageHeight*Image_Desc.ImageWidth*3);
            free(Data);
			Data=NULL;
			FrameInfo->DisposalMethod=Graph_Renderblk.DisposalMethod;
			if(FrameInfo->frameScreenInfo.local_Clr_Flag)
				FrameInfo->CMAP=LocalTable;
			else
				FrameInfo->CMAP=CLMAP;
			if(FrameInfo->transperencyflag)
				FrameInfo->transperencyindex=Graph_Renderblk.Transp_Clr_Index;
			else
				FrameInfo->transperencyindex='\0';

		    (FrameInfo)->Next=FrameStarter;
			}
			
			fread(&Data_Block,1,1,GIF_FILE);
			fread(&Data_Block,1,1,GIF_FILE);
			break;
			
		case ApplicationExtension:
			fread(&Data_Block,1,1,GIF_FILE);
			while(Data_Block!=gTypeExtension)
			{
				fread(&Data_Block,1,1,GIF_FILE);
				
				
			}
			fread(&Data_Block,1,1,GIF_FILE);
			break;
			
		case CommentExtension:
			fread(&Data_Block,1,1,GIF_FILE);
			while(Data_Block!=gTypeExtension)
			{
				fread(&Data_Block,1,1,GIF_FILE);
				
				
			}
			fread(&Data_Block,1,1,GIF_FILE);
			break;
			
			
		}
		
	}
	
	fclose(GIF_FILE);
	Display(FrameStarter);
	
	
}