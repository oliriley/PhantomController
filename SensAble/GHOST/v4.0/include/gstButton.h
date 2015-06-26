//=============================================================================
//   Filename : gstButton.h
//    Project : GHOST library v3.0
//    Authors : Chris Tarr
//    Revised : W. A. Aviles
// ----------------------------------------------------------------------------
// Description: Dynamic class for simulating a physical button 
//
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#ifndef GST_BUTTON
#define GST_BUTTON

#include <gstDllExport.h>
#include <gstDynamic.h>

#define BUTTON_RESTFORCE 0.1
#define BUTTON_DEADBAND 5.0
#define BUTTON_THROWDIST 20.0
#define BUTTON_K 0.00007

#define BUTTON_MASS 0.5
#define BUTTON_DAMPING 0.5


//  Dynamic button class. This creates a button 
//  which is aligned down the z-axis, with
//  the nominal position (resting) at the origin.  The button can be
//  pushed from the origin along the negative z-axis
//  until the end of travel (-throwDist).
//  For a small region (deadband) before the end of
//  travel, the button spring becomes inactive and only
//  the much smaller restoring force is used.  This produces
//  a feeling of a soft click when the button is pushed into
//  the deadband region.
//  If the user releases the button, it should move 
//  back to the origin.
//  gstButton produces a pressed or released event when the
//  button transitions between these two states.  The id field
//  of the gstEvent structure passed into the event callback is
//  assigned gstButton::PRESSED or gstButton::RELEASED for the 
//  corresponding event.

class GHOST_DLL_IMPORT_EXPORT gstButton : public gstDynamic
{
    GHOST_INHERIT_HEADER(gstButton, gstDynamic);

public:

    static const gstEventType PRESSED;
    static const gstEventType RELEASED;

	// Constructor.
	gstButton();

	// Destructor.
	virtual ~gstButton() {}

	// Copy Constructor.
	gstButton(const gstButton &);

	// Clone.
	virtual gstNode *Clone() const {return CloneButton();}
	gstButton *CloneButton() const {return (new gstButton(*this));}

	// Set spring constant [Kg/(1000.0*sec^2)].
	void	setK(double _K) { K = _K; }
	
	// Set restoring force [Newtons] to push button out of the deadband
	// region.  This force is only active when the button is in the 
	// deadband region.
	void	setRestoringForce(double _restoringForce) { restoringForce = _restoringForce; }

	// Set deadband [millimeters].
	void	setDeadband(double _deadband) { deadband = _deadband; }

	// Set throw distance [millimeters].
	void	setThrowDist(double _throwDist) { throwDist = _throwDist; }

	// Get spring constant [Kg/(1000.0*sec^2)].
	double	getK() const { return K; }

	// Get restoring force [Newtons].
	double	getRestoringForce() const { return restoringForce; }

	// Get deadband [millimeters].
	double	getDeadband() const { return deadband; }

	// Get throw distance [millimeters].
	double	getThrowDist() const { return throwDist; }

	// Get current state of object.
	gstEvent	getEvent() {
		gstEvent event;
		event.id = currentState;
		return event;
	}
  

gstInternal public:
	// FOR_GHOST_EXTENSION:
	// Used by system or for creating sub-classes only.
	// When button has velocity or a force, this method
	// does Euler integration of button state and
	// creates button events to be processed by
	// gstTransform::updateEvents().
	void updateDynamics();

protected:

	double	K;				// Spring constant for button.

	double	restoringForce;	// Small restoring force used to push
				            // button through deadband.

	double	deadband;		// Length of deadband at end of
                            //  button throw.

	double	throwDist;		// Distance button moves.

	gstEventType currentState;
};

#endif
