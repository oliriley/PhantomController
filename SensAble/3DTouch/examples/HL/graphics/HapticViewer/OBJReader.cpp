/*****************************************************************************

Copyright (c) 2004 SensAble Technologies, Inc. All rights reserved.

OpenHaptics(TM) toolkit. The material embodied in this software and use of
this software is subject to the terms and conditions of the clickthrough
Development License Agreement.

For questions, comments or bug reports, go to forums at: 
    http://dsc.sensable.com

Module Name:

  OBJReader.cpp

Description: 

  Routines for reading meshses from Wavefront .obj files.

******************************************************************************/

#include "stdAfx.h"
#include <assert.h>

#include "objReader.h"
#include "Mesh.h"
#include <HDU/hduBoundBox.h>

static const int STRING_LENGTH = 1024;

  class FileHandle
  {
  public:
    FileHandle(const _TCHAR *filename, const _TCHAR *mode)
    {
      // Windows fopen asserts if the filename is null
      // or is "" so add a check here.
      if (!filename)
      {
          m_file = 0;
      }
      else if (*filename == _T('\0'))
      {
          m_file = 0;
      }
      else
      {
          m_file = _tfopen(filename,mode);
      }
    }
    FileHandle(FILE *file)
    {
      m_file = file;
    }
    operator FILE*()
    {
      return m_file;
    }
    ~FileHandle()
    {
      if (m_file)
        fclose(m_file);
    }
    FILE *file() { return m_file; }

  private:
    FILE *m_file;
  };

// Performs case-insensitive string comparison.
static inline bool stringis_noCase(const char *s1, const char *s2)
{
    return !stricmp(s1,s2);
}

// Reads the next line in a file.  If eof is reached, returns FALSE.
static bool getNextLine(FileHandle &fptr, char *line) 
{ 
    if (!fgets(line,STRING_LENGTH,fptr))
        return FALSE;
    if (feof((FILE *)fptr))
        return FALSE;
    return TRUE;
}

// Reads the next line in a file.  If eof is reached, returns FALSE.
static bool getNextLine(FILE *fptr, char *line) 
{
    if (!fgets(line,STRING_LENGTH,fptr))
        return FALSE;
    if (feof((FILE *)fptr))
        return FALSE;
    return TRUE;
}

//---------------------------------------------------------------------------
// readObjAscii helper function.
// Finds the first occurrence of <substring> in <line>,
// and returns a pointer to the position of <line> after
// <substring> (i.e. after the last character in <substring>).  
// If <substring> does not exist in <line>, the function returns 
// NULL.
//---------------------------------------------------------------------------
static char *advanceString(const char *line, const char *substring) 
{
    char *substring_start = strstr(line,substring);
    if (!substring_start) return NULL;
    char *substring_end = substring_start + strlen(substring);
    if (substring_end >= line+strlen(line))
        return NULL;
    return substring_end;
}

//---------------------------------------------------------------------------
// readObjAscii helper function.
// Scans one set of up to three slash-separated integers (i.e. a set),
// from the head of "line", returns the position of "line"
// after the scanned set. 
//// Inputs 
//// *line - the line from which the set is scanned.
//// pts[3] - modified to contain the resulting integers scanned
//// pts_b[3] - modified to be TRUE for each index that contained
////  a successfully scanned integer.
//
// The set can be in any of these forms:
// <x>
// <x>/<y>
// <x>/<y>/<z>
// <x>//<z>
// For example - 
// line = "3//5 4/5/6"
// would yield:
// pts[3] - {3,0,5}
// pts_b[3] - {TRUE,FALSE,TRUE}
// return value - " 4/5/6"
//
//---------------------------------------------------------------------------
static char *scanSet(const char *line, int pts[3], bool pts_b[3]) 
{
    for (int i=0; i < 3; i++)
        pts_b[i] = FALSE;
    
    // Extract the set from the string, and also
    // advance the line pointer to the next character
    // after the string (this becomes the return value).
    // A set is a single string containing no spaces (obj spec,
    // p. B1-16).
    if (line[0] == '\n')
        return NULL;
    char line_set_head[100];
    char *line_set = line_set_head;
    if (sscanf(line,"%s",line_set) == 0) {
        return NULL;
    }
    if (strlen(line_set) == 0) {
        return NULL;
    }
    line = advanceString(line,line_set);
    if (!line) {
        return NULL;
    }
    
    // Scan up to three '/'-separated integers in the set.
    // If there are more than three '/'s, only the 
    // numbers associated with the first three will be scanned.
    for (int n=0; n < 3; n++) 
    {
        // Try to scan in an integer for the current index,
        // and set the boolean for that index to TRUE
        // if successful.
        pts_b[n] = (sscanf(line_set,"%d",&pts[n]) == 1);
        
        // Find the next slash in the set, if one exists.  If 
        // we reach a space before the slash (i.e. this means 
        // we've reached the next set) or do not find
        // a slash before EOL, end.  Either of those conditions
        // signifies that we are at the end of the set.
        char *next_ptr = strchr(line_set,(int) '/');
        if (next_ptr != NULL)
        {
            line_set = next_ptr + 1;
        }
        else break;
    }
    return (char *)line;
}

bool readObjVertices(
    FileHandle &fptr,
    VertContainer* verts,
    hduBoundBox3Df* vertBox
    )
{
    char line[STRING_LENGTH];

    while (getNextLine(fptr,line))
    {
        if (strlen(line) > STRING_LENGTH-2)
        {
            throw MsgIDError("readOBJvertices maximum single line length exceeded.");
        }
        
        char identifier[STRING_LENGTH];
        if (sscanf(line,"%s",identifier) != 1)
        {
            continue;
        }
        // All vertex info must be in the form 
        // "v <x> <y> <z>", e.g. "v 1.2 2.1 5.56".
        if (stringis_noCase(identifier,"v"))
        {
            float x,y,z;
            if (sscanf(line,"%*[^ ]%f %f %f",&x,&y,&z) != 3)
            {
                throw MsgIDError("readOBJvertices bad vertex count");
            }
            Vertex v(x,y,z);
            verts->push_back(v);
            vertBox->Union(v);
        }

        // Ignore the following identifiers, since we don't parse
        // those in V1.  These are here just for future reference as
        // place holders.
        if (stringis_noCase(identifier,"vt"))
        {
            float u,v;
            if (sscanf(line,"%*[^ ]%f %f",&u,&v) != 2)
            {
                throw MsgIDError("readOBJvertices bad texture vertex count");
            }
            // textures.push_back(hduVector3Df(u,v,0));
        }
        if (stringis_noCase(identifier,"vn"))
        {
            ;
        }
    }

    return TRUE;
}    

//---------------------------------------------------------------------------
// readObjAscii helper function.
// This function reads face information from the file starting
// at the location of fptr, and stores those in
// <tris>.  The function currently either returns TRUE
// or throws an error.  
//
//---------------------------------------------------------------------------
bool readObjFaces(
    FileHandle &fptr,
    VertContainer* verts,
    TriContainer* tris
    )
{
    vector<hduVector3Df> vertTextures;
    vector<Triangle> triTextures;

    long vertex_indices[3] = {0,0,0}; 
    const int STRING_LENGTH = 1024;
    char line[STRING_LENGTH];
    while (getNextLine(fptr,line))
    {
        char identifier[STRING_LENGTH];
        if (sscanf(line,"%s",identifier) != 1)
        {
            continue;
        }
        
        // Keep track of which vertex index we are at currently while reading
        // for vertex positions, texture coordinates, and normals.  This is
        // necessary because geometries can reference these 
        // in relation to where
        // they are located themselves (e.g. "-5" as a reference 
        // means "five vertices 
        // above where this statement is.").
        const int V_INDEX = 0;
        const int VT_INDEX = 1;
        const int VN_INDEX = 2;
        if (stringis_noCase(identifier,"v"))
        {
            vertex_indices[V_INDEX]++;
        }
        if (stringis_noCase(identifier,"vt"))
        {
            vertex_indices[VT_INDEX]++;
        }
        if (stringis_noCase(identifier,"vn"))
        {
            vertex_indices[VN_INDEX]++;
        }
        
        // Parse faces and add resulting triangles.
        if (stringis_noCase(identifier,"f") ||
            stringis_noCase(identifier,"fo")) 
        {
            char *line_pointer = line+2; // Start parsing after the "f " or "fo"
            vector<int>v_vertexList;
            vector<int>v_vertexTextureList;
            
            // Parse a single line of vertex information, resulting in
            // a list containing the vertex indices for one face.
            while (line_pointer) 
            {
                // Parse the vertex set, advance the line_pointer 
                // to the next vertex set.
                // Vertex sets can appear as 
                // <x>, <x>/<y>, <x>//<y>, or <x>/<y>/<z>.
                int pt[3];
                bool pt_b[3];
                line_pointer = scanSet(line_pointer,pt,pt_b);
                // Check for validity. 
                // Vertex indices must be non-zero.
                // If vertex indices are negative, they are converted
                // to absolute indices.
                for (int i=0; i < 3; i++) 
                {
                    if (!pt_b[i]) 
                    {
                        continue;
                    }
                    if (pt[i] < 0)
                    {
                        pt[i] = vertex_indices[i] + pt[i] + 1;                        
                    }
                    if (pt[i] <= 0 )
                    {
                        assert(false);
                        throw "readObjFaces invalid vertex (zero or negative) read.";
                    }

                    // Subtract one because SurfaceModel indices are
                    // zero-ordered whereas OBJ indices are 1-ordered.
                    // E.G. a "1" as a vertex index in the OBJ file
                    // refers to the first vertex in the file, which
                    // is array element 0 in the verts().
                    pt[i]--;
                }
                
                // Push back the vertex positional index.  
                // Note that we ignore the normal and texture information, 
                // but would add those into their respective lists as well 
                // if we needed that information.
                if (pt_b[V_INDEX] == TRUE) 
                {
                    
                    if (pt[V_INDEX] >= verts->size())
                    {
                        assert(false);
                        throw "readObjFace face specifies invalid vertex.";
                    }
                    v_vertexList.push_back(pt[V_INDEX]);
                    
                    // Push back any corresponding texture for this vertex.
                    // If none, just stuff a zero.
                    if (pt_b[VT_INDEX] == TRUE)
                    {
                        if (pt[VT_INDEX] >= vertTextures.size()) 
                            throw "readObjFace face specifies invalid vertex texture.";
                        v_vertexTextureList.push_back(pt[VT_INDEX]);
                    }
                    else
                    {
                        v_vertexTextureList.push_back(0);
                    }
                }
            }
            
            if (v_vertexList.size() < 3) 
                throw "readObjFace face list with fewer than three vertices read.";
            if (v_vertexList.size() == 3)
            {
                // Push back indices of vertices, e.g. (0,1,2), (2,3,0)
                tris->push_back(Triangle(v_vertexList[0],
                                         v_vertexList[1],
                                         v_vertexList[2]));
                triTextures.push_back(Triangle(v_vertexTextureList[0],
                                               v_vertexTextureList[1],
                                               v_vertexTextureList[2]));
            }
            else if (v_vertexList.size() == 4)
            {
                tris->push_back(Triangle(v_vertexList[0],
                                         v_vertexList[1],
                                         v_vertexList[2]));
                triTextures.push_back(Triangle(v_vertexTextureList[0],
                                               v_vertexTextureList[1],
                                               v_vertexTextureList[2]));

                tris->push_back(Triangle(v_vertexList[0],
                                         v_vertexList[2],
                                         v_vertexList[3]));
                triTextures.push_back(Triangle(v_vertexTextureList[0],
                                               v_vertexTextureList[2],
                                               v_vertexTextureList[3]));
            }
            else
            {
                throw "face found with more than four vertices.";
            }
        }
        
        // Ignore other types of geometric data, such as surfaces,
        // because we currently do not use those.  In the future,
        // if we were to parse surfaces, curves, groups, render/display
        // attributes, or other data, we would add that information here.
        if (stringis_noCase(identifier,"cstype")) 
        {
            ;
        }
        if (stringis_noCase(identifier,"surf")) 
        {
            ;
        }
        if (stringis_noCase(identifier,"g")) 
        {
            ;
        }
    }
    return TRUE;
}

//---------------------------------------------------------------------------
// This function reads an OBJ file and generates vertices
// and triangles.  
// The function will throw ReadOBJError types if the file
// read is not entirely successful.
// Note that this function does not collapse duplicate
// vertices; it simply parses the file.  Freeform-specific
// operations such as removing duplicate vertices and checking
// if the file is a valid solid FF model should be performed at 
// a higher level.
//
//---------------------------------------------------------------------------
bool readOBJAscii(const char *filename,
                  VertContainer* verts,
                  TriContainer* tris,
                  hduBoundBox3Df* vertBox)
{
    FileHandle fptr(filename,"r");
    if (!fptr) return false;
    
    long numverts = 0;
    
    // Do first pass through file.  
    // Read vertices and other appropriate numerically 
    // ordered information (e.g. textures, normals) and store those.  
    // These will then be used as information to 
    // generate the entities (e.g. polygons, surfaces) during
    // the second pass.
    if (!readObjVertices(fptr,verts,vertBox))
        throw "readObjAscii vertex read failed.";
    // Do second pass through file.  
    // Read entities and construct using vertex information stored from 
    // first pass.
    if (verts->size() == 0)
        throw "readOBJAscii zero vertices read.";
        
    rewind(fptr);
    if (!readObjFaces(fptr,verts, tris))
        throw "readObjAscii faces read failed.";
    if (tris->size() == 0)
        throw "readOBJAscii zero faces read.";

    return true;
}

/******************************************************************************/
