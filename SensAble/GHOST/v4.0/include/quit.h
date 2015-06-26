/*_______________________________________________________________________________
__________________________ SensAble Technologies, Inc. __________________________
_________________________________________________________________________________
__                                                                             __
__      File:		        quit                                           __
__      Author:                 Loren Shih                                     __
__      Copyright 1997-1999, Sensable Technlologies, Inc.                      __
__                                                                             __
__-----------------------------------------------------------------------------__
__                                                                             __
__      File type: C++ header                                                  __
__      Libraries: OpenGL, GHOST                                               __
__      Platforms: SGI, Windows NT 3.51 or 4.0                                 __
__                                                                             __
__-----------------------------------------------------------------------------__
__                        
__      DESCRIPTION: This library contains functions that implement
__      a visual and haptic quit button.  
__      
__      DYNAMIC PROPERTIES: The quit button is a fairly firm, clickable 
__      button.  When pressed, it stops the servo loop.
__      SURFACE PROPERTIES: The quit button has standard material properties.
__      
_________________________________________________________________________________
_______________________________________________________________________________*/

class gstPoint;
class gstScene;
class gstSeparator;

void create_quit_button(float width);
void draw_quit_button();

gstSeparator *create_quit_buttonH(
    float width,
    gstPoint &position,
    gstScene *scene);


