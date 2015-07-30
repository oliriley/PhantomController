/////////////////////////////////////////////////////////////////////////////
//OpenHaptics QuickHaptics - EarthSpin example
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
    DeviceSpace* Omni = new DeviceSpace;//Find the default device
    DisplayObject->tell(Omni);//tell quickHaptics that Omni exists
    
    Sphere* SimpleSphere1 = new Sphere();//Decalsre a sphere
    SimpleSphere1->setSpin();//Make it spin
	SimpleSphere1->setFriction();//Give it friction
    SimpleSphere1->setTexture("models/earth.jpg");//Load the earth texture on it
    DisplayObject->tell(SimpleSphere1);//tell Quickhaptics that a sphere exists

    Cursor* OmniCursor = new Cursor;//Declare a default cursor
    DisplayObject->tell(OmniCursor);//Tell Quick haptics that a cursor exists
    
  
    qhStart();//Set everything in motion
}


