/* 
---------------------------------------------------------------- 
	This is a barebones Xcode Project and Application that is 
	intended to help you in seting up your own projects.
	This source code is by no means a suggestion of how you should
	create your application, but instead just a reference that 
	addresses key OS X specific issues.

Based partially on code from:
						http://www.ogre3d.org/wiki/index.php/MinimalApplication
---------------------------------------------------------------- 
	*/ 

// Used to determine the fully qualified path to your application
// bundle.
#include "macBundlePath.h"
#include "gamem.h"



GameManager gmMgr;

////////////////////////////////////////////////////////////////
// Window Close UPP - "universal procedure pointers"
// -------------------------------------------------
// This code deals with the Carbon event manager, and is here so
// we are notified when the user wants to quit the application.
// Depending on your application, this may be all the
// functionality you need.  However A more robust solution for
// carbon would be to use something like OIS, as the samples do.
////////////////////////////////////////////////////////////////

// This is the window close callback
OSStatus doWindowClose(EventHandlerCallRef nextHandler, 
					   EventRef theEvent, void* userData)
{
	// Set our application state to false, causing an exit on the
	// next update
	gmMgr.stop();
	
	
	// Propegate the event down the chain
	return CallNextEventHandler (nextHandler, theEvent);
}

// This sets up the EventManager to use our callback when needed
void setupEventHandler()
{
	OSStatus status;
	WindowRef appWindowRef = GetUserFocusWindow();
	EventHandlerUPP appHandler = NewEventHandlerUPP(doWindowClose);
	const EventTypeSpec windowEvents[] =
	{
	{ kEventClassWindow, kEventWindowClose },
	{ kEventClassWindow, kEventWindowCloseAll }
	};
	
	status = InstallWindowEventHandler(appWindowRef, appHandler, GetEventTypeCount(windowEvents), windowEvents, NULL, NULL);
}





////////////////////////////////////////////////////////////////
// main
////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{

	// This is the path to our stored resources, we copy the
	// resources there with Xcode using the Copy Bundle
	// Resources Phase of the Application Target.
	std::string resourcePath = macBundlePath() + "/Contents/Resources/media";
	
		setupEventHandler();

	gmMgr.init(resourcePath);
	gmMgr.run();
	
	
	
	
	return 0;
}





