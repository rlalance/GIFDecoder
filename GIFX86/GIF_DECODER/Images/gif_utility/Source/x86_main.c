#ifndef _SYSM_
#include"sysm.h"
#endif


HWND hWnd;
extern DisplayCntrl;
void Gif_Read();
HANDLE hThread;


const char *ClassName = "MyWindow";

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, 
						 WPARAM wParam, LPARAM lParam);


DWORD WINAPI ThreadFunc( void* param )
{

	Gif_Read();
	return(0);
    
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrev, 
                   LPSTR lpCmdLine, int nShowCmd) 
{
	
  DWORD  threadId;

    

	
	WNDCLASSEX wc;
	// Handle to our window.
	

	// Will hold messages sent to our window.
	
	MSG msg;
	
	// The size of the structure
	
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	wc.hCursor = LoadCursor(hInstance, IDC_ARROW);
	wc.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(hInstance, IDI_APPLICATION);
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = ClassName;
	wc.lpszMenuName = NULL;
	wc.style = 0;
	
	// Register the class to be used with CreateWindow.
	
	if(!RegisterClassEx(&wc)) {
		MessageBox(NULL, "Could not register window.", "Error",
			MB_ICONEXCLAMATION | MB_OK);
		return -1;
	}
	
   	
	
	// Create the window using the "MyWindow" class.
	
	hWnd = CreateWindowEx(WS_EX_WINDOWEDGE, ClassName, "GIF DECODER", 
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT|WS_HSCROLL|WS_VSCROLL, 900, 900,
		NULL, NULL, hInstance, NULL);
	
	// If the window was not created show 
	// If the window was not created show error and exit.
	
	if(hWnd == NULL) {
		MessageBox(NULL, "Could not create window.", "Error",
			MB_ICONEXCLAMATION | MB_OK);
		return -1;
	}
	
	// Set the windows show state, to show it.
	

	
	ShowWindow(hWnd, nShowCmd);
	// Draw the window.


	
	UpdateWindow(hWnd);
		
	


//hThread = CreateThread( NULL, 0, ThreadFunc, NULL, 
  //        0, &threadId );

    
	
	
Gif_Read();
	/*********READ BMP ***************************/
	
	
	// Retrieve messages from the message queue.
	
	while(GetMessage(&msg, NULL, 0, 0) > 0) {
		CloseHandle( hThread );
		// Convert virtual key messages into a char
		
		TranslateMessage(&msg);
		// Send the message to our WndProc function
		
		DispatchMessage(&msg);
	}
	
	return msg.wParam;
}

// This will handle the messages.

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, 
						 WPARAM wParam, LPARAM lParam) {
	
	PAINTSTRUCT ps;
	switch(uMsg) {
		// Recieved when the user hits the X box or alt-F4
		
	case WM_CLOSE:
		CloseHandle( hThread );
		// Tell the program to end, and dispath WM_DESTROY
		
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		// End the program.
		
		PostQuitMessage(0);
		break;
		case WM_PAINT:

		EndPaint(hWnd, &ps);
			break;
	default:
		// DefWindowProc processes anything we don't.
		
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	
	return 0;
}
