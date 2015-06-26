//=============================================================================
//   Filename : gstPolyPropertyContainer.h
//    Project : GHOST library v3.0
//    Authors : Chris Tarr
//    Revised :
// ----------------------------------------------------------------------------
// Description: Container class to hold indexed property information of 1,2, 
//              and/or 3 dimensional quality pertaining to an entire polygon
//              or to it's individual vertices.
//
// Send your questions, comments or bugs to:
//    support@sensable.com
//    http://www.sensable.com
//
// Copyright (c) 1996-1999 SensAble Technologies, Inc. All rights reserved.
//=============================================================================

#if !defined(GSTPOLYPROPERTYCONTAINER_INCLUDED)
#define GSTPOLYPROPERTYCONTAINER_INCLUDED

#include <gstDllExport.h>
#include <gstPolyPropertyBase.h>
#include <list.h>

// One poly property with an ID
class gstOnePolyPropertyIdStruct {
public:
    int Id;
    gstPolyPropertyBase *prop;

    //
    // Equality test operator.
    //
    inline gstBoolean operator ==(
        const gstOnePolyPropertyIdStruct &e) const
    {
        return((Id == e.Id) && (prop == e.prop));
    }
    
    //
    // Less than operator.
    //
    inline gstBoolean operator < ( const gstOnePolyPropertyIdStruct &e) const
    {
        return((Id < e.Id));
    }

};

// Three poly properties with an ID
class gstThreePolyPropertyIdStruct {
public:
    int Id;
    gstPolyPropertyBase *prop1;
    gstPolyPropertyBase *prop2;
    gstPolyPropertyBase *prop3;

    //
    // Equality test operator.
    //
    inline gstBoolean operator ==(
        const gstThreePolyPropertyIdStruct &e) const
    {
        return((Id == e.Id) && (prop1 == e.prop1) && 
                (prop2 == e.prop2) && (prop3 == e.prop3));
    }
    
    //
    // Less than operator.
    //
    inline gstBoolean operator < ( const gstThreePolyPropertyIdStruct &e) const
    {
        return((Id < e.Id));
    }
};

#if(GHOST_EXTRA_TEMPLATE_DECLARATIONS)
// Declare STL routines used by GHOST to be exported symbols from the dll.
GHOST_EXPIMP_TEMPLATE template class GHOST_DLL_IMPORT_EXPORT _List_base<gstOnePolyPropertyIdStruct, class __default_alloc_template<1,0> >;
GHOST_EXPIMP_TEMPLATE template class GHOST_DLL_IMPORT_EXPORT _List_base<gstThreePolyPropertyIdStruct, class __default_alloc_template<1,0> >;
GHOST_EXPIMP_TEMPLATE template class GHOST_DLL_IMPORT_EXPORT list<gstOnePolyPropertyIdStruct, class __default_alloc_template<1,0> >;
GHOST_EXPIMP_TEMPLATE template class GHOST_DLL_IMPORT_EXPORT list<gstThreePolyPropertyIdStruct, class __default_alloc_template<1,0> >;
#endif


//
// Container class to hold indexed property information of 1,2, and/or 3 dimensional
// quality pertaining to an entire polygon or to it's individual vertices.
//
class GHOST_DLL_IMPORT_EXPORT gstPolyPropertyContainer
{
  public:


    virtual ~gstPolyPropertyContainer();

    //
    // Evaluates the value of the gstPolyProperty with Id() equal to polyPropertyId
    // at the barycentric coordinate barycentricCoord and stores the value in 'val'.
    // Returns TRUE if successful.  If one value of polyPropertyId is stored through
    // the setPropertyPoly method then 'val' is set to this and TRUE is returned.  
    // If three values of polyPropertyId are stored for vertices v1, v2, and v3 then 
    // 'val' is set to the interpolated value at barycentricCoord and TRUE is returned.
    // If no property of Id equal to polyPropertyId is stored then FALSE is returned.
    //
    gstBoolean evalDoubleProperty(
        int polyPropertyId,
        gstPoint barycentricCoord,        // Barycentric coordinate to evaluate property at
        double &val);

    //
    // Evaluates the value of the gstPolyProperty with Id() equal to polyPropertyId
    // at the barycentric coordinate barycentricCoord and stores the value in 'val'.
    // Returns TRUE if successful.  If one value of polyPropertyId is stored through
    // the setPropertyPoly method then 'val' is set to this and TRUE is returned.  
    // If three values of polyPropertyId are stored for vertices v1, v2, and v3 then 
    // 'val' is set to the interpolated value at barycentricCoord and TRUE is returned.
    // If no property of Id equal to polyPropertyId is stored then FALSE is returned.
    //
    gstBoolean evalPoint2DProperty(
        int polyPropertyId,
        gstPoint barycentricCoord,        // Barycentric coordinate to evaluate property at
        gstPoint2D &val);

    //
    // Evaluates the value of the gstPolyProperty with Id() equal to polyPropertyId
    // at the barycentric coordinate barycentricCoord and stores the value in 'val'.
    // Returns TRUE if successful.  If one value of polyPropertyId is stored through
    // the setPropertyPoly method then 'val' is set to this and TRUE is returned.  
    // If three values of polyPropertyId are stored for vertices v1, v2, and v3 then 
    // 'val' is set to the interpolated value at barycentricCoord and TRUE is returned.
    // If no property of Id equal to polyPropertyId is stored then FALSE is returned.
    //
    gstBoolean evalPoint3DProperty(
        int polyPropertyId,
        gstPoint barycentricCoord,        // Barycentric coordinate to evaluate property at
        gstPoint &val);

    //
    // Returns a pointer to the property associated with the entire poly with Id equal 
    // to polyPropertyId.  If none exists, NULL is returned.
    //
    gstPolyPropertyBase * getPropertyPoly(
        int polyPropertyId);

    //
    // Returns a pointer to the property associated with vertex v1 with Id equal 
    // to polyPropertyId.  If none exists, NULL is returned.
    //
    gstPolyPropertyBase * getPropertyV1(
        int polyPropertyId);

    //
    // Returns a pointer to the property associated with vertex v2 with Id equal 
    // to polyPropertyId.  If none exists, NULL is returned.
    //
    gstPolyPropertyBase * getPropertyV2(
        int polyPropertyId);

    //
    // Returns a pointer to the property associated with vertex v3 with Id equal 
    // to polyPropertyId.  If none exists, NULL is returned.
    //
    gstPolyPropertyBase * getPropertyV3(
        int polyPropertyId);

    //
    // Removes the property associated with the entire poly with Id equal 
    // to polyPropertyId and returns TRUE.  If none exists, FALSE is returned.
    //
    gstBoolean removePropertyPoly(
        int polyPropertyId);

    //
    // Removes the property associated with vertices with Id equal 
    // to polyPropertyId and returns TRUE.  If none exists, FALSE is returned.
    //
    gstBoolean removePropertyVerts(
        int polyPropertyId);

    //
    // Stores the property prop and associates it with the entire poly and
    // returns TRUE.  Returns FALSE is prop is NULL.
    //
    gstBoolean setPropertyPoly(
        gstPolyPropertyBase *prop);

    //
    // Stores the property prop and associates it with vertex v1 of poly and
    // returns TRUE.  Returns FALSE is prop is NULL.
    //
    gstBoolean setPropertyV1(
        gstPolyPropertyBase *prop);

    //
    // Stores the property prop and associates it with vertex v2 of poly and
    // returns TRUE.  Returns FALSE is prop is NULL.
    //
    gstBoolean setPropertyV2(
        gstPolyPropertyBase *prop);

    //
    // Stores the property prop and associates it with vertex v3 of poly and
    // returns TRUE.  Returns FALSE is prop is NULL.
    //
    gstBoolean setPropertyV3(
        gstPolyPropertyBase *prop);

  protected:

    //
    // Returns structure for vertex properties of Id = polyPropertyId.
    //
    gstThreePolyPropertyIdStruct * getVertPropertyStruct(
        int polyPropertyId);

  private:


    list<gstOnePolyPropertyIdStruct> propsPoly;


    list<gstThreePolyPropertyIdStruct> propsVerts;
};


#endif  // GSTPOLYPROPERTYCONTAINER_INCLUDED
