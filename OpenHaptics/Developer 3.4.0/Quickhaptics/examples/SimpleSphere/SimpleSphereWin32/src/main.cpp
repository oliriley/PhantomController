/////////////////////////////////////////////////////////////////////////////
//OpenHaptics QuickHaptics - SimpleSphere example
//SensAble Technologies, Woburn, MA
//September 03, 2008
//Programmer: Hari Vasudevan
//////////////////////////////////////////////////////////////////////////////

#include <QHHeadersWin32.h>



int WINAPI WinMain(	HINSTANCE	hInstance,				// Instance
			HINSTANCE	hPrevInstance,				// Previous Instance
			LPSTR		lpCmdLine,				// Command Line Parameters
			int		nCmdShow)				// Window Show State
{
	
	
	
	
	QHWin32* DisplayObject = new QHWin32;//create a display window
    DeviceSpace* Omni = new DeviceSpace;//Find a the default phantom
    DisplayObject->tell(Omni);//Tell Quickhaptics that omni exists
    
    Sphere* SimpleSphere = new Sphere;//Get a sphere
    DisplayObject->tell(SimpleSphere);//Tell Quick Haptics that the sphere exists

    Cursor* OmniCursor = new Cursor;//Create a cursor
    DisplayObject->tell(OmniCursor);//Tell QuickHaptics that a cursor exists
    
  
    qhStart();//Set everything in motion
}


