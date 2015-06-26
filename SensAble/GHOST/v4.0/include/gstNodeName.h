//=============================================================================
//   Filename : gstNodeName.h
//    Project : GHOST library v3.0
//    Authors : Chris Tarr
//    Revised : W. A. Aviles
// ----------------------------------------------------------------------------
// Description: Character string that can be assigned to nodes
//
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#ifndef GST_NODE_NAME
#define GST_NODE_NAME

#include <string.h>

#include <gstDllExport.h>
#include <gstTypedefs.h>

#define GST_MAX_NODE_NAME_LENGTH 255

// Handles character string for node names.
class GHOST_DLL_IMPORT_EXPORT gstNodeName {

public:

	// Constructor.
	gstNodeName(){
		name = new char[1];
		strcpy(name,"");
	}

	// Constructor.
	gstNodeName(const char *newName){
		name = new char[strlen(newName)+1];
		strcpy(name, newName);
	}

	// Constructor.
	gstNodeName(gstNodeName *newName){
		name = new char[strlen(newName->name)+1];
		strcpy(name, newName->name);
	}

	// Constructor.
	gstNodeName(const gstNodeName &newName){
		name = new char[strlen(newName.name)+1];
		strcpy(name, newName.name);
	}

	// Destructor.
	~gstNodeName() {
		delete name;
	}

	// Cast to string operator.
	operator char *() {
		char *temp = new char[strlen(name)+1];
		strcpy(temp, name);
		return temp;
	}

	// Assignment operator.
	gstNodeName&  operator =	(const char *p) { 
		delete name;
		name = new char[strlen(p)+1];
		strcpy(name, p);
		return *this; 
	}

	// Assignment operator.
	gstNodeName&  operator =	(const gstNodeName &p) { 
		delete name;
		name = new char[strlen(p.name)+1];
		strcpy(name, p.name);
		return *this; 
	}

	// Assignment operator.
	gstNodeName&  operator =	(const gstNodeName *p) { 
		delete name;
		name = new char[strlen(p->name)+1];
		strcpy(name, p->name);
		return *this; 
	}

	// Equality operator.
	gstBoolean operator == (const gstNodeName &p) const { 
		return (strcmp(name,p.name) == 0); 
	}

	// Inequality operator.
	gstBoolean operator != (const gstNodeName &p) const { 
		return (strcmp(name,p.name) != 0); 
	}

	// Less than operator.
	gstBoolean operator <  (const gstNodeName &p) const 
	{ return (gstBoolean)( strcmp(name, p.name) < 0); }

	// Less than or equal to operator.
	gstBoolean operator <=  (const gstNodeName &p) const 
	{ return (gstBoolean)( strcmp(name, p.name) <= 0); }

	// Greater than operator.
	gstBoolean operator >  (const gstNodeName &p) const
    { return (gstBoolean)( strcmp(name,p.name) > 0);  }

	// Greater than or equal to operator.
	gstBoolean operator >=  (const gstNodeName &p) const
    { return (gstBoolean)( strcmp(name,p.name) >= 0);  }

protected:
	char		*name;
};


#endif // GST_NODE_NAME
