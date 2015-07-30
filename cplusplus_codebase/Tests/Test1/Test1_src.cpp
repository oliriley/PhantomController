/*	Test of setup for Win32 Console (GLUT) application
	Start date	26 July 2015
	Author		Oliver Stanley
	Group		Moritz lab, UW Dept. Rehab Medicine	*/

#include <cstdio>
#include <iostream>

#include <HD/hd.h>
#include <HL/hl.h>
//#include <QHHeadersGLUT.h>
//#include <QHHeadersWin32.h>
#include <HDU/hduVector.h>
#include <GL/glut.h>
int main()
{
	printf("Hello World!");
	return 0;
}

/*// Initialization of device, forces, and scheduler
HHD hHD = hdInitDevice(HD_DEFAULT_DEVICE);

hdEnable(HD_FORCE_OUTPUT);

hdStartScheduler();

// Haptic functions

hdBeginFrame(hHD);

hdEndFrame(hHD);

hdStopScheduler();

hdUnschedule(scheduleCallbackHandle);

hdDisabelDevice(hdGetCurrentDevice());
*/