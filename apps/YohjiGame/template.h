#ifndef _H_APP
#define _H_APP

#include "agk.h"
#include "player.h"
#include "level.h"
#include "camera.h"

// use in Core.mm to set the window properties
#define DEVICE_WIDTH 1920
#define DEVICE_HEIGHT 1080
#define WINDOW_TITLE "OverThroned"
#define FULLSCREEN 0

// used to make a more unique folder for the write path
#define COMPANY_NAME "Yojo Inc."

// Global values for the app
class app
{
	public:
        Player * mainPlayero;
        Camera2D potato;
        Level levelOne;
        UI userInterface;
        // If 0, main menu, 1 game, 2 menu
        int gameState;
        int background;
        // global game vars
    
        // Global Func
        std::string intToString(int value);

	public:

		// constructor
		app() {
            mainPlayero = new Player();
        }
        ~app() {}

		// main app functions
        void Begin( void );
        int Loop( void );
        void End( void );
};

extern app App;


// When adding new classes change .cpp into Objective C++ Source or sad boi
#endif
