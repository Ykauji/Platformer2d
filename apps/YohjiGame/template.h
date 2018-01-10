#ifndef _H_APP
#define _H_APP

// Includes
#include "agk.h"
#include "player.h"
#include "level.h"



// use in Core.mm to set the window properties
#define DEVICE_WIDTH 1920
#define DEVICE_HEIGHT 1080
#define WINDOW_TITLE "Jim"
#define FULLSCREEN 0

// used to make a more unique folder for the write path
#define COMPANY_NAME "Yojo Inc."

// Global values for the app
class app
{
	public:
        Player mainPlayer;
        Level levelOne;
        UI userInterface;
        int background;
        float xSin = 0;
        // global game vars
    
        // Global Func
        std::string intToString(int value);

	public:

		// constructor
		app() {}
        ~app() {}

		// main app functions - mike to experiment with a derived class for this..
        void Begin( void );
        int Loop( void );
        void End( void );
};

extern app App;


// When adding new classes change .cpp into Objective C++ Source or sad boi
#endif
