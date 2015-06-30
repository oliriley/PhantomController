/*Class for Phantom Omni device*/
#include <stdio.h>

#if defined(WIN32)
# include <conio.h>
#else
# include "conio.h"
#endif

#include <HD/hd.h>
#include <HDU/hduError.h>
#include <HDU/hduVector.h>

///////////////////////////////
/////Stuff Oliver has added////
double dPosCurrentX;
double dPosCurrentY;
double dPosCurrentZ;

double dForceCurrentX;
double dForceCurrentY;
double dForceCurrentZ;

int count;
static int MAX_COUNT = 750;
///////////////////////////////
///////////////////////////////

HDSchedulerHandle gCallbackHandle = 0;

void mainLoop();

HDCallbackCode HDCALLBACK OmniForceCallback(void *pUserData);

/******************************************************************************
 Main function.
******************************************************************************/
int main(int argc, char* argv[])
{  
    Omni device = Omni();

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
HDCallbackCode HDCALLBACK OmniForceCallback(void *pUserData)
{
    static hduVector3Dd anchor;
    static HDboolean bRenderForce = FALSE;
    HDErrorInfo error;

    // HDint nCurrentButtons, nLastButtons;
    hduVector3Dd position;
    hduVector3Dd force = { 0, 0, 0 };

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

/////////////////////////////////////////////////////////////////////
//Stuff Oliver has added: call to update position and force display//
	if (count>MAX_COUNT)
	{
		updateUser(position, force);
		count -= MAX_COUNT;
	}
//////////////////////////////////////////////
//////////////////////////////////////////////

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