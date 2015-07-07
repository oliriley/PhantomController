#include <stdio.h>

#if defined(WIN32)
# include <conio.h>
#else
# include "conio.h"
#endif

#include <HD/hd.h>
#include <HDU/hduError.h>
#include <HDU/hduVector.h>

int count;
static int MAX_COUNT = 750;
HHD hHD;

HDSchedulerHandle gCallbackHandle = 0;

void mainLoop();
class Omni;
HDCallbackCode HDCALLBACK OmniForceCallback(void *pUserData/*, Omni device*/);

class Omni
{
protected:
	hduVector3Dd position, force;
	double x,y,z;
	double xf,yf,zf;
public:
	double getPosX();
	double getPosY();
	double getPosZ();
	double getForceX();
	double getForceY();
	double getForceZ();
	
	Omni();
	
	~Omni();

	void updateUser();

	HHD setup();
};

Omni::Omni()
{ 
	x = 0;
	y = 0;
	z = 0;
	xf = 0;
	yf = 0;
	zf = 0;
}

Omni::~Omni() {}

double Omni::getPosX() {return position[0];}

double Omni::getPosY() {return position[1];}

double Omni::getPosZ() {return position[2];}

double Omni::getForceX() {return force[0];}

double Omni::getForceY() {return force[1];}

double Omni::getForceZ() {return force[2];}

void Omni::updateUser()	
{
	if (position[0] != x || position[1] != y || position[2] != z)
	{
		x = position[0];
		y = position[1];
		z = position[2];
				
		printf("x = %f\n",x);
		printf("y = %f\n",y);
		printf("z = %f\n",z);
		printf("\n");
	}
	if (force[0] != xf || force[1] != yf || force[2] != zf)
	{
		xf = force[0];
		yf = force[1];
		zf = force[2];

		printf("x force = %f\n",xf);
		printf("y force = %f\n",yf);
		printf("z force = %f\n",zf);
		printf("\n");
	}
}

HHD Omni::setup()
{
	HDErrorInfo error;

	HHD hHD = hdInitDevice(HD_DEFAULT_DEVICE);
	if (HD_DEVICE_ERROR(error = hdGetError()))
	{
	    hduPrintError(stderr, &error, "Failed to initialize haptic device");
	    fprintf(stderr, "\nPress any key to quit.\n");
	    getch();
//	    return -1;
	}

	printf("Omni Initialized\n");

	/* Schedule the haptic callback function for continuously monitoring the
		button state and rendering the anchored spring force. */
	gCallbackHandle = hdScheduleAsynchronous(OmniForceCallback, 0, HD_MAX_SCHEDULER_PRIORITY);

	hdEnable(HD_FORCE_OUTPUT);
	
	/* Start the haptic rendering loop. */
	hdStartScheduler();
	if (HD_DEVICE_ERROR(error = hdGetError()))
	{
	    hduPrintError(stderr, &error, "Failed to start scheduler");
	    fprintf(stderr, "\nPress any key to quit.\n");
	    getch();
//	    return -1;
	}
	return hHD;
}

//////Declaring device as global scope - find a different workaround////////
Omni device = Omni();
////////////////////////////////////////////////////////////////////////////

/******************************************************************************
 Main function.
******************************************************************************/
int main(int argc, char* argv[])
{  
	hHD = device.setup();

    /* Start the main application loop. */
    mainLoop();

    /* Cleanup by stopping the haptics loop, unscheduling the asynchronous
       callback, disabling the device. */
    hdStopScheduler();
    hdUnschedule(gCallbackHandle);
    hdDisableDevice(hHD);

    return 0;
}

/******************************************************************************
 Main loop.  
 Detects and interprets keypresses.  Monitors and initiates error recovery if 
 necessary.
******************************************************************************/
void mainLoop()
{
    int keypress;

    while (TRUE)
    {       
        /* Check for keyboard input. */
        if (_kbhit())
        {
            keypress = getch();
            keypress = toupper(keypress);
            
            if (keypress == 'Q')
            {
                return;
            }
            else
            {
                printf("\nInvalid keypress\n");
                printf("Press Q to quit.\n\n");
            }
        }

        /* Check if the main scheduler callback has exited. */
        if (!hdWaitForCompletion(gCallbackHandle, HD_WAIT_CHECK_STATUS))
        {
            fprintf(stderr, "\nThe main scheduler callback has exited\n");
            fprintf(stderr, "\nPress any key to quit.\n");
            getch();
            return;
        }
    }
}
/******************************************************************************
 * Main scheduler callback for rendering the anchored spring force.
 *****************************************************************************/
HDCallbackCode HDCALLBACK OmniForceCallback(void *pUserData/*, Omni device*/)
{
    static hduVector3Dd anchor;
    static HDboolean bRenderForce = FALSE;
    HDErrorInfo error;

    // HDint nCurrentButtons, nLastButtons;
    hduVector3Dd position;
    hduVector3Dd force;
	force[0] = 0;
	force[1] = 0;
	force[2] = 0;

    hdBeginFrame(hdGetCurrentDevice());

    hdGetDoublev(HD_CURRENT_POSITION, position);

	/*hdGetIntegerv(HD_CURRENT_BUTTONS, &nCurrentButtons);
    hdGetIntegerv(HD_LAST_BUTTONS, &nLastButtons);

    if ((nCurrentButtons & HD_DEVICE_BUTTON_1) != 0 &&
        (nLastButtons & HD_DEVICE_BUTTON_1) == 0)
    {
        * Detected button down *
        memcpy(anchor, position, sizeof(hduVector3Dd));
        bRenderForce = TRUE;
    }
    else if ((nCurrentButtons & HD_DEVICE_BUTTON_1) == 0 &&
             (nLastButtons & HD_DEVICE_BUTTON_1) != 0)

    {
        * Detected button up *
        bRenderForce = FALSE;

        * Send zero force to the device, or else it will just continue
           rendering the last force sent *
        hdSetDoublev(HD_CURRENT_FORCE, force);
    }

    if (bRenderForce)
    {
        * Compute spring force as F = k * (anchor - pos), which will attract
           the device position towards the anchor position *
        hduVecSubtract(force, anchor, position);
        hduVecScaleInPlace(force, gSpringStiffness);
                
        hdSetDoublev(HD_CURRENT_FORCE, force);
    }*/

	if (count>MAX_COUNT)
	{
		device.updateUser();
		count -= MAX_COUNT;
	}

    hdEndFrame(hdGetCurrentDevice());

    /* Check if an error occurred while attempting to render the force */
    if (HD_DEVICE_ERROR(error = hdGetError()))
    {
        if (hduIsForceError(&error))
        {
            bRenderForce = FALSE;
        }
        else if (hduIsSchedulerError(&error))
        {
            return HD_CALLBACK_DONE;
        }
    }
	count++;
    return HD_CALLBACK_CONTINUE;
}	