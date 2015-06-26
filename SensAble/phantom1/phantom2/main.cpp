
#include <stdio.h>
#include <assert.h>

#include <conio.h>

#include "lib/common.h"
#include "control.h"

#include <HD/hd.h>
#include <HDU/hduVector.h>
#include <HDU/hduError.h>

void mainLoop(void);

HDCallbackCode HDCALLBACK gravityWellCallback(void *data);


int main(int argc, char* argv[])
{    
 //SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
//SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
    init_control();

    HDErrorInfo error;
    HDSchedulerHandle hGravityWell;

    /* Initialize the device, must be done before attempting to call any hd 
       functions. Passing in HD_DEFAULT_DEVICE causes the default device to be 
       initialized. */
    HHD hHD = hdInitDevice(HD_DEFAULT_DEVICE);

	if (HD_DEVICE_ERROR(error = hdGetError())) 
    {
        hduPrintError(stderr, &error, "Failed to initialize haptic device");
        fprintf(stderr, "\nPress any key to quit.\n");
        getch();
        return -1;
    }

    printf("Hello Haptic Device!\n");
    printf("Found device model: %s.\n\n", hdGetString(HD_DEVICE_MODEL_TYPE));

    /* Schedule the main callback that will render forces to the device. */
    hGravityWell = hdScheduleAsynchronous(
        gravityWellCallback, 0, 
        HD_MAX_SCHEDULER_PRIORITY);

    hdEnable(HD_FORCE_OUTPUT);
	hdDisable(HD_SOFTWARE_VELOCITY_LIMIT);
	hdSetSchedulerRate(1000);
	
    hdStartScheduler();
    
    /* Check for errors and abort if so. */
    if (HD_DEVICE_ERROR(error = hdGetError()))
    {
        hduPrintError(stderr, &error, "Failed to start scheduler");
        fprintf(stderr, "\nPress any key to quit.\n");
        return -1;
    }

    /* Wait until the user presses a key.  Meanwhile, the scheduler
       runs and applies forces to the device. */
    printf("Feel around for the gravity well...\n");
    printf("Press any key to quit.\n\n");
    while (!_kbhit())
    {
        /* Periodically check if the gravity well callback has exited. */
        if (!hdWaitForCompletion(hGravityWell, HD_WAIT_CHECK_STATUS))
        {
            fprintf(stderr, "Press any key to quit.\n");     
            getch();
            break;
        }
    }
   
    /* For cleanup, unschedule callback and stop the scheduler. */
    hdStopScheduler();
    hdUnschedule(hGravityWell);

    /* Disable the device. */
    hdDisableDevice(hHD);

    return 0;
}


HDCallbackCode HDCALLBACK gravityWellCallback(void *data)
  {
  static timer t;
  static long long count=0;
  double frame_interval,total_length,control_length;
  
  count++;
  frame_interval=t(1);
  t.start(1);
  
  HDErrorInfo error;
  double pos[3],vel[3],ang[3],force[3],mat[16],tcf[32];
  long enc[6],dac[6],setdac[6],tcl[32];
  
  HHD hHD = hdGetCurrentDevice();
    
  hdBeginFrame(hHD);
    
  
  hdGetDoublev(HD_CURRENT_POSITION, pos);
  hdGetDoublev(HD_CURRENT_JOINT_ANGLES, ang);
  hdGetDoublev(HD_CURRENT_VELOCITY, vel);
  hdGetDoublev(HD_CURRENT_FORCE, force);
  hdGetDoublev(HD_CURRENT_TRANSFORM, mat);
  hdGetLongv(HD_CURRENT_ENCODER_VALUES, enc);
  hdGetLongv(HD_CURRENT_MOTOR_DAC_VALUES, dac);
  
  
  memset(setdac,0,sizeof(setdac));
  memset(tcf,0,sizeof(tcf));
  memset(tcl,0,sizeof(tcl));
  
  
  for(int q1=0;q1<3;q1++)tcf[q1]=pos[q1];
  for(int q1=0;q1<3;q1++)tcf[q1+3]=ang[q1];
  
  
  
  t.start(2);  
  control(tcf,NULL,enc,setdac);
  control_length=t(2);
    
    
  hdSetLongv(HD_CURRENT_MOTOR_DAC_VALUES,setdac);
	
  	
  	  
  hdEndFrame(hHD);
  
   
  if(HD_DEVICE_ERROR(error=hdGetError()))
    {
    hduPrintError(stderr,&error,"Error detected while rendering gravity well\n");   
    //if(hduIsSchedulerError(&error))return HD_CALLBACK_DONE;
    if(hduIsSchedulerError(&error))return HD_CALLBACK_CONTINUE;
    }   
  
  total_length=t(1);
  
  if(count%100==1110 || frame_interval>0.0011)printf("%lf %lf %lf\n",frame_interval,total_length,control_length);
  
  return HD_CALLBACK_CONTINUE;
  }
