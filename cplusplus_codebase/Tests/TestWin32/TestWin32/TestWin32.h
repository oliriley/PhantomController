/*	Test of setup for Win32 Console (GLUT) application
	Start date	26 July 2015
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

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <math.h>

#include <iostream>
#include <fstream>
#include <string>

#include <HD/hd.h>
#include <HL/hl.h>
#include <HDU/hduVector.h>
#include <HDU/hduError.h>
#include <GL/glut.h>

const float HI_STIFF = 0.05;
const float LO_STIFF = 0.01;
const float HI_DAMP = 0.1;
const float LO_DAMP = 0.01;
const float G = 0.012;
const float STIFF_SCALE = 1.02;
const float DAMP_SCALE = 1.05;
const float SUCCESS_BARRIER = -70;

bool checkQuit();
bool checkSuccess(hduVector3Dd position);
bool errReport(char *fail, char *work);

float distance3D(hduVector3Dd position, hduVector3Dd anchor);

std::string getRat();

void hapticLoop();
void goHome(hduVector3Dd position, hduVector3Dd anchor, hduVector3Dd positionPast, hduVector3Dd force);
void stiffDown(hduVector3Dd position, hduVector3Dd anchor, hduVector3Dd positionPast, hduVector3Dd force);
void dampDown(hduVector3Dd position, hduVector3Dd anchor, hduVector3Dd positionPast, hduVector3Dd force);


