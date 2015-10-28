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

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include <iostream>
#include <fstream>
#include <string>

#include <HD/hd.h>
#include <HL/hl.h>
#include <HDU/hduVector.h>
#include <HDU/hduError.h>
#include <GL/glut.h>

using namespace std;

