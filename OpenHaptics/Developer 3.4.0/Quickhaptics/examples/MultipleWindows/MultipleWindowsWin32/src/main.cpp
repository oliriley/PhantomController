/////////////////////////////////////////////////////////////////////////////
//OpenHaptics QuickHaptics - MultipleWindow example
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
	
	
	
	
	QHWin32* DisplayObject1 = new QHWin32;//create a display window
    DisplayObject1->hapticWindow(true);//The haptics are with respect to this window
    DisplayObject1->setWindowTitle("Front View\0");//make the title of this window "Front View"


    QHWin32* DisplayObject2 = new QHWin32;//create a display window
    DisplayObject2->hapticWindow(false);//Disable haptics in this window
    DisplayObject2->setWindowTitle("Right View\0");//Set the title of this window as "Left View"


    QHWin32* DisplayObject3 = new QHWin32;//create a display window
    DisplayObject3->hapticWindow(false);//Disable Haptics in this window
    DisplayObject3->setWindowTitle("Top View\0");//Set the title of this window as "Top View"
   
    DeviceSpace* OmniSpace = new DeviceSpace;//Find the default device
    DisplayObject1->tell(OmniSpace);//Tell QuickHaptics about it

    TriMesh* Cow = new TriMesh("models/cow.3ds");//Load a cow model
	Cow->setFriction();//give friction to the cow surface
    DisplayObject1->tell(Cow);//Tell Quickhaptics about the cow

    Cursor* OmniCursor = new Cursor;//Declare a new cursor
    DisplayObject1->tell(OmniCursor);//tell QuickHaptics about the cursor


    float FOV1, FOV2, FOV3;
    float NearPlane1, NearPlane2, NearPlane3;
    float FarPlane1, FarPlane2, FarPlane3;
    hduVector3Dd Eye1, Eye2, Eye3;
    hduVector3Dd LookAt1, LookAt2, LookAt3;
    hduVector3Dd UpVector1, UpVector2, UpVector3;
  
    
    DisplayObject1->setDefaultCamera();
    DisplayObject1->getCamera(&FOV1,&NearPlane1,&FarPlane1,&Eye1,&LookAt1,&UpVector1);

    
    FOV3 = FOV2 = FOV1;
    NearPlane3 = NearPlane2 = NearPlane1;
    FarPlane3 = FarPlane2 = FarPlane1;
    Eye3 = Eye2 = Eye1;
    LookAt3 = LookAt2 = LookAt1;
    UpVector2 = UpVector1;
	UpVector3 = UpVector1;

    Eye2.set(Eye1[2],LookAt2[1],LookAt2[2]);
    Eye3.set(LookAt2[0],Eye1[2],LookAt2[2]);
    UpVector3.set(0.0,0.0,-1.0);
    
    DisplayObject2->setCamera(FOV2, NearPlane2, FarPlane2, Eye2,LookAt2, UpVector2);
    DisplayObject3->setCamera(FOV3, NearPlane3, FarPlane3, Eye3,LookAt3, UpVector3);
   
    qhStart();//Set everything in motion
}



