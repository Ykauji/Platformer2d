// includes
#import <Cocoa/Cocoa.h>
#include "agk.h"
#include "template.h"

// platform specific
unsigned int TranslateKey( unsigned int key )
{
	switch( key )
	{
		case GLFW_KEY_ESC: key = 27; break; // Esc
			
		// F1 - F8
		case GLFW_KEY_F1: key = 112; break;
		case GLFW_KEY_F2: key = 113; break;
		case GLFW_KEY_F3: key = 114; break;
		case GLFW_KEY_F4: key = 115; break;
		case GLFW_KEY_F5: key = 116; break;
		case GLFW_KEY_F6: key = 117; break;
		case GLFW_KEY_F7: key = 118; break;
		case GLFW_KEY_F8: key = 119; break;
			
		case 96: key = 223; break; // `
		case 45: key = 189; break; // -
		case 61: key = 187; break; // =
		case GLFW_KEY_HOME: key = 36; break; // Home
		case GLFW_KEY_PAGEUP: key = 33; break; // PgUp
		case GLFW_KEY_PAGEDOWN: key = 34; break; // PgDown
		case GLFW_KEY_END: key = 35; break; // End
		case GLFW_KEY_DEL: key = 46; break; // Del
		//case 160: key = 223; break;  // +-
		case GLFW_KEY_INSERT: key = 45; break; // Insert
			
		case GLFW_KEY_LSHIFT: key = 16; break; // Shift (left)
		case GLFW_KEY_RSHIFT: key = 16; break; // Shift (right)
		case GLFW_KEY_LCTRL: key = 17; break; // Ctrl (left)
		case GLFW_KEY_RCTRL: key = 17; break; // Ctrl (right)
		case GLFW_KEY_LALT: key = 0; break;  // Alt/Cmd (left)
		case GLFW_KEY_RALT: key = 18; break;  // Cmd (right)
			
		case GLFW_KEY_TAB: key = 9; break; // Tab
		case GLFW_KEY_ENTER: key = 13; break; // Enter
		case GLFW_KEY_BACKSPACE: key = 8; break; // Backspace
			
		case 91: key = 219; break; // [
		case 93: key = 221; break; // ]
		case 59: key = 186; break; // ;
		case 39: key = 192; break; // '
			
		case GLFW_KEY_LEFT: key = 37; break; // Left
		case GLFW_KEY_UP: key = 38; break; // Up
		case GLFW_KEY_RIGHT: key = 39; break; // Right
		case GLFW_KEY_DOWN: key = 40; break; // Down
			
		case 44: key = 188; break; // ,
		case 46: key = 190; break; // .
		case 47: key = 191; break; // /
		case 92: key = 220; break; // \
			
		case GLFW_KEY_KP_ENTER: key = 13; break; // Num pad enter
		case GLFW_KEY_KP_DIVIDE: key = 111; break; // Num pad divide 
		case GLFW_KEY_KP_MULTIPLY: key = 106; break; // Num pad multiply
		case GLFW_KEY_KP_SUBTRACT: key = 109; break; // Num pad subtract
		case GLFW_KEY_KP_ADD: key = 107; break; // Num pad add
		case GLFW_KEY_KP_DECIMAL: key = 110; break; // Num pad decimcal
		case GLFW_KEY_PAUSE: key = 0; break; // Pause/Break
		case GLFW_KEY_CAPS_LOCK: key = 0; break; // Caps lock 
		case GLFW_KEY_KP_0: key = 45; break; // Num pad 0
		case GLFW_KEY_KP_1: key = 35; break; // Num pad 1
		case GLFW_KEY_KP_2: key = 40; break; // Num pad 2
		case GLFW_KEY_KP_3: key = 34; break; // Num pad 3
		case GLFW_KEY_KP_4: key = 37; break; // Num pad 4
		case GLFW_KEY_KP_5: key = 12; break; // Num pad 5
		case GLFW_KEY_KP_6: key = 39; break; // Num pad 6
		case GLFW_KEY_KP_7: key = 36; break; // Num pad 7
		case GLFW_KEY_KP_8: key = 38; break; // Num pad 8
		case GLFW_KEY_KP_9: key = 33; break; // Num pad 9
			
		case 323: key = 0; break; // Command button
	}
	
	return key;
}

static void GLFWCALL keyboard_callback( int key, int action )
{
	if ( action == GLFW_PRESS )
	{
		agk::KeyDown( TranslateKey(key) );
	}
	else 
	{
		agk::KeyUp( TranslateKey(key) );
	}

}

static void GLFWCALL char_callback( int key, int action )
{
	if ( action == GLFW_PRESS )
	{
		agk::CharDown( key );
	}
}

static void GLFWCALL mouse_pos_callback( int x, int y )
{
	agk::MouseMove( 0, x, y );
}

static void GLFWCALL mouse_button_callback( int button, int action )
{
	if ( button == GLFW_MOUSE_BUTTON_LEFT )
	{
		agk::MouseLeftButton( 0, action==GLFW_PRESS ? 1 : 0 );
	}
	else if ( button == GLFW_MOUSE_BUTTON_RIGHT )
	{
		agk::MouseRightButton( 0, action==GLFW_PRESS ? 1 : 0 );
	}
}

static float g_fWheelPrev = 0;

static void GLFWCALL mouse_wheel_callback( int w )
{
    float wheel = w - g_fWheelPrev;
    g_fWheelPrev = (float) w;
    agk::MouseWheel(0, wheel);
}

static void GLFWCALL window_size_callback(int width, int height)
{
	agk::UpdateDeviceSize();
}


int main (int argc, char **argv)
{
    agk::InitJoysticks();
    
	// create a window for the app
	if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        exit(1);
    }
    
    // Creates Screens
    NSScreen *mainScreen = [NSScreen mainScreen];
    NSRect screenRect = [mainScreen visibleFrame];
    
    int width = DEVICE_WIDTH;
    int height = DEVICE_HEIGHT;
    
    float appAspect = width / (float) height;
	float windowAspect = (screenRect.size.width-15) / (float) (screenRect.size.height-80);
    
	if ( appAspect > windowAspect )
	{
		if ( width > screenRect.size.width-15 )
		{
			float ratio = (screenRect.size.width-15) / (float)width;
			width = screenRect.size.width-15;
			height = (int) height*ratio;
		}
	}
	else
	{
		if ( height > screenRect.size.height-80 )
		{
			float ratio = (screenRect.size.height-80) / (float)height;
			height = screenRect.size.height-80;
			width = (int) width*ratio;
		}
	}
	
	glfwOpenWindowHint( GLFW_FSAA_SAMPLES, 4 );
    
    UINT wfflag=GLFW_WINDOW;
#if FULLSCREEN == 1
    //wfflag=GLFW_FULLSCREEN;
#endif
	if (!glfwOpenWindow(width, height, 8, 8, 8, 8, 32, 0, wfflag))
    {
        glfwTerminate();
        fprintf(stderr, "Failed to open GLFW window\n");
        exit(1);
    }
	glfwSetWindowTitle(WINDOW_TITLE);
	glfwSetWindowSizeCallback( window_size_callback );
	glfwSetKeyCallback( keyboard_callback );
	glfwSetMousePosCallback( mouse_pos_callback );
	glfwSetMouseButtonCallback( mouse_button_callback );
	glfwSetMouseWheelCallback( mouse_wheel_callback );
	glfwSetCharCallback( char_callback );
	
	// initialise graphics API for app
    agk::SetExtraAGKPlayerAssetsMode ( 1 );
		
	agk::SetCompanyName( COMPANY_NAME );

	// call app begin
	int done = 0;
	try
	{
		agk::InitGL(0);
        
#if FULLSCREEN == 1
        agk::SetWindowSize( 0,0, 1 );
#endif

		// rename minimize button to pass Apple validation
		[[[[[NSApp mainMenu] itemAtIndex:1] submenu] itemAtIndex:0] setTitle:@"Minimize"];

		App.Begin();
	}
	catch( ... )
	{
		uString err = agk::GetLastError();
		err.Prepend( "Uncaught exception: \n\n" );
		NSString* pString = [ [ NSString alloc ] initWithCString: err.GetStr() ];
		NSAlert *alert = [[NSAlert alloc] init];
		[ alert setMessageText: pString ];
		[ pString release ];
		[ alert runModal ];
		[ alert release ];
		done = 1;
	}
    
	while (!done && glfwGetWindowParam(GLFW_OPENED) == GL_TRUE)
	{
		// call app each frame
		try
		{
            if ( App.Loop() == 1 ) done = 1;
		}
		catch( ... )
		{
			uString err = agk::GetLastError();
			err.Prepend( "Uncaught exception: \n\n" );
			NSString* pString = [ [ NSString alloc ] initWithCString: err.GetStr() ];
			NSAlert *alert = [[NSAlert alloc] init];
			[ alert setMessageText: pString ];
			[ pString release ];
			[ alert runModal ];
			[ alert release ];
			done = 1;
		}
	}
	
	// close app
	App.End();
    agk::CleanUp();
	
	// close window system
    glfwTerminate();
	
	// success    
    return 0;
}
