// gstPHANToMSelect.h
//      Adds the ability to send SELECT events from gstPHANToM.
//      SELECT events are generated when the stylus is pressed
//      for each shape the phantom is in conact with.
//      A pointer to the selected shape is passed in the .data1v
//      field of the event structure.
//
//      Possible enhancements gstPHANToMSelect could use:
//          - send events for pressing/releasing the stylus
//            (even if not in contact with anything)
//          - Send SELECTDOWN/SELECTUP (or something like that) events
//            to differentiate between pressing and releasing the button
//            since some receivers of the events might only want to take
//            action when the button is released


class gstPHANToMSelect : public gstPHANToM
{
    public:
        static const gstEventType SELECT;

        gstPHANToMSelect(
            char *configFile,
            int resetEncoders = TRUE);

          // we override setForce_WC() because it is called every server loop
        virtual int setForce_WC(
            const gstVector &force,
            const gstVector &torque);


          // boilerplate typeId functions
        static gstType getClassTypeId()
        {
	        gstAssignUniqueId(gstPHANToMSelectClassTypeId);
	        return gstPHANToMSelectClassTypeId;
        }

        virtual gstType	   getTypeId() const { return getClassTypeId(); }
        virtual gstBoolean isOfType(gstType type) const {return staticIsOfType(type);}
        static gstBoolean staticIsOfType(gstType type)
        {
	        if (type == getClassTypeId()) return TRUE;
	        else return (gstPHANToM::staticIsOfType(type));
        }

    private:
        gstBoolean pressed;

        static gstType gstPHANToMSelectClassTypeId;
};
