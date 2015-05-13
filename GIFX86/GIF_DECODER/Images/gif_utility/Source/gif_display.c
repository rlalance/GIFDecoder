


#ifndef _SYSM_
#include"sysm.h"
#endif

#ifndef _DISPLAY_
#include"gif_display.h"
#endif


#ifndef _GIFLZW_
#include"gif_lzw.h"
#endif

#ifndef _READ_
#include"gif_read.h"
#endif


extern  hWnd;
HDC DisplayCntrl;



void SETBACKGROUND(FrameData *tempInfo)
{
	
	unsigned int ImageStartX=0;
	unsigned int ImageStartY=0;
	
	
	
	ImageStartX=(tempInfo->frameScreenInfo.LeftPosition);
	ImageStartY=(tempInfo->frameScreenInfo.TopPosition);
	
	
    while(ImageStartY<(tempInfo->frameScreenInfo.ImageHeight))
	{
		
		while(ImageStartX<(tempInfo->frameScreenInfo.ImageWidth))
		{
			
			
			
			SetPixel(DisplayCntrl,ImageStartX,ImageStartY,RGB(255,255,255));
			
			ImageStartX++;
		}
		
		ImageStartX=(tempInfo->frameScreenInfo.LeftPosition);
		ImageStartY++;
		
		
	}
				
	
	
}


void Display(FrameData *FrameInfo)
{
	
	
	
	short int ImageStartX=0;
	short int ImageStartY=0;
	int Index=0;
	
	
	
	DisplayCntrl=GetDC(hWnd);
	
	
	
	
	while(1)
	{
		
		Index=0;
		ImageStartX=(FrameInfo->frameScreenInfo.LeftPosition);
		ImageStartY=(FrameInfo->frameScreenInfo.TopPosition);   
		
		while(ImageStartY<((FrameInfo->frameScreenInfo.ImageHeight)+(FrameInfo->frameScreenInfo.TopPosition)))
		{
			
			while(ImageStartX<((FrameInfo->frameScreenInfo.ImageWidth)+(FrameInfo->frameScreenInfo.LeftPosition)))
			{
				if(FrameInfo->frame[Index]!=FrameInfo->transperencyindex)
					SetPixel(DisplayCntrl,ImageStartX,ImageStartY,RGB(((FrameInfo->CMAP)+(FrameInfo->frame[Index]))->Red,((FrameInfo->CMAP)+(FrameInfo->frame[Index]))->Green,((FrameInfo->CMAP)+(FrameInfo->frame[Index]))->Blue));
				
				Index++;
				ImageStartX++;
			}
			
			
			ImageStartY++;
			
			
			ImageStartX=(FrameInfo->frameScreenInfo.LeftPosition);
		}
		
		
		
		Sleep((FrameInfo->InterFrameDelay*10));
		WaitForSingleObject(hWnd,10); 
	/*if(	FrameInfo->DisposalMethod==2)
		{
			
			SETBACKGROUND(FrameInfo);
		}*/
		FrameInfo=FrameInfo->Next;
	
	}
	
	
	
}

