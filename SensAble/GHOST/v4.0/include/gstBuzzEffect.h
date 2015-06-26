//=============================================================================
//   Filename : gstBuzzEffect.h
//    Project : GHOST library v3.0
//    Authors : Chris Tarr
//    Revised : W. A. Aviles
// ----------------------------------------------------------------------------
// Description: Buzz effect to cause the PHANToM to buzz/vibrate
//
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#ifndef GST_BUZZ_EFFECT
#define GST_BUZZ_EFFECT

#include <gstBasic.h>
#include <math.h>
#include <gstDllExport.h>
#include <gstEffect.h>
#include <gstErrorHandler.h>

// Buzz effect for PHANToM.  This effect
// vibrates the PHANToM end point along the y-axis 
// with a given frequency, amplitude, and duration.
class GHOST_DLL_IMPORT_EXPORT gstBuzzEffect : public gstEffect 
{

public:

	// Constructor.
	gstBuzzEffect():gstEffect(){
		frequency = 1000.0;
		duration = 2.0;
		amplitude = 1.0;
	}

	// Destructor.
	~gstBuzzEffect() {}

	// Set duration of effect [seconds].
	void	setDuration(double durInSec) {
		if (durInSec <= 0) {
			gstErrorHandler(GST_BOUNDS_ERROR,
				"gstBuzzEffect:setDuration Invalid Duration <= 0",durInSec);
			return;
		}
		duration = durInSec; 
	}

	// Get duration of effect [seconds].
	double 	getDuration() { return duration; }

	// Set "buzz" frequency [Hz].
	void	setFrequency(double newFreq) {
		if (newFreq <= 0) {
			gstErrorHandler(GST_BOUNDS_ERROR,
				"gstBuzzEffect:setFrequency Invalid Frequency <= 0",newFreq);
			return;
		}
		frequency = newFreq; 
	}

	// Get "buzz" frequency [Hz].
	double 	getFrequency() { return frequency; }

	// Set amplitude of effect [millimeters].
	void	setAmplitude(double newAmp) {
		if (newAmp < 0) {
			gstErrorHandler(GST_BOUNDS_ERROR,
				"gstBuzzEffect:setAmplitude Invalid Amplitude < 0",newAmp);
			return;
		}
		amplitude = newAmp; 
	}

	// Get amplitude of effect [millimeters].
	double 	getAmplitude() { return amplitude; }

	
	// FOR_GHOST_EXTENSION:
	// Caculate the force.  Force is returned in parent 
	// reference frame of phantom. When subclassing, the first 
	// parameter should be cast to gstPHANToM to retrieve 
	// any information about the state of the PHANToM that 
	// is needed to calculate the forces.  deltaT should 
	// be used to update the time.  Also, if the effect is
	// not active, the zero vector should be returned.
	// ACTUNG!
	// WARNING!: Never call PHANToM->setForce or
	//			PHANToM->setForce_WC from this function.
	//			It will cause an infinite recursion.
	virtual gstVector	calcEffectForce(void *phantom);
	virtual gstVector	calcEffectForce(void *phantom, gstVector &torques) {
		torques.init(0.0, 0.0, 0.0);
		return calcEffectForce(phantom);
	}

protected:

	double		frequency, amplitude, duration;

};



#endif // GST_BUZZ_EFFECT
