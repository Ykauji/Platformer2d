#ifndef _H_NETWORK_COMMON
#define _H_NETWORK_COMMON

	#ifdef IDE_VS2008
		#include "../../platform/windows/Source/WindowsNetwork.h"
	#endif

	#ifdef IDE_BADA
		#include "../../platform/samsung/Source/SamsungNetwork.h"
	#endif

	#ifdef IDE_XCODE
		#include "../../platform/apple/Source/AppleNetwork.h"
	#endif

	#ifdef IDE_MAC
		#include "../../platform/mac/Source/MacNetwork.h"
	#endif

	#ifdef IDE_MEEGO
		#include "../../platform/meego/Source/MeeGoNetwork.h"
	#endif

	#ifdef IDE_ANDROID
		#include "../../platform/android/Source/AndroidNetwork.h"
	#endif

	#ifdef IDE_BLACKBERRY
		#include "../../platform/blackberry/Source/BlackberryNetwork.h"
	#endif

	#ifdef IDE_LINUX
		#include "../../platform/linux/Source/LinuxNetwork.h"
	#endif

	#ifdef IDE_PI
		#include "../../platform/pi/Source/PiNetwork.h"
	#endif
	
	#ifdef IDE_HTML5
		#include "../../platform/html5/Source/HTML5Network.h"
	#endif

#endif
