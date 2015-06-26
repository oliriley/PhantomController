// gstPHANToMSelect.cpp : add the ability to send SELECT events to gstPHANToM

#include "gstAfx.h"
#include "gstPHANToMSelect.h"

gstType	gstPHANToMSelect::gstPHANToMSelectClassTypeId;
const gstEventType gstPHANToMSelect::SELECT = gstNextUniqueId();

gstPHANToMSelect::gstPHANToMSelect(
    char *configFile,
    int resetEncoders) :
    
    gstPHANToM(configFile, resetEncoders),
    pressed(FALSE)
{}


int gstPHANToMSelect::setForce_WC(const gstVector &force_WC, const gstVector &torque_WC)
{
    if (getStylusSwitch()) {
        if (!pressed) {
              // the pressed variable makes sure we send only one event per
              // button press
            pressed = TRUE;
              // send one SELECT event for each object we are in contact with
            for (int i = 0; i < getLastNumCollisionObjs(); i++) {
                gstShape *obj = getLastCollisionInfo()[i].obj;
                assert(obj);
                gstEvent event;
                event.id = SELECT;
                event.data1v = (void *)obj;
                addEvent(event);
            }
        }
    } else if (pressed) {
        pressed = FALSE;
    }
    
    return gstPHANToM::setForce_WC(force_WC, torque_WC);
}
