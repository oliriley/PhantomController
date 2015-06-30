class Omni()
{
public:
	hduVector3Dd position, force;

	void updateUser(hduVector3Dd position, hduVector3Dd force)
	{
		if (position[0] != dPosCurrentX || position[1] != dPosCurrentY || position[2] != dPosCurrentZ)
		{
			dPosCurrentX = position[0];
			dPosCurrentY = position[1];
			dPosCurrentZ = position[2];
				
			printf("x = %f\n",dPosCurrentX);
			printf("y = %f\n",dPosCurrentY);
			printf("z = %f\n",dPosCurrentZ);
			printf("\n");
		}
		if (force[0] != dForceCurrentX || force[1] != dForceCurrentY || force[2] != dForceCurrentZ)
		{
			dForceCurrentX = force[0];
			dForceCurrentY = force[1];
			dForceCurrentZ = force[2];

			printf("x force = %f\n",dForceCurrentX);
			printf("y force = %f\n",dForceCurrentY);
			printf("z force = %f\n",dForceCurrentZ);
			printf("\n");
		}
	}

	Omni()
	{ 
		HDErrorInfo error;

	    HHD hHD = hdInitDevice(HD_DEFAULT_DEVICE);
	    if (HD_DEVICE_ERROR(error = hdGetError()))
	    {
	        hduPrintError(stderr, &error, "Failed to initialize haptic device");
	        fprintf(stderr, "\nPress any key to quit.\n");
	        getch();
	        return -1;
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
	        return -1;
	    }
	}
}