//=============================================================================
//   Filename : gstSlider.h
//    Project : GHOST library v3.0
//    Authors : Chris Tarr
//    Revised : W. A. Aviles
// ----------------------------------------------------------------------------
// Description: Dynamic class for simulating a physical slider
//
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#ifndef GST_SLIDER
#define GST_SLIDER

#define SLIDER_NOTCHES 4
#define SLIDER_DISTANCE 100.0
#define SLIDER_INIT_NOTCH 0
#define SLIDER_K 0.004

#define SLIDER_MASS 0.5
#define SLIDER_DAMPING 2.0

#include <gstDllExport.h>
#include <gstDynamic.h>

// Slider dynamic class.  The slider translates in the z=0 plane
// and is, initially, located at the origin oriented along
// the X axis. The slider has notches (or detents), a moment 
// of intertia, a damping coefficient, a throw distance, and 
// a spring constant representing the force needed to leave a notch. 
// Notches are placed evenly along the slider's
// extent (or throw distance).  Methods to set the number of notches, 
// the sliders's location, throw distance, and the spring constant are
// available.  Other state variables (e.g., damping 
// coefficient) are inherited  from the base class, gstDynamic.
class GHOST_DLL_IMPORT_EXPORT gstSlider : public gstDynamic
{
    GHOST_INHERIT_HEADER(gstSlider, gstDynamic);

public:

    static const gstEventType UP;
    static const gstEventType DOWN;

	// Constructor.
	gstSlider();

	// Destructor.
	virtual ~gstSlider() {}

	// Copy Constructor.
	gstSlider(const gstSlider &);

	// Clone.
	virtual gstNode *Clone() const {return CloneSlider();}
	gstSlider *CloneSlider() const {return (new gstSlider(*this));}

	// Returns integer ID of nearest notch.
	int getClosestNotch();

	// Get current state of object.
    gstEvent getEvent();

	// Set the number of notches (minimum is two--one at each end
	// of slider).
	void setNumberNotches(int n);

	// Set throw (travel) distance of slider [millimeters].
	void setDistance(double dist);

	// Set initial notch location for slider.
	void setInitialNotch(int newNotch);
  
	// Set notch spring constant [Kg/(1000.0*sec^2)].
	void setK(double _K) { K = _K; }

	// Get current notch.
	int getNotch() {return notch;}

	// Get notch spring constant.
	double getK() const { return K; }

	// Get number of notches in slider.
	int getNumberNotches() const { return number_notches; }

	// Get slider throw (travel) distance.
	double getDistance() const { return distance; }

	// Get initial notch.
	int getInitialNotch() const { return initial_notch; }


gstInternal public:

	// FOR_GHOST_EXTENSION
	// Used by system or for creating sub-classes only.
	// Performs Euler integration and updates state of slider.
	void updateDynamics();

protected:

	int number_notches;
	double distance; // total distance traversable by slider
	int initial_notch;
	int	notch;
	double K; 
	double sectionLength;
};

#endif
