
/*	Experimenting with different task setups
	Start date	08 Oct 2015
	Author		Oliver Stanley
	Group		Moritz lab, UW Dept. Rehabilitation Medicine	*/

#include <TaskPractice.h>

//Output file
ofstream trainingData(getRat(),ios::ate|ios::app);
//Maximum distance handle has moved from the home position
int max_distance = 0;
//Number of successes completed
int successCount = 0;
//How many radii the handle has to move to count as a success
// 1 by default
int shells = 1;

//Track previous successful points
std::vector<hduVector3Dd> prevSuccess;

//Qualities of haptic force:
//Spring constant for restoring force
float stiffness = STIFF;
//Damping constant to prevent too-rapid movement
float damping = DAMP;

//These monitor the time between updates
long BASE_TICKS = GetTickCount();
long PREV_TICKS = BASE_TICKS;
long CURR_TICKS;

int main(int argc, char *argv[])
{
	//Open output file
	if (!trainingData.is_open())
	{
		printf("Problem opening training data. Please retry.\n");
		_getch();
		return -1;
	}
	trainingData << ("% Columns: Time (msec)---Successes---x---y---z---x-force---y-force---z-force\n");
	
	//Initialize haptic device
	HHD hHD = hdInitDevice(HD_DEFAULT_DEVICE);

	//Make sure nothing broke
	if (errReport("Failed to initialize haptic device.\n","Device successfully initialized.\n"))
	{
		return -1;
	}

	//Turn on haptic force feedback
	hdEnable(HD_FORCE_OUTPUT);
	printf("Force output enabled.\n");

	//Start thread for reading position and applying force
	hdStartScheduler();
	
	//Make sure nothing broke
	if (errReport("Failed to start scheduler.\n","Scheduler started.\n"))
	{
		return -1;
	}

	//Create the haptic environment, monitor the device and apply forces
	hapticLoop();

	//Once the program is closed, close down all the things started earlier
	hdStopScheduler();
	hdDisable(HD_FORCE_OUTPUT);
	hdUnschedule(HD_CALLBACK_DONE);
	hdDisableDevice(hdGetCurrentDevice());

	//Wrap up and report to the user
	printf("All done!\n");
	_getch();
	return 0;
}

//This is the function that actually monitors the device and applies forces
void hapticLoop()
{
	//An array to be rewritten repeatedly with
	//new forces depending on the situation
	hduVector3Dd force;
		force[0] = 0;
		force[1] = 0;
		force[2] = 0;

	//Write a blank vector to previous successes to compare to
	prevSuccess.push_back(force);

	//Arrays to take in the handle position 
	//in 3D, storing one previous value
	//to monitor speed
	hduVector3Dd position;
		position[0] = 0;
		position[1] = 0;
		position[2] = 0;
	hduVector3Dd positionPast;

	//anchor is where the handle will return to, where [0,0,0]
	//represents the point the device was last calibrated to
	hduVector3Dd anchor;
		anchor[0] = 0;
		anchor[1] = 0;
		anchor[2] = 0;

	while(true)
	{
		//A 'frame' is a block of code within which
		//all device properties are assumed static
		hdBeginFrame(hdGetCurrentDevice());

		//Track position before updating
		positionPast = position;		
		
		//Write the current device coordinates to 'position'
		hdGetDoublev(HD_CURRENT_POSITION, position);
		
		//Check success condition(s)
		checkSuccess(position,anchor);

		//Write the distance from the home position to 'force'
		//then scale it by the stiffnessm and add damping
		//and an anti-gravity term
		hduVecSubtract(force, anchor, position);
		float dist = distance3D(position,positionPast);
		//Determine whether to have each direction +/-
		float signx = 2*((position[0]>0)-0.5);
		float signy = 2*((position[1]>0)-0.5);
		float signz = 2*((position[2]>0)-0.5);

		force[0] = force[0]*stiffness - damping*signx*dist;
		force[1] = force[1]*stiffness - damping*signy*dist + G;
		force[2] = force[2]*stiffness - damping*signz*dist;

		//Write the resulting 'force' vector to the device and output file
		hdSetDoublev(HD_CURRENT_FORCE, force);
		
		writeData(position,force);
		
		//At the end of the frame, device properties will (/can?) change again
		hdEndFrame(hdGetCurrentDevice());

		//See if the user has instructed the program to quit
		if (checkQuit())
		{
			return;
		}
	}
}

//Based on position, has the task been completed?
void checkSuccess(hduVector3Dd position, hduVector3Dd anchor)
{
	if (distance3D(position,anchor) >= shells*SHELL_RADIUS)
	{
		bool succeed = true;
		for (int j=0; j < prevSuccess.size(); j++)
		{
			if (distance3D(position,prevSuccess[j]) < INTERDICT)
			{
				succeed = false;
			}
		}
		if (succeed)
		{
			successCount ++;
			cout<< "Shell size: " << shells << '\n' << "Success: " << successCount << '\n';
			prevSuccess.push_back(position);
		}
	}
	shells = ceil(distance3D(position,anchor) / SHELL_RADIUS);
}

//Check whether the user has indicated to stop the program
bool checkQuit()
{
	if (_kbhit())
	{
		int keypress = _getch();
		keypress = toupper(keypress);
            
		if (keypress == 'Q')
		{
			printf("You quitter.\n");
			return true;
		}
	}
	return false;
}

//Add information about current device state to the output file
void writeData(hduVector3Dd position, hduVector3Dd force)
{
	CURR_TICKS = GetTickCount();
	if (CURR_TICKS - PREV_TICKS > 0)
	{
		trainingData << CURR_TICKS - BASE_TICKS << ",\t" << successCount << ",\t" << position[0] << ",\t" << position[1] << ",\t" << position[2] << ",\t" << force[0] << ",\t" << force[1] << ",\t" << force[2] << "\n";
		PREV_TICKS = CURR_TICKS;
	}
}

//Get information from user to create file name
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

//Calculate the distance between two points in 3D
float distance3D(hduVector3Dd position, hduVector3Dd anchor)
{
	float xdist = pow(position[0] - anchor[0],2);
	float ydist = pow(position[1] - anchor[1],2); 
	float zdist = pow(position[2] - anchor[2],2);\
	return sqrt(xdist + ydist + zdist);
}

//Check if something has gone wrong in setup
bool errReport(char *fail, char *work)
{
	HDErrorInfo error;
	if (HD_DEVICE_ERROR(error = hdGetError()))
    {
        hduPrintError(stderr, &error, fail);
        fprintf(stderr, "Press any key to quit.\n");
        _getch();
        return true;
    }
	printf(work);
	return false;
}