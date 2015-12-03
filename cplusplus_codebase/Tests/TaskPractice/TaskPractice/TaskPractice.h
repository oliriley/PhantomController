/*	Exploring possible task setups
	Start date	08 Oct 2015
	Author		Oliver Stanley
	Group		Moritz lab, UW Dept. Rehab Medicine	*/

#ifdef  _WIN64
#pragma warning (disable:4996)
#endif

#if defined(WIN32)
# include <windows.h>
# include <conio.h>
#else
# include "conio.h"
# include <stdlib.h>
# include <ctype.h>
# include <string.h>
#endif

#include <HL/hl.h>
#include <GL/glut.h>
#include <HD/hd.h>
#include <HDU/hduVector.h>
#include <HDU/hduError.h>

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <vector>

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

const float STIFF = 0.03;
const float DAMP = 0.01;
const float G = 0.012;
const float SHELL_RADIUS = 35;
const float INTERDICT = 20; //make sure this is less than SHELL_RADIUS

bool checkQuit();
bool errReport(char *fail, char *work);

float distance3D(hduVector3Dd position, hduVector3Dd anchor);

std::string getRat();

void hapticLoop();
void checkSuccess(hduVector3Dd position, hduVector3Dd anchor);
void writeData(hduVector3Dd position, hduVector3Dd force);

