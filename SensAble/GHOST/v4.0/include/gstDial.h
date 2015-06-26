//=============================================================================
//   Filename : gstDial.h
//    Project : GHOST library v3.0
//    Authors : Chris Tarr
//    Revised : W. A. Aviles
// ----------------------------------------------------------------------------
// Description: Dynamic class for simulating a physical dial
//
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#ifndef GST_DIAL
#define GST_DIAL

#include <gstDllExport.h>
#include <gstDynamic.h>


#define DIAL_NOTCHES 6
#define DIAL_INIT_NOTCH 0
#define DIAL_K 0.06

#define DIAL_MOMENT 0.0018
#define DIAL_DAMPING 0.02


// Dial dynamic class.  The dial rotates in the z=0 plane
// and is by default located at the origin. The dial
// has notches (detents), a moment of inertia, 
// a damping coefficient, and a spring constant associated
// with the amount of force needed to leave a notch.  
// Notches are placed evenly spaced from 0-359 degrees with
// notch 0 starting at 0 degrees.  Methods
// to set the number of notches, the dial's location
// relative to these notches, and the spring constant are 
// available in this class.  Other state variables (e.g., damping 
// coefficient) are inherited from the base class, gstDynamic.
// gstDial produces a notch event each time the dial moves into
// a new notch. The id field of the gstEvent structure passed
// the event callback is assigned the current notch and the
// data1d integer field specifies the direction the dial came
// from by assigning the field GST_COUNTERCLOCKWISE or
// GST_CLOCKWISE.
class GHOST_DLL_IMPORT_EXPORT gstDial : public gstDynamic
{
    GHOST_INHERIT_HEADER(gstDial, gstDynamic);

public:

    static const gstEventType CLOCKWISE;
    static const gstEventType COUNTERCLOCKWISE;    

	// Constructor.
	gstDial();

	// Destructor.
	virtual ~gstDial() {}

	// Copy Constructor.
	gstDial(const gstDial &);

	// Clone.
	virtual gstNode *Clone() const {return CloneDial();}
	gstDial *CloneDial() const {return (new gstDial(*this));}

	// Get current state of object.
	gstEvent			getEvent();
  
	// Set the number of notches (detents).
	// Notches are placed evenly spaced from 0-359 degrees with
	// notch 0 starting at 0 degrees.
 	void				setNumberNotches(int n);

	// Get number of notches (detents).
	int					getNumberNotches() const { return number_notches; }

	// Set initial notch (angular orientation) of dial.
	void				setInitialNotch(int initNotch);
  
	// Get initial notch (angular orientation) of dial.
	int					getInitialNotch() const { return initial_notch; }

	// Set spring constant of spring that is holding dial in current notch [Kg/(1000.0*sec^2)].
	void				setK(double _K) { K = _K; }

	// Get spring constant of spring that is holding dial in current notch [Kg/(1000.0*sec^2)].
	double				getK() const { return K; }


gstInternal public:
	
	// FOR_GHOST_EXTENSION:
	// Used by system or for creating sub-classes only.
	// When button has velocity or a force, this method
	// does Euler integration of button state and
	// creates button events to be processed by
	// gstTransform::updateEvents().
	void updateDynamics();

protected:

	int number_notches, currentNotch;
	int initial_notch;
    gstEventType direction;
	double K, orientation;
	double notch;
	double sectionTheta;  // Angle of each sectionTheta
};
 

#endif
