//
// Copyright 1998-1999, SensAble Technologies, Inc.
// 
// file: gstVRMLError.h
//
// Rev: 1.0
//
// Description: Error reporting functionality for the VRML parsing. 
//
// Author: Tim Gallagher tim@sensable.com
//
#ifndef _GST_VRML_ERROR_H_
#define _GST_VRML_ERROR_H_


#include <gstVRMLDllExport.h>

class gstSeparator;

// The possible error types during the GHOST VRML reading process.
enum gstVRMLErrorType
{
	gst_VET_NoError,					// there was no error
	gst_VET_NoFile,						// the file was not found
	gst_VET_CannotAddNullNode,			// an attempt to add a null node
	gst_VET_CurrentNodeIsNull,			// an attempt to add a node to a null node
	gst_VET_RootNodeIsNull,				// there is no valid root node
	gst_VET_CouldNotFindNodeByName,		// a node specified by name could not be found
	gst_VET_CouldNotFindParentNode,		// a node that should have a parent could not locate it
	gst_VET_AttemptToPushNullNode,		// an attempt to add a null node
	gst_VET_FieldConversionFailure,		// text string could not be converted to proper type
	gst_VET_NonUniformScaleOnNonLeaf,	// attempt to perform nonuniform scale on leaf node
	gst_VET_ScaleOrientationNotSupported,
	gst_VET_UnknownNodeType,			// a node that is of some unrecognized type
	gst_VET_UnknownFieldType,			// a field that is of some unrecognized type
	gst_VET_SyntaxError,					// encountered a syntax error during parsing
	gst_VET_InvalidHeader
};


// Class to handle VRML file reading errors.
// Records the error type, file string related to error, and the
// line number where the error was encountered.
class GHOST_VRML_DLL_IMPORT_EXPORT gstVRMLError
{
    friend class gstVRMLGraph;
	friend void gstVRMLPushError(
		gstVRMLErrorType error, int line, const char *msg);

    public:
        gstVRMLError();
		gstVRMLError(const gstVRMLError &);
        ~gstVRMLError();

		gstVRMLError &operator=(const gstVRMLError &err);
		// Return the last parsed text string when error encountered.
        const char *GetMSG() const {return msg_;}

		// Return the line number which was current when error encountered.
        int GetLine() const {return line_;}

		// Return the specific error type.
		gstVRMLErrorType GetError() const {return error_;}

    private:
        void SetMSG(const char *msg);
        void SetLine(int line);
		void SetError(gstVRMLErrorType error);

		void Set(gstVRMLErrorType error, int line, const char *msg);

        char *msg_; // some error text, perhaps null
        int line_;  // line number of error
		gstVRMLErrorType error_; // is there an error?
};

void gstVRMLPushError(gstVRMLErrorType error,int line,const char *msg);
/* Refer to gstVRML.h for the rest of these
int	gstVRMLGetNumErrors();
gstVRMLError gstVRMLPopEarliestError();
gstVRMLError gstVRMLPopLatestError();
void gstVRMLClearErrors();
int gstVRMLWriteErrorsToFile(const char *filename);
*/

#endif // _GST_VRML_ERROR_H_
