// Common Includes (for Win32)

// windows 7 for touch commands
#define _WIN32_WINNT 0x0601

#include "AGK.h"
#include <windows.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <stdio.h>
#include <gl\gl.h>								// Header File For The OpenGL32 Library
#include <gl\glu.h>								// Header File For The GLu32 Library
#include "direct.h"

// App specific include for Core (so core can be distributed between apps without editing inside it)
#include "interpreter.h"

#ifdef AGK_STEAM
	#include "steam/steam_api.h"
	#include "Shlwapi.h"
#endif

#define WM_SOUND_EVENT 0x8001
#define WM_VIDEO_EVENT 0x8002

// comment out to revert to single mouse
//#define MULTI_MOUSE

// Namespace
using namespace AGK;

// Globals for core
HANDLE hDebugConsoleOut = NULL;
HWND g_hWnd = NULL;
bool g_bAGKInitialised = false;

//
// Windows Application STUB requires entry function, create window and message pump
//

#define AGK_MOUSE_LEFTDOWN		0x00000001
#define AGK_MOUSE_RIGHTDOWN		0x00000002
#define AGK_MOUSE_MIDDLEDOWN	0x00000004

void PlatformAppQuit()
{
	PostQuitMessage(0);
}

class cMouseDevice
{
	protected:

	public:
		cMouseDevice() { ID = 0; hDevice = 0; x = 0; y = 0; pNext = 0; sprite = 0; m_bFlags = 0; }
		~cMouseDevice();

		UINT ID;
		HANDLE hDevice;
		int x;
		int y;
		UINT m_bFlags;
		cMouseDevice *pNext;
		UINT sprite;
};

cMouseDevice *pMouseList = 0;

void Output( const char *szMsg )
{
	if ( !hDebugConsoleOut ) 
	{
		AllocConsole ( );
		HWND hConsole = GetConsoleWindow();
		MoveWindow( hConsole, 32, 500, 800, 350, TRUE );
		hDebugConsoleOut = GetStdHandle ( STD_OUTPUT_HANDLE );
	}
	
	DWORD dwWritten = 0;
	WriteConsole ( hDebugConsoleOut, "> ", 2, &dwWritten, NULL );
	WriteConsole ( hDebugConsoleOut, szMsg, (DWORD) strlen( szMsg ), &dwWritten, NULL );
	WriteConsole ( hDebugConsoleOut, "\n", 1, &dwWritten, NULL );
}

unsigned int TranslateKey( unsigned int key )
{
	switch( key )
	{
		case 20: key = 0; break; // Caps lock
		case 120: key = 0; break; // F9
		case 121: key = 0; break; // F10
		case 122: key = 0; break; // F11
		case 123: key = 0; break; // F12
		case 18: key = 18; break; // Alt Gr
		case 91: key = 0; break; // Windows key (left)
		case 92: key = 0; break; // Windows key (right)

		// Num pad 0-9
		case 96:
		case 97: 
		case 98: 
		case 99: 
		case 100: 
		case 101: 
		case 102: 
		case 103: 
		case 104: 
		case 105: key -= 48; break;

		case 111: key = 192; break; // Num pad /
		case 109: key = 189; break; // Num pad -
		case 110: key = 190; break; // Num pad .

		case 144: key = 0; break;  // Num lock
		case 19: key = 0; break;  // pause/break
	}

	return key;
}

bool g_bShouldBeTopMost = false;

// delay the loading of these functions so Windows XP and Vista don't complain
int (__stdcall *CloseTouchInputHandleDelayed)( HTOUCHINPUT ) = 0;
int (__stdcall *GetTouchInputInfoDelayed)( HTOUCHINPUT, unsigned int, PTOUCHINPUT, int ) = 0;
BOOL (__stdcall *RegisterTouchWindowDelay)(HWND, ULONG) = 0;

void LoadDelayedFunctions()
{
	// only checks once
	static int first = 1;
	if ( first == 1 )
	{
		first = 0;
		HMODULE user32 = LoadLibrary( "USER32.dll" );
		CloseTouchInputHandleDelayed = (int(__stdcall *)(HTOUCHINPUT)) GetProcAddress( user32, "CloseTouchInputHandle" );
		GetTouchInputInfoDelayed = (int(__stdcall *)(HTOUCHINPUT, unsigned int, PTOUCHINPUT, int)) GetProcAddress( user32, "GetTouchInputInfo" );
		RegisterTouchWindowDelay = (BOOL(__stdcall *)(HWND, ULONG)) GetProcAddress( user32, "RegisterTouchWindow" );
		FreeLibrary( user32 );
	}
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static bool bLeftDown = false;

	switch (message)
	{
#ifdef WM_TOUCH
		// only works on Windows 7 or above
		case WM_TOUCH:
		{
			LoadDelayedFunctions();

			if ( CloseTouchInputHandleDelayed )
			{
				// windows based touch device
				UINT cInputs = LOWORD(wParam);
				if ( cInputs > 0 )
				{
					// compensate for window (which may not be in top left)
					RECT rect;
					::GetWindowRect( hWnd, &rect );
					
					PTOUCHINPUT pInputs = new TOUCHINPUT[cInputs];
					if (GetTouchInputInfoDelayed((HTOUCHINPUT)lParam, cInputs, pInputs, sizeof(TOUCHINPUT)))
					{
						// process pInputs
						for ( UINT i = 0; i < cInputs; i++ )
						{
							pInputs[i].x -= (rect.left*100);
							pInputs[i].y -= (rect.top*100);
							pInputs[i].x /= 100;
							pInputs[i].y /= 100;

							if ( pInputs[i].dwFlags & TOUCHEVENTF_DOWN ) agk::TouchPressed( pInputs[i].dwID, pInputs[i].x, pInputs[i].y );
							if ( pInputs[i].dwFlags & TOUCHEVENTF_MOVE ) agk::TouchMoved( pInputs[i].dwID, pInputs[i].x, pInputs[i].y );
							if ( pInputs[i].dwFlags & TOUCHEVENTF_UP ) agk::TouchReleased( pInputs[i].dwID, pInputs[i].x, pInputs[i].y );
						}
						
						CloseTouchInputHandleDelayed((HTOUCHINPUT)lParam);
					}
					
					delete [] pInputs;
				}
			}

			break;
		}
		
#endif

#ifdef MULTI_MOUSE
		case WM_INPUT:
		{			
			if ( !bCapturePointer ) break;
			// raw input from registered devices (in this case mice)
			UINT dwSize;
			GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER));
			LPBYTE lpb = new BYTE[dwSize];
			GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER));
			RAWINPUT* raw = (RAWINPUT*)lpb;

			// find mouse device
			int count = 0;
			cMouseDevice *pMouse = pMouseList;
			while ( pMouse )
			{
				count++;
				if ( pMouse->hDevice == raw->header.hDevice ) break;
				pMouse = pMouse->pNext;
			}

			// is this a new mouse device?
			if ( !pMouse )
			{
				pMouse = new cMouseDevice();
				pMouse->hDevice = raw->header.hDevice;
				pMouse->pNext = pMouseList;
				pMouse->ID = count+1;
				pMouse->x = lastX;
				pMouse->y = lastY;
				pMouse->sprite = agk::CreateSprite( 99 );
				agk::SetSpriteSize( pMouse->sprite, 4,-1 );
				agk::SetSpriteTransparency( pMouse->sprite, 2 );
				switch( count )
				{
					case 0: agk::SetSpriteColor( pMouse->sprite, 255,0,0,255 ); break;
					case 1: agk::SetSpriteColor( pMouse->sprite, 0,255,0,255 ); break;
					case 2: agk::SetSpriteColor( pMouse->sprite, 0,0,255,255 ); break;
					case 3: agk::SetSpriteColor( pMouse->sprite, 255,255,0,255 ); break;
				}
				pMouseList = pMouse;
			}

			// relative or absolute positioning?
			if ( raw->data.mouse.usFlags & MOUSE_MOVE_ABSOLUTE )
			{
				pMouse->x = raw->data.mouse.lLastX;
				pMouse->y = raw->data.mouse.lLastY;
			}
			else
			{
				pMouse->x += raw->data.mouse.lLastX;
				pMouse->y += raw->data.mouse.lLastY;
			}

			// check buttons
			if ( raw->data.mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_DOWN )
			{
				if ( (pMouse->m_bFlags & AGK_MOUSE_LEFTDOWN) == 0 )
				{
					agk::TouchPressed( pMouse->ID, pMouse->x, pMouse->y );
				}

				pMouse->m_bFlags |= AGK_MOUSE_LEFTDOWN;
			}

			if ( raw->data.mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_UP )
			{
				if ( (pMouse->m_bFlags & AGK_MOUSE_LEFTDOWN) != 0 )
				{
					agk::TouchReleased( pMouse->ID, pMouse->x, pMouse->y );
				}

				pMouse->m_bFlags &= ~AGK_MOUSE_LEFTDOWN;
			}

			// if holding left button send drag event
			if ( (pMouse->m_bFlags & AGK_MOUSE_LEFTDOWN) != 0 )
			{
				if ( raw->data.mouse.lLastX != 0 && raw->data.mouse.lLastY != 0 )
				{
					agk::TouchMoved( pMouse->ID, pMouse->x, pMouse->y );
				}
			}

			if ( pMouse->x > DEVICE_WIDTH ) pMouse->x = DEVICE_WIDTH;
			if ( pMouse->y > DEVICE_HEIGHT ) pMouse->y = DEVICE_HEIGHT;
			if ( pMouse->x < 0 ) pMouse->x = 0;
			if ( pMouse->y < 0 ) pMouse->y = 0;

			agk::SetSpritePosition( pMouse->sprite, agk::DeviceToScreenX(pMouse->x), agk::DeviceToScreenY(pMouse->y) );

			//char str[ 256 ];
			//sprintf( str, "X: %d, Y: %d", pMouse->x, pMouse->y );
			//Output( str );
			if ( bCapturePointer )
			{
				RECT rect;
				::GetWindowRect( hWnd, &rect );
				int centerX = (rect.left + rect.right) / 2;
				int centerY = (rect.top + rect.bottom) / 2;
				SetCursorPos( centerX,centerY );
			}
			break;
		}
#endif

		case WM_LBUTTONDOWN:
		{
			bLeftDown = true;
			int x = lParam & 0x0000ffff;
			int y = (lParam >> 16) & 0x0000ffff;
			if ( x > 32767 ) x = x - 65536;
			if ( y > 32767 ) y = y - 65536;

			// detect pen touch
			//if ( (GetMessageExtraInfo() & 0xFFFFFF00) != 0xFF515700 ) 
				agk::MouseLeftButton( 0, 1 );

#ifdef MULTI_MOUSE
			if ( !bCapturePointer )
			{
				bCapturePointer = true;
				ShowCursor( FALSE );
				lastX = x;
				lastY = y;
			}
#else
			SetCapture(hWnd);
			//agk::TouchPressed( 1, x,y );
#endif
			break;
		}

		case WM_LBUTTONUP:
		{
			bLeftDown = false;
			int x = lParam & 0x0000ffff;
			int y = (lParam >> 16) & 0x0000ffff;
			if ( x > 32767 ) x = x - 65536;
			if ( y > 32767 ) y = y - 65536;

			// detect pen touch
			//if ( (GetMessageExtraInfo() & 0xFFFFFF00) != 0xFF515700 ) 
				agk::MouseLeftButton( 0, 0 );

#ifndef MULTI_MOUSE
			ReleaseCapture();
			//agk::TouchReleased( 1, x,y );
#endif
			break;
		}

		case WM_RBUTTONDOWN:
		{
			agk::MouseRightButton( 0, 1 );

#ifdef MULTI_MOUSE
			if ( bCapturePointer ) // Alt Gr
			{
				bCapturePointer = false;
				ShowCursor( TRUE );
			}
#endif
			break;
		}

		case WM_RBUTTONUP:
		{
			agk::MouseRightButton( 0, 0 );
			break;
		}

		case WM_MBUTTONDOWN:
		{
			agk::MouseMiddleButton( 0, 1 );
			break;
		}

		case WM_MBUTTONUP:
		{
			agk::MouseMiddleButton( 0, 0 );
			break;
		}

		case WM_MOUSEWHEEL:
		{
			int w = (wParam >> 16) & 0x0000ffff;
			if ( w > 32767 ) w = w - 65536;

			float fW = w / 40.0f;

			agk::MouseWheel( 0, fW );
			break;
		}

		case WM_MOUSEMOVE:
		{
			int x = lParam & 0x0000ffff;
			int y = (lParam >> 16) & 0x0000ffff;
			if ( x > 32767 ) x = x - 65536;
			if ( y > 32767 ) y = y - 65536;

			agk::MouseMove( 0, x, y );

			if ( bLeftDown )
			{
#ifndef MULTI_MOUSE
				//agk::TouchMoved( 1, x,y );
#endif
			}
			break;
		}

		case WM_SIZE:
		{
			// if maximizing then set window to topmost to prevent it overlapping the start button when "use small icons" is ticked
			// which seems to cause image stutter
			g_bShouldBeTopMost = (wParam == SIZE_MAXIMIZED);
			if ( g_bShouldBeTopMost ) ::SetWindowPos( hWnd, HWND_TOPMOST, 0,0,0,0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSENDCHANGING | SWP_NOSIZE );
			else ::SetWindowPos( hWnd, HWND_NOTOPMOST, 0,0,0,0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSENDCHANGING | SWP_NOSIZE );

			agk::UpdateDeviceSize();
			agk::WindowMoved();
			break;
		}

		case WM_WINDOWPOSCHANGED:
		case WM_MOVE:
		{
			if ( g_bAGKInitialised )
			{
				agk::WindowMoved();

				// save window position for next opening
				RECT rc;
				GetWindowRect( hWnd, &rc );
				cFile posFile;
				posFile.OpenToWrite( "/window_position.dat" );
				posFile.WriteInteger( rc.left );
				posFile.WriteInteger( rc.right );
				posFile.WriteInteger( rc.top );
				posFile.WriteInteger( rc.bottom );
				posFile.Close();
			}

			return DefWindowProc(hWnd, message, wParam, lParam);
		}

		case WM_ACTIVATE:
		{
			// if being made inactive and currently we are topmost temporarily remove topmost until we are active again.
			if ( LOWORD(wParam) != WA_INACTIVE ) 
			{
				if ( g_bShouldBeTopMost ) ::SetWindowPos( hWnd, HWND_TOPMOST, 0,0,0,0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSENDCHANGING | SWP_NOSIZE );
				agk::Resumed();
			}
			else
			{
				agk::MouseLeftButton( 0, 0 );
				agk::MouseRightButton( 0, 0 );
				agk::MouseMiddleButton( 0, 0 );
				if ( g_bShouldBeTopMost ) ::SetWindowPos( hWnd, HWND_NOTOPMOST, 0,0,0,0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSENDCHANGING | SWP_NOSIZE );
				agk::Paused();
			}
			agk::WindowMoved();
			return DefWindowProc(hWnd, message, wParam, lParam);
		}

		case WM_NCACTIVATE:
		{
			if ( wParam ) agk::WindowMoved();
			return DefWindowProc(hWnd, message, wParam, lParam);
		}

		case WM_SETFOCUS:
		{
			agk::WindowMoved();
			return DefWindowProc(hWnd, message, wParam, lParam);
		}

		case WM_KEYDOWN:
		{
#ifdef MULTI_MOUSE
			if ( wParam == 17 && bCapturePointer ) // Alt Gr
			{
				bCapturePointer = false;
				ShowCursor( TRUE );
			}
#endif
			unsigned int key = TranslateKey( wParam );
			if ( key > 0 )
			{
				agk::KeyDown( key );
			}
			
			/*
			uString sKey;
			sKey.Format( "Key: %d", wParam );
			agk::Warning( sKey );
			*/

			//if ( wParam == VK_ESCAPE ) PostQuitMessage(0);
			break;
		}

		case WM_KEYUP:
		{
			unsigned int key = TranslateKey( wParam );
			if ( key > 0 )
			{
				agk::KeyUp( key );
			}
			break;
		}

		case WM_CHAR:
		{
			agk::CharDown( wParam );
			break;
		}

		case WM_SYSKEYDOWN:
		{
			if ( wParam == VK_MENU ) agk::KeyDown( 18 ); // Alt
			break;
		}

		case WM_SYSKEYUP:
		{
			if ( wParam == VK_MENU ) agk::KeyUp( 18 ); // Alt
			break;
		}

		case WM_SOUND_EVENT:
		{
			agk::HandleMusicEvents( (void*) lParam );
			break;
		}

		case WM_VIDEO_EVENT:
		{
			agk::HandleVideoEvents();
			break;
		}
			
		case WM_COMMAND:
		{
			if ( (wParam & 0xffff) == 124 )
			{
				// capture
				agk::CaptureImage();
				agk::CancelCapture();
			}
			else if ( (wParam & 0xffff) == 125 )
			{
				// cancel
				agk::CancelCapture();
			}
			break;
		}

		case WM_CLOSE:
		{
			//PostQuitMessage(0);
			App.AppQuit();
			break;
		}

		case WM_ERASEBKGND:
		{
			break;
		}

		case WM_PAINT:
		{
			ValidateRect( hWnd, NULL );
			break;
		}

		case 0x031E: //WM_DWMCOMPOSITIONCHANGED
		{
			agk::CompositionChanged();
			break;
		}

		default:
		{
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	return 0;
}

void CreateInternalBitmapsForLIBIMAGES ( void )
{
	// use it
	if ( 0 )
	{
		// used to create byte data from images (used in builds)
		// interpreter graphics are stored internally, generated in hard code, right here (in debug mode)
		
		// The new lib images are uploaded and stored as PNG to try and keep the size down
		// although the backdrop file takes up 600KB of the 700KB libImages.h file.
		// We will read in the file and output it's raw data with the following code:
		for ( int n=0; n<1; n++ )
		{
			const char* pFileIn, *pFileOut;
			if ( n==0 ) { pFileIn = "interpreter-logo.png"; pFileOut="interpreter-logo.txt"; }
			cFile oFile, oFile2;
			oFile.OpenToRead( pFileIn );
			oFile2.OpenToWrite( pFileOut );
			int count = 0;
			int count2 = 0;
			char str[ 30 ];
			while ( !oFile.IsEOF() )
			{
				UINT i = oFile.ReadInteger();
				sprintf( str, "0x%x, ", i );
				oFile2.WriteData( str, strlen(str) );
				count++;
				count2++;
				if ( count > 30 ) 
				{
					oFile2.WriteData( "\\\n", 2 );
					count = 0;
				}
			}
			sprintf( str, "\nsize = %d", count2 );
			oFile2.WriteData( str, strlen(str) );
			oFile2.Close();
			oFile.Close();
		}
		// The final line will be a size value
		// copy that into the "libImageInterpreterBackgroundPNGSize" parameter in libImages.h
		// copy everything else into the curley braces after "libImageInterpreterBackgroundPNG"
		// (or whatever image variable you are modifying). Everything else should be automatic.
		// output file is located at: C:\Users\you\Documents\AGK\yourapp\
		// i.e. C:\Users\Lee\Documents\AGK\c_TGCSHARED_GDKProWorkspace_GDKPro_Compiler_AGKFinalOuput_Projects_Basic_TestArea_SimpleTier1_SimpleTier1exe
	}
}

HWND CreateWin32Window( HINSTANCE hInstance, int width, int height, uString &szTitle, int x, int y, bool fullscreen )
{
	//MessageBox( NULL, "Wait", "Info", 0 );

	HANDLE hIcon = LoadImageA(NULL, "icon.ico", IMAGE_ICON, 16, 16, LR_LOADFROMFILE);

	// register the class
	WNDCLASSEX wcex;

	// hardcoded resource IDs for icons
	wcex.cbSize			= sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= hIcon ? (HICON)hIcon : LoadIcon(hInstance, MAKEINTRESOURCE(104)); 
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= "OPENGLWINDOW";
	wcex.hIconSm		= NULL;
	RegisterClassEx(&wcex);

	// check the given values against the desktop resolution and center if requested (x=-1 or y=-1)
	RECT rc;
	GetWindowRect(GetDesktopWindow(), &rc);
	
	if ( width < 1 ) width = 1;
	if ( height < 1 ) height = 1;

	float appAspect = width / (float) height;
	float windowAspect = (rc.right-rc.left-15) / (float) (rc.bottom-rc.top-80);

	if ( appAspect > windowAspect )
	{
		if ( width > rc.right-rc.left-15 )
		{
			float ratio = (rc.right-rc.left-15) / (float)width;
			width = rc.right-rc.left-15;
			height = (int) (height*ratio);
		}
	}
	else
	{
		if ( height > rc.bottom-rc.top-80 )
		{
			float ratio = (rc.bottom-rc.top-80) / (float)height;
			height = rc.bottom-rc.top-80;
			width = (int) (width*ratio);
		}
	}
	
	if ( x == -1 ) x = ((rc.right-rc.left)-width)/2 + rc.left;
	if ( y == -1 ) y = ((rc.bottom-rc.top-50)-height)/2 + rc.top;
	if ( x < 0 ) x = 0;
	if ( y < 0 ) y = 0;
	if ( x+width > rc.right ) x = rc.right-width;
	if ( y+height > rc.bottom ) y = rc.bottom-height;

	// check if window has a previous position
	
	int loaded = 0;
	if ( agk::GetFileExists( "/window_position.dat" ) )
	{
		cFile posFile;
		posFile.OpenToRead( "/window_position.dat" );

		RECT rc2;
		rc2.left = posFile.ReadInteger();
		rc2.right = posFile.ReadInteger();
		rc2.top = posFile.ReadInteger();
		rc2.bottom = posFile.ReadInteger();
		posFile.Close();

		x = rc2.left;
		y = rc2.top;
		width = rc2.right - rc2.left;
		height = rc2.bottom - rc2.top;

		int vLeft = GetSystemMetrics( SM_XVIRTUALSCREEN );
		int vTop = GetSystemMetrics( SM_YVIRTUALSCREEN );
		int vRight = GetSystemMetrics( SM_CXVIRTUALSCREEN ) + vLeft;
		int vBottom = GetSystemMetrics( SM_CYVIRTUALSCREEN ) + vTop;

		if ( x+width > vRight ) x = vRight - width;
		if ( y+height > vBottom ) x = vBottom - height;

		if ( x < vLeft ) x = vLeft;
		if ( y < vTop ) y = vTop;

		loaded = 1;
	}

	if ( fullscreen )												
    {
		width = rc.right-rc.left;
		height = rc.bottom-rc.top;
		x = 0;
		y = 0;
		loaded = 0;

		/*
    	DEVMODE dmScreenSettings;								// Device Mode
    	memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	// Makes Sure Memory's Cleared
    	dmScreenSettings.dmSize=sizeof(dmScreenSettings);		// Size Of The Devmode Structure
		dmScreenSettings.dmPelsWidth	= width;				// Selected Screen Width
		dmScreenSettings.dmPelsHeight	= height;				// Selected Screen Height
    	dmScreenSettings.dmBitsPerPel	= 32;					// Selected Bits Per Pixel
    	dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

    	// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
    	if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
    	{
    		fullscreen = false;
    	}
		*/
    }

	DWORD dwExStyle;
	DWORD dwStyle;

	if ( fullscreen )
    {
    	dwExStyle = WS_EX_APPWINDOW;							
    	dwStyle = WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;	
    }
    else
    {
    	dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;	
    	dwStyle = WS_OVERLAPPEDWINDOW & (~WS_SIZEBOX);	
    }

	// adjust the window and make it bigger for borders, window title, etc
	RECT WindowRect;							
	WindowRect.left=(long)0;					
	WindowRect.right=(long)width;				
	WindowRect.top=(long)0;						
	WindowRect.bottom=(long)height;				
	if ( !loaded ) AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);

	// initialise window
	HWND hWnd = CreateWindow("OPENGLWINDOW", szTitle.GetStr(), dwStyle, x, y, 
			WindowRect.right-WindowRect.left, WindowRect.bottom-WindowRect.top, NULL, NULL, hInstance, NULL);

	// terminate if failed to create window
	if (!hWnd) return FALSE;
	g_hWnd = hWnd;
	
	// show window
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

#ifdef MULTI_MOUSE
	// Register that we want raw input from mice
	RAWINPUTDEVICE pDevices;
	pDevices.usUsage = 2;
	pDevices.usUsagePage = 1;
	pDevices.hwndTarget = hWnd;
	pDevices.dwFlags = 0;
	RegisterRawInputDevices( &pDevices, 1, sizeof(RAWINPUTDEVICE) );
#endif

	LoadDelayedFunctions();
	if ( RegisterTouchWindowDelay ) RegisterTouchWindowDelay(hWnd,TWF_WANTPALM);

	// success
	return hWnd;
}

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
#ifdef AGK_STEAM
	// SteamAPI_Init() will produce a message box if steam_api.dll is not found, so check for it first
	if ( PathFileExists( "steam_api.dll" ) )
	{
		SteamAPI_Init();
	}
#endif

	// load company name from bytecode file to make write folder path more unique, used in InitGL
	if ( agk::GetFileExists("media/bytecode.byc") && App.m_sProgram.LoadBytecodeHeader("media/bytecode.byc") )
	{
		agk::SetCompanyName( App.m_sProgram.GetCompanyName() );
	}

	agk::InitFilePaths();

	App.g_dwDeviceWidth = DEVICE_WIDTH;
	App.g_dwDeviceHeight = DEVICE_HEIGHT;
	App.g_dwFullScreen = FULLSCREEN ? 1 : 0;

	char* pSetupFile = (char*)"setup.agc";
	if ( agk::GetFileExists ( pSetupFile )==1 )
	{
		char* pField = (char*)"";
		strcpy ( App.g_pWindowTitle, "" );
		agk::OpenToRead ( 1, pSetupFile );
		while ( agk::FileEOF ( 1 )==false ) 
		{
			char* pLineToRead = agk::ReadLine ( 1 );
			pField=(char*)"title="; if ( strncmp ( pLineToRead, pField, strlen(pField) )==0 )	strcpy ( App.g_pWindowTitle, pLineToRead+strlen(pField) );
			pField=(char*)"width="; if ( strncmp ( pLineToRead, pField, strlen(pField) )==0 )	App.g_dwDeviceWidth = (unsigned int)atoi(pLineToRead+strlen(pField));
			pField=(char*)"height="; if ( strncmp ( pLineToRead, pField, strlen(pField) )==0 )	App.g_dwDeviceHeight = (unsigned int)atoi(pLineToRead+strlen(pField));
			pField=(char*)"fullscreen="; if ( strncmp ( pLineToRead, pField, strlen(pField) )==0 )	App.g_dwFullScreen = (unsigned int)atoi(pLineToRead+strlen(pField));
			pField=(char*)"resolutionmode="; if ( strncmp ( pLineToRead, pField, strlen(pField) )==0 )	App.g_dwResolutionMode = (unsigned int)atoi(pLineToRead+strlen(pField));
		}
		agk::CloseFile ( 1 );
	}

	// create a win32 window for the app
	uString szTitle( App.g_pWindowTitle );
	HWND hWnd = CreateWin32Window( hInstance, App.g_dwDeviceWidth, App.g_dwDeviceHeight, szTitle, -1, -1, App.g_dwFullScreen==1 );

	agk::SetExtraAGKPlayerAssetsMode ( 2 ); // 0-no assets, 1-minimum assets, 2-extra AGK Player assets

	// call app begin
	bool bExitLoop = false;
	try
	{
		// initialise graphics API (win32 openGL) for app
		agk::InitGL( (void*) hWnd );
		g_bAGKInitialised = true;

		App.Begin();
	}
	catch(...)
	{
		uString err; agk::GetLastError(err);
		err.Prepend( "Uncaught exception: \n\n" );
		MessageBox( NULL, err.GetStr(), "Error", 0 );
		bExitLoop = true;
	}

	int firstRun = 1;

	// message pump
	MSG msg;
	while ( !bExitLoop )
	{
		if ( PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) ) 
		{	
			if ( msg.message == WM_QUIT ) bExitLoop = true;
			if ( msg.message == 799 ) continue; // this is sent just after a change to fullscreen mode and for some reason sets it back to windowed mode
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			/*
			if ( msg.message == 799 && firstRun == 1 )
			{
				firstRun = 0;
				if ( App.g_dwFullScreen == 1  )
				{
					SetWindowLongPtr( g_hWnd, GWL_STYLE, WS_SYSMENU | WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE );
					SetWindowLongPtr( g_hWnd, GWL_EXSTYLE, WS_EX_APPWINDOW );
					::SetWindowPos( g_hWnd, 0, 0,0,0,0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSENDCHANGING | SWP_NOSIZE );
				}
			}
			*/
		}
		else 
		{
			// call app each frame
			try
			{
				if ( !agk::IsCapturingImage() ) 
				{
					App.Loop();
				}
				else
				{
					agk::Sleep( 10 );
				}
			}
			catch(...)
			{
				uString err; agk::GetLastError(err);
				err.Prepend( "Uncaught exception: \n\n" );
				MessageBox( NULL, err.GetStr(), "Error", 0 );
				bExitLoop = true;
			}
		}
	}
	
	// call app end 
	App.End();

	agk::CleanUp();

	// finished
	return 0;
}

