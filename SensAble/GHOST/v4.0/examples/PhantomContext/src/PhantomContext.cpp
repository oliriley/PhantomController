//*****************************************************************************
//      Filename : PhantomContext.cpp
//   Authored by : Brandon Itkowitz (bitkowitz@sensable.com)
// Last Modified : 08/21/99
// ----------------------------------------------------------------------------
// SensAble Technologies, Inc. Copyright © 1999
// All rights reserved.
//*****************************************************************************

#include "gstAfx.h"
#include "PhantomContext.h"
#include "PhantomContext_graphics.h"
#include "PhantomContext_haptics.h"

//*****************************************************************************
// Function : start_program
// ------------------------
// Description : Issues the necessary initialization routines
// Return type : void 
// Argument #1 : void
//*****************************************************************************
void start_program(int bResetPHANToM)
{
    static int bfirstTime = TRUE;
    gstScene *myScene = NULL;

    // Initialize haptics and start haptics
    if (bfirstTime) {
        myScene = init_haptics(bResetPHANToM);
        init_graphics(myScene);
        bfirstTime = FALSE;
    }
}

//*****************************************************************************
// Function : end_program
// ----------------------
// Description : Called at program end. Do cleanup
// Return type : void 
// Argument #1 : void
//*****************************************************************************
void end_program(void)
{
    // Terminate haptics
    term_haptics();
}


