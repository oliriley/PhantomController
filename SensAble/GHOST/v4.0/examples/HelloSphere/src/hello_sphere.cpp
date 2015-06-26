//=============================================================================
//     Filename : HelloSphere.cpp
//   Written by : Abbe Cohen
//     Contains : Example GHOST Application using gstDeviceIO.h functions
// ----------------------------------------------------------------------------
// This program demonstrates how to use the GHOST DeviceIO API functions
// to touch a sphere without using the higher-level GHOST scene graph, sphere
// primitives, etc.
//
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-2002 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#include <gstPHANToM.h>
#include <iostream.h>
#include <gstDeviceIO.h>


typedef struct _servoLoopData 
{
    int id;
    float maxStiffness; 
    bool useLowLevel;
} servoLoopData;

void collisionDetectSphere(gstPoint& pos,
                           gstPoint& spherePos,
                           float sphereRadius,
                           float sphereStiffness,
                           gstVector& force);

GST_SCHEDULER_CALLBACK  servoLoopSphere(void *pUserData);

void processInputUntilQuit (int stylusSwitch, 
                            bool forcesEnabled, 
                            servoLoopData& data, 
                            int phantomID);
void printStartupMessages(int phantomID);



main()
{

    int phantomID;
    
    // Initialize the Phantom (// name is the string from Phantom Configuration)
    phantomID = gstInitializePhantom("Default PHANToM");
    
    // Check for success of initialization:
    if (phantomID < 0) 
    {
        cout << "Error initializing Phantom: Error code " << phantomID << endl;
        return -1;
    }


    cout << "Phantom successfully initialized." << endl;

    // Reset the encoders, if needed
    if( gstIsPhantomResetNeeded(phantomID) )
    {
        cout << "Place the PHANToM in its reset position and press <ENTER>." << endl;
        cin.get();


        if( gstResetPhantomEncoders(phantomID) != PHTM_OK )
        {
            gstDisablePhantom(phantomID);
            cout << "phantom reset error" << endl;
            return -1;
        }
    }


    // Enable Phantom forces
    bool forcesEnabled = false;

    if (gstEnablePhantomForces(phantomID) != PHTM_OK) 
    {
        gstDisablePhantom(phantomID);
        cout << "phantom reset error" << endl;
        return -1;
    }

    forcesEnabled = true;
    int stylusSwitch = gstGetStylusSwitchState(phantomID);

    // set up the servo loop scheduler
    if (gstInitServoScheduler() == PHTM_OK)
    {
        // start the servo loop
        // servoLoopUpdate is the gstServoSchedulerCallback function that
        // will get called at the appropriate update rate, until it's stopped
        // sending.
        servoLoopData data;
        data.id = phantomID;
        data.maxStiffness = gstGetPhantomMaxStiffness(phantomID);
        data.useLowLevel = false;

        gstStartServoScheduling(servoLoopSphere, &data);
        

        
        printStartupMessages(phantomID);
        processInputUntilQuit(stylusSwitch, forcesEnabled, data, phantomID);
        return 0;
 
    }
    else
        return -1;
}


void printStartupMessages(int phantomID)
{
    gstPHANToMInfoStruct phantomInfo;
    gstGetPhantomInfo(phantomID, phantomInfo);
    
    if ( phantomInfo.is6DOF )
            cout << "Welcome to Hello Sphere with a 6dof Phantom model." << endl;
        else if ( phantomInfo.isDesktop )
            cout << "Welcome to Hello Sphere with a Phantom Desktop model." << endl;
        else 
            cout << "Welcome to Hello Sphere with a  Phantom Premium model." << endl;
        
        cout << "Press <q> to quit" << endl;
        cout << "Press <g> to show gimbal angles" << endl;
        cout << "Press <j> to show joint angles" << endl;
        cout << "Press <e> to toggle to raw-encoder based servo loop" << endl;
        cout << "Press <t> to show motor temperatures" << endl;
        cout << "Press <m> to show endpoint matrix" << endl;
        cout << "press <f> to toggle Phantom forces" << endl;
        cout << "press <v> to show phantom velocity" << endl;
        cout << "press <u> to show phantom instantaneous update rate" << endl;
    //    cout << "press <s> to show phantom error status" << endl;
        cout << "Press <ENTER> after your choice" << endl;
}

void processInputUntilQuit(int stylusSwitch, bool forcesEnabled, servoLoopData& data, int phantomID)
{
    char nextKey;
    cin.get(nextKey);
    while (nextKey != 'q')
    {   
        switch( nextKey )
        {
        case 'g':
            {
                    gstVector gimbals;
                    gstGetStylusJointAngles(phantomID, gimbals);
                    cout << "gimbal yaw: " << gimbals.x() 
                        << " gimbal pitch: " << gimbals.y() 
                        << " gimbal roll: " << gimbals.z() << endl;
                    break;
            }
        case 'j':
            {
                    gstVector joints;
                    gstGetJointAngles(phantomID, joints);
                    cout << "x (left-right): " << joints.x()
                        << " y (up-down): " << joints.y()
                        << " z (in-out): " << joints.z() << endl;
                    break;
            }
        case 'm':
            {
                gstTransformMatrix m;
                gstGetStylusMatrix(phantomID, m);
                m.printSelf();
                break;
            }
        case 't':
            {
                float temp[6];
                gstGetPhantomTemperature(phantomID, temp);
                cout << "motor 1: " << temp[0]
                    << " motor 2: " << temp[1]
                    << " motor 3: " << temp[2] << endl;
                cout << "motor 4: " << temp[3]
                    << " motor 5: " << temp[4]
                    << " motor 6: " << temp[5] << endl;
                break;
            }
        case 'v':
            {
                gstVector vel;
                gstGetPhantomVelocity(phantomID, vel);
                cout << "x velocity: " << vel.x()
                    << " y velocity: " << vel.y()
                    << " z velocity: " << vel.z() << endl;
                break;
            }
        case 'u':
            {
                cout << "updateRate: " << gstGetPhantomUpdateRate(phantomID) 
                    << endl;
                break;
            }
        //case 's':
         //   {
           //     cout << "error status: " << gstGetPhantomError(phantomID)
             //       << endl;
               // break;
            //}

        case 'e':
            {
                // stop the old servo and start a new one
                cout << "Stopping servo loop" << endl;
                gstStopServoScheduling();
                
                // toggle useLowLevel;
                data.useLowLevel = !data.useLowLevel;
                cout << "Starting new servo loop " 
                    << ( data.useLowLevel ? "with " : "without " )
                    << "low-level encoder based pos calculations" << endl;
                gstStartServoScheduling(servoLoopSphere, &data);
                break;
            }
        case 'f':
            {
                forcesEnabled = !forcesEnabled;
                if (forcesEnabled)
                {
                    cout << "Enabling forces!" << endl;
                    gstEnablePhantomForces(phantomID);
                    gstStartServoScheduling(servoLoopSphere, &data);
                }
                else 
                {
                    cout << "Disabling forces!" << endl;
                    gstDisablePhantomForces(phantomID);
                    gstStopServoScheduling();
                }
                
                
            }
        default:
            break;
        }
        
        cin.get(nextKey);
        
    }
    // stop the servo loop when 'q' key is hit.
    
    gstStopServoScheduling();

}


// ServoLoopUpdate
// Simple servo loop that reads Phantom state and sends a reaction force
// based on intersection with a sphere.  Also prints out random information

GST_SCHEDULER_CALLBACK servoLoopSphere(void *pUserData)
{
    int status=0;

    servoLoopData data = * ( (servoLoopData*)pUserData );
    int phantomID = data.id;
    float maxStiffness = data.maxStiffness;
    bool useLowLevelPos = data.useLowLevel;

    // set sphere position, radius, etc
    static gstPoint spherePos(20.0, 0.0, 0.0);
    static double sphereRadius = 50.0;
    static double sphereStiffness = 0.8;
    static int stylusSwitch = 0;

    // echo when stylus state changes.
    int newStylusSwitch = gstGetStylusSwitchState(phantomID);
    if ( newStylusSwitch != stylusSwitch )
    {
        cout << "Stylus switch press/release detected" << endl;
        stylusSwitch = newStylusSwitch;
    }


    // update the Phantom for this servo cycle so we can
    // read positions, etc.
    status = gstUpdatePhantom(phantomID);
    if (status == PHTM_OK)
    {
        gstPoint phantomPos;
        gstVector phantomForce;

        // test low level position
        if (useLowLevelPos)
        {
            long encoders[6];
            gstGetRawEncoderValues(phantomID, encoders);
           

            // you could implement your own filter or change the encoder here.
            // We're just demonstrating this part of the gstDeviceAPI, so we
            // won't change anything here.

            gstTransformMatrix endpointTransform;
            gstEncodersToTransform(phantomID, encoders, endpointTransform);
            
            // we only care about the position.
            endpointTransform.getTranslation(phantomPos);
        }
        else
            gstGetPhantomPosition(phantomID, phantomPos);

        // calculate force based on phantomPos
        collisionDetectSphere(phantomPos,
            spherePos,
            sphereRadius,
            sphereStiffness*maxStiffness,
            phantomForce);

        status = gstSetPhantomForce(phantomID, phantomForce);
        if (status != PHTM_OK)
        {
            // this disable is actually redundant, as gstSetPhantomForce
            // is safe and will disable them for you if any Phantom Errors
            // occur, but we are demonstrating how to disable forces here
            // just for the hell of it.
            gstDisablePhantomForces(phantomID);
            cout << "Phantom forces disabled due to Phantom Error!" << endl;
            cout << "Error Code: " << status << endl;
        }
    }
    else
    {
        cout << "Phantom forces disabled due to Phantom Error!" << endl;
        cout << "Error Code: " << status << endl;
    }


    return status;
}




//   collisionDetectSphere()
//   If phantom position intersects the sphere the appropriate 
//   reaction force is returned in the array force.
//   Otherwise force will be zero.

void collisionDetectSphere(gstPoint& pos,
                           gstPoint& spherePos,
                           float sphereRadius,
                           float sphereStiffness,
                           gstVector& force)
{

    // Compute vector direction and distance from center of sphere to phantom.

    gstVector sphereCenterToPhantomPos(pos - spherePos);
    double distFromCenter = sphereCenterToPhantomPos.norm();
    sphereCenterToPhantomPos.normalize();

    // if magnitude of dispFromCenter is less than sphere radius
    // then the Phantom is inside the sphere.
    if ( distFromCenter < (double)sphereRadius )
    {
        // Apply a force proportional to how much the Phantom is embedded
        // in the sphere. 
        double forceMag = sphereStiffness*(sphereRadius - distFromCenter);

        // We want this force to be radially out from the center of the sphere,
        // ie, along the sphereCenterToPhantomPos vector.
        force = forceMag*sphereCenterToPhantomPos;
    }
    else
    {
        force = gstVector(0.0, 0.0, 0.0);
    }
    
}

