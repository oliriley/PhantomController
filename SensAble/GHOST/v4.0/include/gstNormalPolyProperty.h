//=============================================================================
//   Filename : gstNormalPolyProperty.h
//    Project : GHOST library v3.0
//    Authors : Chris Tarr
//    Revised :
// ----------------------------------------------------------------------------
// Description: For assigning normals to vertices in a gstPolyMesh
//
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#if !defined(GSTNORMALPOLYPROPERTY_INCLUDED)
#define GSTNORMALPOLYPROPERTY_INCLUDED

#include <gstPolyPropertyPoint3D.h>
#include <gstDllExport.h>

// For assigning normals to vertices in a gstPolyMesh
class GHOST_DLL_IMPORT_EXPORT gstNormalPolyProperty:
	public gstPolyPropertyPoint3D
{
  public:

	gstNormalPolyProperty();

	gstNormalPolyProperty(
		const gstPoint &val);

	inline virtual int Id();

	inline static int staticGetId();

  protected:

	virtual ~gstNormalPolyProperty();

  private:

	static int s_Id;
};


inline int gstNormalPolyProperty::Id()
{
	return(s_Id);
}


inline int gstNormalPolyProperty::staticGetId()
{
	return(s_Id);
}


#endif  // GSTNORMALPOLYPROPERTY_INCLUDED
