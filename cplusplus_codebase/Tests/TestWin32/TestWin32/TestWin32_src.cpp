/*	Test of setup for Win32 Console (GLUT) application
	Start date	26 July 2015
	Author		Oliver Stanley
	Group		Moritz lab, UW Dept. Rehab Medicine	*/

#include <TestWin32.h>

int successCount = 0;

float stiffness = LO_STIFF;
float damping = LO_DAMP;

ofstream trainingData(getRat(),ios::ate|ios::app);
int BASE_TICKS = GetTickCount();
int PREV_TICKS = BASE_TICKS;
int CURR_TICKS;

int main(int argc, char *argv[])
{	
	if (!trainingData.is_open())
	{
		printf("Problem opening training data. Please retry.\n");
		getch();
		return -1;
	}
	
	/*string WTnum;
	printf("Rat weight: ");
	trainingData << getline(cin,WTnum);*/

	//Initialize haptic device
	HHD hHD = hdInitDevice(HD_DEFAULT_DEVICE);

	// Make sure nothing broke
	if (errReport("Failed to initialize haptic device.\n","Device successfully initialized.\n"))
	{
		return -1;
	}

	// Turn on haptic force feedback
	hdEnable(HD_FORCE_OUTPUT);
	printf("Force output enabled.\n");

	// Start thread for reading position and applying force
	hdStartScheduler();
	
	// Make sure nothing broke
	if (errReport("Failed to start scheduler.\n","Scheduler started.\n"))
	{
		return -1;
	}

	// This is the function that actually monitors the device and applies forces
	hapticLoop(/*trainingData*/);

	// Close down all the things started earlier
	hdStopScheduler();
	hdDisable(HD_FORCE_OUTPUT);
	hdUnschedule(HD_CALLBACK_DONE);
	hdDisableDevice(hdGetCurrentDevice());

	// Wrap up and report to the user
	printf("All done!\n");
	getch();
	return 0;
}

// This is the function that actually monitors the device and applies forces
void hapticLoop()
{
	bool is_success;

	// An array to be rewritten repeatedly with
	// new forces depending on the situation
	hduVector3Dd force;
		force[0] = 0;
		force[1] = 0;
		force[2] = 0;
	
	// Arrays to take in the handle
	// position in 3D, storing one previous value
	hduVector3Dd position;
	hduVector3Dd positionPast;

	// anchor is where the handle will return to, where
	// [0 0 0] represents the point the device was last calibrated to
	hduVector3Dd anchor;
		anchor[0] = 0;
		anchor[1] = 0;
		anchor[2] = 0;

	while(true)
	{
		// A 'frame' is a block of code within which
		// all device properties are assumed static
		hdBeginFrame(hdGetCurrentDevice());

		// Track position before updating
		positionPast = position;		
		
		// Write the current device coordinates to 'position
		hdGetDoublev(HD_CURRENT_POSITION, position);

		// Check success condition(s)
		is_success = checkSuccess(position);

		// Write the difference between 'position' and 'anchor' to 'force'
		hduVecSubtract(force, anchor, position);
		force[1] = force[1] + G;

		// Scale 'force' by 'stiffness', 'damping', and the distance from the last position, defined at the top
		hduVecScaleInPlace(force, stiffness);

		// Write the resulting 'force' vector to the device
		hdSetDoublev(HD_CURRENT_FORCE, force);

		// Write current state to output file
		CURR_TICKS = GetTickCount();
		if (CURR_TICKS - PREV_TICKS > 0)
		{
			trainingData << CURR_TICKS - BASE_TICKS << ",\t";
			trainingData << successCount << ",\t";
			trainingData << position[0] << ",\t";
			trainingData << position[1] << ",\t";
			trainingData << position[2] << ",\t";
			trainingData << force[0] << ",\t";
			trainingData << force[1] << ",\t";
			trainingData << force[2] << "\n";

			PREV_TICKS = CURR_TICKS;
		}

		// At the end of the frame, device properties will (/can?) change again
		hdEndFrame(hdGetCurrentDevice());

		if (is_success)
		{
			//goHome(position,anchor,positionPast,force);	
			waitForHome(position,anchor,positionPast,force);
		}

		// See if the user has instructed the program to quit
		if (checkQuit())
		{
			return;
		}
	}
}

string getRat()
{
	string IDnum;
	string year;
	string month;
	string day;
	
	printf("\nRat ID ###: ");
	getline(cin,IDnum);

	printf("\nYear (YYYY): ");
	getline(cin,year);

	printf("\nMonth (MM): ");
	getline(cin,month);

	printf("\nDay (DD): ");
	getline(cin,day);

	return IDnum + "_" +  year + month + day + ".txt";
}

float distance3D(hduVector3Dd position, hduVector3Dd anchor)
{
	float xdist = pow(position[0] - anchor[0],2);
	float ydist = pow(position[1] - anchor[1],2); 
	float zdist = pow(position[2] - anchor[2],2);\
	return sqrt(xdist + ydist + zdist);
}

bool checkQuit()
{
	if (_kbhit())
	{
		int keypress = getch();
		keypress = toupper(keypress);
            
		if (keypress == 'Q')
		{
			printf("You quitter.\n");
			return true;
		}
	}
	return false;
}

bool checkSuccess(hduVector3Dd position)
{
	return (position[1] < SUCCESS_BARRIER);
}

bool errReport(char *fail, char *work)
{
	HDErrorInfo error;
	if (HD_DEVICE_ERROR(error = hdGetError()))
    {
        hduPrintError(stderr, &error, fail);
        fprintf(stderr, "Press any key to quit.\n");
        getch();
        return true;
    }
	printf(work);
	return false;
}

void goHome(hduVector3Dd position, hduVector3Dd anchor, hduVector3Dd positionPast, hduVector3Dd force)
{
	successCount ++;
	cout << successCount << "\n";
	//vibrate();	
	while (distance3D(position, anchor) >= 15)
	{
		if (stiffness < HI_STIFF)
		{
			if (stiffness*STIFF_SCALE >= HI_STIFF)
			{
				stiffness = HI_STIFF;
			}
			else
			{
				stiffness = STIFF_SCALE*stiffness;
			}
		}
		if (damping < HI_DAMP)
		{
			if (damping*DAMP_SCALE >= HI_DAMP)
			{
				damping = HI_DAMP;
			}
			else
			{
				damping = damping*DAMP_SCALE;
			}
		}

		hdBeginFrame(hdGetCurrentDevice());
				
		positionPast = position;				
		hdGetDoublev(HD_CURRENT_POSITION, position);

		hduVecSubtract(force, anchor, position);
		force[0] = force[0] - damping*distance3D(position,positionPast);
		force[1] = force[1] - damping*distance3D(position,positionPast) + G;
		force[2] = force[2] - damping*distance3D(position,positionPast);

		hduVecScaleInPlace(force, stiffness);

		hdSetDoublev(HD_CURRENT_FORCE, force);

		CURR_TICKS = GetTickCount();
		if (CURR_TICKS - PREV_TICKS > 0)
		{
			trainingData << CURR_TICKS - BASE_TICKS << ",\t";
			trainingData << successCount << ",\t";
			trainingData << position[0] << ",\t";
			trainingData << position[1] << ",\t";
			trainingData << position[2] << ",\t";
			trainingData << force[0] << ",\t";
			trainingData << force[1] << ",\t";
			trainingData << force[2] << "\n";

			PREV_TICKS = CURR_TICKS;
		}
				
		hdEndFrame(hdGetCurrentDevice());

		if (checkQuit())
		{
			stiffness = LO_STIFF;
			damping = LO_DAMP;
			return;
		}
	}

	stiffDown(position,anchor,positionPast,force);
	dampDown(position,anchor,positionPast,force);

	return;
}

void waitForHome(hduVector3Dd position, hduVector3Dd anchor, hduVector3Dd positionPast, hduVector3Dd force)
{
	
	successCount ++;
	cout << successCount << "\n";
	//vibrate();	
	while (distance3D(position, anchor) >= 15)
	{
		hdBeginFrame(hdGetCurrentDevice());
				
		positionPast = position;				
		hdGetDoublev(HD_CURRENT_POSITION, position);

		hduVecSubtract(force, anchor, position);
		force[0] = force[0] - damping*distance3D(position,positionPast);
		force[1] = force[1] - damping*distance3D(position,positionPast) + G;
		force[2] = force[2] - damping*distance3D(position,positionPast);

		hduVecScaleInPlace(force, stiffness);

		hdSetDoublev(HD_CURRENT_FORCE, force);

		CURR_TICKS = GetTickCount();
		if (CURR_TICKS - PREV_TICKS > 0)
		{
			trainingData << CURR_TICKS - BASE_TICKS << ",\t";
			trainingData << successCount << ",\t";
			trainingData << position[0] << ",\t";
			trainingData << position[1] << ",\t";
			trainingData << position[2] << ",\t";
			trainingData << force[0] << ",\t";
			trainingData << force[1] << ",\t";
			trainingData << force[2] << "\n";

			PREV_TICKS = CURR_TICKS;
		}

		hdEndFrame(hdGetCurrentDevice());

		if (checkQuit())
		{
			return;
		}
	}

	printf("Trial done.\n");

	return;
}

void stiffDown(hduVector3Dd position, hduVector3Dd anchor, hduVector3Dd positionPast, hduVector3Dd force)
{
	cout << "Stiff down \n";
	while (stiffness > LO_STIFF)
	{
		stiffness = stiffness/STIFF_SCALE;

		hdBeginFrame(hdGetCurrentDevice());

		positionPast = position;
		hdGetDoublev(HD_CURRENT_POSITION, position);
	
		hduVecSubtract(force, anchor, position);
		force[0] = force[0] - damping*distance3D(position,positionPast);
		force[1] = force[1] - damping*distance3D(position,positionPast) + G;
		force[2] = force[2] - damping*distance3D(position,positionPast);

		hduVecScaleInPlace(force, stiffness);

		hdSetDoublev(HD_CURRENT_FORCE, force);

		CURR_TICKS = GetTickCount();
		if (CURR_TICKS - PREV_TICKS > 0)
		{
			trainingData << CURR_TICKS - BASE_TICKS << ",\t";
			trainingData << successCount << ",\t";
			trainingData << position[0] << ",\t";
			trainingData << position[1] << ",\t";
			trainingData << position[2] << ",\t";
			trainingData << force[0] << ",\t";
			trainingData << force[1] << ",\t";
			trainingData << force[2] << "\n";

			PREV_TICKS = CURR_TICKS;
		}

		hdEndFrame(hdGetCurrentDevice());
	
		if (checkQuit())
		{
			stiffness = LO_STIFF;
			return;
		}
	}

	stiffness = LO_STIFF;
	return;
}

void dampDown(hduVector3Dd position, hduVector3Dd anchor, hduVector3Dd positionPast, hduVector3Dd force)
{
	cout << "Damping down \n";
	while (damping > LO_DAMP)
	{
		damping = damping/DAMP_SCALE;

		hdBeginFrame(hdGetCurrentDevice());

		positionPast = position;
		hdGetDoublev(HD_CURRENT_POSITION, position);

		hduVecSubtract(force, anchor, position);

		hduVecSubtract(force, anchor, position);
		force[0] = force[0] - damping*distance3D(position,positionPast);
		force[1] = force[1] - damping*distance3D(position,positionPast) + G;
		force[2] = force[2] - damping*distance3D(position,positionPast);

		hduVecScaleInPlace(force, stiffness);

		hdSetDoublev(HD_CURRENT_FORCE, force);

		CURR_TICKS = GetTickCount();
		if (CURR_TICKS - PREV_TICKS > 0)
		{
			trainingData << CURR_TICKS - BASE_TICKS << ",\t";
			trainingData << successCount << ",\t";
			trainingData << position[0] << ",\t";
			trainingData << position[1] << ",\t";
			trainingData << position[2] << ",\t";
			trainingData << force[0] << ",\t";
			trainingData << force[1] << ",\t";
			trainingData << force[2] << "\n";

			PREV_TICKS = CURR_TICKS;
		}

		hdEndFrame(hdGetCurrentDevice());

		if (checkQuit())
		{
			damping = LO_DAMP;
			return;
		}
	}

	damping = LO_DAMP;
	return;
}
/*

*/