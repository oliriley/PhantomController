/*****************************************************************************

Copyright (c) 2004 SensAble Technologies, Inc. All rights reserved.

OpenHaptics(TM) toolkit. The material embodied in this software and use of
this software is subject to the terms and conditions of the clickthrough
Development License Agreement.

For questions, comments or bug reports, go to forums at: 
    http://dsc.sensable.com

Module Name:

  HapticMaterial.cpp

Description: 

  This example demonstrates setting haptic material properties on a group of 
  objects and dragging objects with contraints.

  This example displays a group of objects.  When the user touches one and 
  holds the stylus button down, the user can move it around the screen.  While
  the button is down, constraint axes are drawn allowing more precise
  control over dragging.  The user can change the haptic material
  properties of the object via the GLUI user interface.

  This example shows the integration with GLUI, a user interface
  toolkit, from University of North Carolina.

  This example shows the integration with haptic device as a mouse.

*******************************************************************************/

#include <math.h>
#include <assert.h>

#ifdef WIN32
#include <windows.h>
#endif

#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/glui.h>

#include <string.h>

#include <HL/hl.h>
#include <HDU/hduMath.h>
#include <HDU/hduMatrix.h>
#include <HDU/hduQuaternion.h>
#include <HDU/hduError.h>
#include <HLU/hlu.h>

#include <HapticMouse/HapticMouse.h>

#include <vector>
#include <iostream>

/* Function prototypes. */
void glutDisplay();
void glutReshape(int width, int height);
void glutIdle(); 
void glutMouse(int button, int state, int x, int y);
void glutMotion(int x, int y);
void glutKeyboard(unsigned char key, int x, int y);

void initHL();
void initGL();
void initScene();
void drawScene();
void drawString(const char* string);
void drawPrompts();
void updateCamera();
void updateHapticMapping();

void createDraggableObjects();
void drawDraggableObjects();

void redrawCursor();

void HLCALLBACK buttonDownCollisionThreadCallback(HLenum event, HLuint object, 
                                                  HLenum thread, HLcache *cache, 
                                                  void *userdata);
void HLCALLBACK buttonDownClientThreadCallback(HLenum event, HLuint object, 
                                               HLenum thread, HLcache *cache, 
                                               void *userdata);
void HLCALLBACK buttonUpClientThreadCallback(HLenum event, HLuint object, 
                                             HLenum thread, HLcache *cache, 
                                             void *userdata);
void updateDragObjectTransform();


void __cdecl exitHandler();


static hduVector3Dd gCameraPosWC;
static int gWindowWidth, gWindowHeight;
static int gViewportWidth, gViewportHeight;

#define CURSOR_SIZE_PIXELS 20
static double gCursorScale;
static GLint gCursorDisplayList = 0;

/* Variables used by the trackball emulation. */
static hduMatrix gCameraRotation;
float view_rotate[16] = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
float obj_pos[] = { 0.0, 0.0, 0.0 };

static double gCameraScale = 1.0;
static double gCameraTranslationX = 0;
static double gCameraTranslationY = 0;
static bool gIsRotatingCamera = false;
static bool gIsScalingCamera = false;
static bool gIsTranslatingCamera = false;
static int gLastMouseX, gLastMouseY;

static bool gIsProxyManipPoint = false;

/* Haptic device and rendering context handles. */
static HHD ghHD = HD_INVALID_HANDLE;
static HHLRC ghHLRC = NULL;

HLdouble gInitWorkspace[6];

static const double kPI = 3.1415926535897932384626433832795;

static HLuint gAxisId;
static hduVector3Dd gAxisCenter(0,0,0);


/* Live variables passed into GLUI. */
int   obj_type = 0;
int   light0_enabled = 1;
int   light1_enabled = 1;
float light0_intensity = 1.0;
float light1_intensity = 1.0;
int   main_window;

/* Haptic material properties. */
float hap_stiffness = 0.8;
float hap_damping = 0.0;
float hap_static_friction = 0.0;
float hap_dynamic_friction = 0.0;

/* GLUI: Pointers to the windows and some of the controls. */
GLUI            *glui;
GLUI_Panel      *obj_panel;
GLUI_RadioGroup *radio;
GLUI_Checkbox   *checkbox;
GLUI_Spinner    *light0_spinner, *light1_spinner;

/* haptic material properties. */
GLUI_Panel      *haptic_mat_panel;
GLUI_Spinner    *stiffness_spinner;
GLUI_Spinner    *damping_spinner;
GLUI_Spinner    *static_friction_spinner;
GLUI_Spinner    *dynamic_friction_spinner;

GLUI_Control *lastGluiCon = NULL;

/* User IDs for callbacks */
#define OBJECT_TYPE_ID       100
#define LIGHT0_ENABLED_ID    200
#define LIGHT1_ENABLED_ID    201
#define LIGHT0_INTENSITY_ID  250
#define LIGHT1_INTENSITY_ID  251
 
#define VIEW_ROTATE_ID       252
#define VIEW_PAN_ID          253
#define VIEW_ZOOM_ID         254

/* Lighting parameters */
GLfloat light0_ambient[] =  {0.1f, 0.1f, 0.3f, 1.0f};
GLfloat light0_diffuse[] =  {.6f, .6f, 1.0f, 1.0f};
GLfloat light0_position[] = {.5f, .5f, 1.0f, 0.0f};

GLfloat light1_ambient[] =  {0.1f, 0.1f, 0.3f, 1.0f};
GLfloat light1_diffuse[] =  {.9f, .6f, 0.0f, 1.0f};
GLfloat light1_position[] = {-1.0f, 0.9f, 1.0f, 0.0f};


/* Struct representing a shape in the scene that can be felt, 
   touched, transformed, and drawn. */
struct DraggableObject
{
    HLuint shapeId;
    GLuint displayList;
    hduMatrix transform;

    float hap_stiffness;
    float hap_damping;
    float hap_static_friction;
    float hap_dynamic_friction;
};

/* List of all draggable objects in scene. */
std::vector<DraggableObject> draggableObjects;

/* Object currently being dragged (index into draggableObjects). */
int gCurrentDragObj = -1;

/* Position and orientation of proxy at start of drag. */
hduVector3Dd gStartDragProxyPos;
hduQuaternion gStartDragProxyRot;

/* Position and orientation of drag object at start of drag. */
hduMatrix gStartDragObjTransform;

/* Flag for enabling/disabling axis snap on drag. */
bool gAxisSnap = true;

/* Flag for enabling/disabling rotation. */
bool gRotate = true;

/*******************************************************************************
 Main function.
*******************************************************************************/
int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowSize(850, 500);

    main_window = glutCreateWindow("Haptic Material");
  
    // Set glut callback functions.
    glutDisplayFunc(glutDisplay);
    glutMotionFunc(glutMotion);
    
    GLUI_Master.set_glutReshapeFunc(glutReshape);  
    GLUI_Master.set_glutKeyboardFunc(glutKeyboard);
    GLUI_Master.set_glutMouseFunc(glutMouse);
    GLUI_Master.set_glutSpecialFunc(NULL);
    
    // The GLUT main loop won't return control, so we need to perform cleanup
    // using an exit handler.
    atexit(exitHandler);
    
    initScene(); 
    
    // egister the idle callback with GLUI (not with GLUT).
    GLUI_Master.set_glutIdleFunc(glutIdle);

    glutMainLoop();
    
    return 0;
}


/*******************************************************************************
 GLUI control callback.
*******************************************************************************/
void control_cb(int control)
{
    if( control == OBJECT_TYPE_ID )
    {
        DraggableObject& obj = draggableObjects[obj_type]; 
        hap_stiffness = obj.hap_stiffness;
        hap_damping = obj.hap_damping;
        hap_static_friction = obj.hap_static_friction;
        hap_dynamic_friction = obj.hap_dynamic_friction;
    }
    else if ( control == VIEW_ROTATE_ID ) {
        updateCamera();
    }
    else if ( control == VIEW_PAN_ID ) {
        updateCamera();
    }
    else if ( control == VIEW_ZOOM_ID ) {
        updateCamera();
    }
    
    else if ( control == LIGHT0_ENABLED_ID ) {
        if ( light0_enabled ) {
            glEnable( GL_LIGHT0 );
            light0_spinner->enable();
        }
        else {
            glDisable( GL_LIGHT0 ); 
            light0_spinner->disable();
        }
    }
    else if ( control == LIGHT1_ENABLED_ID ) {
        if ( light1_enabled ) {
            glEnable( GL_LIGHT1 );
            light1_spinner->enable();
        }
        else {
            glDisable( GL_LIGHT1 ); 
            light1_spinner->disable();
        }
    }
    else if ( control == LIGHT0_INTENSITY_ID ) {
        float v[] = { light0_diffuse[0],  light0_diffuse[1],
            light0_diffuse[2],  light0_diffuse[3] };
        
        v[0] *= light0_intensity;
        v[1] *= light0_intensity;
        v[2] *= light0_intensity;
        
        glLightfv(GL_LIGHT0, GL_DIFFUSE, v );
    }
    else if ( control == LIGHT1_INTENSITY_ID ) {
        float v[] = { light1_diffuse[0],  light1_diffuse[1],
            light1_diffuse[2],  light1_diffuse[3] };
        
        v[0] *= light1_intensity;
        v[1] *= light1_intensity;
        v[2] *= light1_intensity;
        
        glLightfv(GL_LIGHT1, GL_DIFFUSE, v );
    }
}

/*******************************************************************************
 Initializes GLUI user interface.
*******************************************************************************/
void initGLUI()
{     
    printf("GLUI version: %3.2f\n", GLUI_Master.get_version());
    
    glui = GLUI_Master.create_glui_subwindow(main_window, GLUI_SUBWINDOW_RIGHT);
    
    glui->add_statictext("Haptic Material Property"); 
    obj_panel = glui->add_panel( "Select an object" );
    
    // Control for the object type.

    GLUI_Panel *type_panel = glui->add_panel_to_panel(obj_panel, "Type");
    
    radio = glui->add_radiogroup_to_panel(
        type_panel,&obj_type, OBJECT_TYPE_ID, control_cb);

    glui->add_radiobutton_to_group(radio, "Sphere");
    glui->add_radiobutton_to_group(radio, "Tetrahedron");
    glui->add_radiobutton_to_group(radio, "Dodecahedron");
    glui->add_radiobutton_to_group(radio, "Cube");
    glui->add_radiobutton_to_group(radio, "Cone");
    
    // Add some controls for haptic material property.
    
    haptic_mat_panel = glui->add_rollout("Haptic Material Setting", true);
    stiffness_spinner  = glui->add_spinner_to_panel(
        haptic_mat_panel, "Stiffness:",
        GLUI_SPINNER_FLOAT, &hap_stiffness);
    stiffness_spinner->set_float_limits(0.0f, 1.0f);
    stiffness_spinner->set_alignment(GLUI_ALIGN_RIGHT);
    
    damping_spinner  = glui->add_spinner_to_panel(
        haptic_mat_panel, "Damping:", 
        GLUI_SPINNER_FLOAT, &hap_damping);
    damping_spinner->set_float_limits(0.0f, 1.0f);
    damping_spinner->set_alignment(GLUI_ALIGN_RIGHT);
    
    static_friction_spinner  = glui->add_spinner_to_panel(
        haptic_mat_panel, "Static friction:", 
        GLUI_SPINNER_FLOAT, &hap_static_friction);
    static_friction_spinner->set_float_limits(0.0f, 1.0f);
    static_friction_spinner->set_alignment( GLUI_ALIGN_RIGHT );
    
    dynamic_friction_spinner  = glui->add_spinner_to_panel(
        haptic_mat_panel, "Dynamic friction:",
        GLUI_SPINNER_FLOAT, &hap_dynamic_friction);
    dynamic_friction_spinner->set_float_limits(0.0f, 1.0f);
    dynamic_friction_spinner->set_alignment(GLUI_ALIGN_RIGHT);
    
    // Add some controls for lights.
    
    GLUI_Panel *light0 = glui->add_panel("Light 1");
    GLUI_Panel *light1 = glui->add_panel("Light 2");
    
    glui->add_checkbox_to_panel(
        light0, "Enabled", &light0_enabled,
        LIGHT0_ENABLED_ID, control_cb );
    light0_spinner = glui->add_spinner_to_panel(
        light0, "Intensity:", GLUI_SPINNER_FLOAT,
        &light0_intensity, LIGHT0_INTENSITY_ID,
        control_cb);
    light0_spinner->set_float_limits(0.0f, 1.0f);
    
    glui->add_checkbox_to_panel(
        light1, "Enabled", &light1_enabled,
        LIGHT1_ENABLED_ID, control_cb );
    light1_spinner = glui->add_spinner_to_panel(
        light1, "Intensity:", GLUI_SPINNER_FLOAT,
        &light1_intensity, LIGHT1_INTENSITY_ID,
        control_cb );
    light1_spinner->set_float_limits( 0.0, 1.0 );
    
    glui->add_column( true );
    GLUI_Rotation *view_rot = glui->add_rotation(
        "Rotate View", view_rotate, VIEW_ROTATE_ID, control_cb);
    view_rot->set_spin( 1.0 );
    
    GLUI_Translation *trans_xy = glui->add_translation(
        "Pan", GLUI_TRANSLATION_XY, obj_pos, VIEW_PAN_ID, control_cb);
    trans_xy->set_speed( .005 );
    
    GLUI_Translation *trans_z = glui->add_translation(
        "Zoom", GLUI_TRANSLATION_Z, &obj_pos[2], VIEW_ZOOM_ID, control_cb);
    trans_z->set_speed( .005 );

    // A 'quit' button.
    
    glui->add_button("Quit", 0,(GLUI_Update_CB) exit);
    
    glui->add_separator();
    glui->add_statictext("SensAble Technologies Inc."); 
    glui->add_statictext("Integration example with GLUI"); 
    glui->add_separator();
    // Link windows to GLUI, and register idle callback.
    
    glui->set_main_gfx_window(main_window);
}


/*******************************************************************************
 GLUT callback for redrawing the view.  Use this to perform graphics rate 
 processing.
*******************************************************************************/
void glutDisplay()
{    
    drawScene();
}

/*******************************************************************************
 GLUT callback for reshaping the window.  This is the main place where the 
 viewing and workspace transforms get initialized.
*******************************************************************************/
void glutReshape(int width, int height)
{
    static const double kFovY = 40;
    static const double kCanonicalSphereRadius = 2;

    gWindowWidth = width;
    gWindowHeight = height;

    int tx, ty;
    GLUI_Master.get_viewport_area(&tx, &ty, &gViewportWidth, &gViewportHeight);
    glViewport(tx, ty, gViewportWidth, gViewportHeight);

    // Compute the viewing parameters based on a fixed fov and viewing
    // sphere enclosing a canonical box centered at the origin.
    double nearDist = kCanonicalSphereRadius / tan((kFovY / 2.0) * kPI / 180.0);
    double farDist = nearDist + 2.0 * kCanonicalSphereRadius;
    double aspect = (double) gViewportWidth / gViewportHeight;
   
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(kFovY, aspect, nearDist, farDist);

    // Place the camera down the Z axis looking at the origin.
    gCameraPosWC[0] = 0;
    gCameraPosWC[1] = 0;
    gCameraPosWC[2] = nearDist + kCanonicalSphereRadius;
 
    updateCamera();
}

/*******************************************************************************
 GLUT callback for idle state.  Use this to request a redraw.
*******************************************************************************/
void glutIdle()
{
    // According to the GLUT specification, the current window is 
    // undefined during an idle callback.  So we need to explicitly change
    // it if necessary.
    if (glutGetWindow() != main_window) 
    {
        glutSetWindow(main_window);
    }
    
    glutPostRedisplay();
    
    glui->sync_live();
    
    // heck which object is currently selected and update its material.
    DraggableObject& obj = draggableObjects[obj_type]; 
    obj.hap_stiffness = hap_stiffness;
    obj.hap_damping = hap_damping;
    obj.hap_static_friction = hap_static_friction;
    obj.hap_dynamic_friction = hap_dynamic_friction;
}

/*******************************************************************************
 Initializes the scene.  Handles initializing both OpenGL and HDAPI. 
*******************************************************************************/
void initScene()
{
    initGL();
    initGLUI();
    initHL();

    createDraggableObjects();
}

/*******************************************************************************
 Cleans up.
*******************************************************************************/
void exitHandler()
{
    // Shutdown the haptic mouse.
    hmShutdownMouse();

    // Free up the haptic rendering context.
    hlMakeCurrent(NULL);
    if (ghHLRC != NULL)
    {
        hlDeleteContext(ghHLRC);
    }

    // Free up the haptic device.
    if (ghHD != HD_INVALID_HANDLE)
    {
        hdDisableDevice(ghHD);
    }
}

/*******************************************************************************
 Sets up general OpenGL rendering properties: lights, depth buffering, etc. 
*******************************************************************************/
void initGL()
{
    glClearColor(0.5f, 0.6f, 0.0f, 1.0f);
    // Enable depth buffering for hidden surface removal.
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);
    
    // Cull back faces.
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    
    // Set lighting parameters.
    glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);

    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

    glEnable(GL_LIGHT1);
    glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
    glLightfv(GL_LIGHT1, GL_POSITION, light1_position);

}

/*******************************************************************************
 Sets up/initializes haptic rendering library.
*******************************************************************************/
void initHL()
{
    HDErrorInfo error;
    ghHD = hdInitDevice(HD_DEFAULT_DEVICE);
    if (HD_DEVICE_ERROR(error = hdGetError()))
    {
        hduPrintError(stderr, &error, "Failed to initialize haptic device");
        fprintf(stderr, "Press any key to exit");
        getchar();
        exit(1);
    }   
    
    // Create a haptic context for the device.  The haptic context maintains 
    // the state that persists between frame intervals and is used for
    // haptic rendering.
    ghHLRC = hlCreateContext(ghHD);
    hlMakeCurrent(ghHLRC); 

    // Generate a shape id to hold the axis snap constraint.
    gAxisId = hlGenShapes(1);

    // Add a callback to handle button down in the collision thread.
    hlAddEventCallback(HL_EVENT_1BUTTONDOWN, HL_OBJECT_ANY, HL_COLLISION_THREAD, 
                       buttonDownCollisionThreadCallback, NULL);

    // Initialize the haptic mouse.
    hmInitializeMouse(ghHLRC, "GLUT", "Haptic Material");

    // Save off the initial workspace, since we will be modifying it for the
    // haptic mouse to allow for motion outside of the viewport.
    hlGetDoublev(HL_WORKSPACE, gInitWorkspace);
}

/*******************************************************************************
 Creates the objects that can be seen, felt and dragged around.
*******************************************************************************/
void createDraggableObjects()
{
    // Create a bunch of shapes and add them to the draggable object vector.
    DraggableObject dro;

    dro.hap_stiffness = 0.8;
    dro.hap_damping = 0.0;
    dro.hap_static_friction = 0.0;
    dro.hap_dynamic_friction = 0.0;
    
    // Sphere.
    dro.shapeId = hlGenShapes(1);
    dro.transform = hduMatrix::createTranslation(0,0,0);
    dro.displayList = glGenLists(1);
    glNewList(dro.displayList, GL_COMPILE);
        glutSolidSphere(0.4, 30, 30);
    glEndList();

    draggableObjects.push_back(dro);

    // Tetrahedron.
    dro.shapeId = hlGenShapes(1);
    dro.transform = hduMatrix::createTranslation(-1.3,0,0);
    dro.displayList = glGenLists(1);
    glNewList(dro.displayList, GL_COMPILE);
        glutSolidTetrahedron();
    glEndList();

    draggableObjects.push_back(dro);

    // Dodecahedron.
    dro.shapeId = hlGenShapes(1);
    dro.transform = hduMatrix::createTranslation(1.4,0,0);
    dro.displayList = glGenLists(1);
    glNewList(dro.displayList, GL_COMPILE);
        glPushMatrix();
        glScalef(0.5,0.5,0.5);
        glutSolidDodecahedron();
        glPopMatrix();
    glEndList();

    draggableObjects.push_back(dro);

    // Cube.
    dro.shapeId = hlGenShapes(1);
    dro.transform = hduMatrix::createTranslation(0,1.4,0);
    dro.displayList = glGenLists(1);
    glNewList(dro.displayList, GL_COMPILE);
        glutSolidCube(0.5);
    glEndList();

    draggableObjects.push_back(dro);

    // Cone.
    dro.shapeId = hlGenShapes(1);
    dro.transform = hduMatrix::createTranslation(0,-1.4,0);
    dro.displayList = glGenLists(1);
    glNewList(dro.displayList, GL_COMPILE);
        glutSolidCone(0.4, 0.6, 30, 30);
    glEndList();

    draggableObjects.push_back(dro);

    // Add event callbacks for button down on each of the shapes.
    // Callbacks will set that shape to be the drag object.
    for (int i = 0; i < draggableObjects.size(); ++i)
    {
        // Pass the index of the object as userdata.
        hlAddEventCallback(HL_EVENT_1BUTTONDOWN, 
                           draggableObjects[i].shapeId, 
                           HL_CLIENT_THREAD, 
                           buttonDownClientThreadCallback, 
                           reinterpret_cast<void *>(i));
    }

    // Add an event callback on button to clear the drag object
    // and end dragging.
    hlAddEventCallback(HL_EVENT_1BUTTONUP, HL_OBJECT_ANY, HL_CLIENT_THREAD, 
                       buttonUpClientThreadCallback, NULL);
}

/*******************************************************************************
 Draws the objects that can be seen, felt and dragged around. 
*******************************************************************************/
void drawDraggableObjects()
{
    hlTouchModel(HL_CONTACT);
    hlTouchableFace(HL_FRONT);

    for (int i = 0; i < draggableObjects.size(); ++i)
    {
        const DraggableObject& obj = draggableObjects[i];

        // Position and orient the object.
        glPushMatrix();

        glMultMatrixd(obj.transform);

        // Draw the object graphically.
        glCallList(obj.displayList);

        // Draw the object haptically (but not if it is being dragged).
        if (i != gCurrentDragObj && !hmIsMouseActive())
        {
            hlBeginShape(HL_SHAPE_FEEDBACK_BUFFER, obj.shapeId);

            hlMaterialf(HL_FRONT_AND_BACK, HL_STIFFNESS, obj.hap_stiffness);
            hlMaterialf(HL_FRONT, HL_DAMPING, obj.hap_damping);
            hlMaterialf(HL_FRONT, HL_STATIC_FRICTION, obj.hap_static_friction);
            hlMaterialf(HL_FRONT, HL_DYNAMIC_FRICTION, obj.hap_dynamic_friction);

            glCallList(obj.displayList);

            hlEndShape();
        }

        glPopMatrix();
    }
}

/*******************************************************************************
 Sets the modelview transform from scratch.  Applies the current view 
 orientation and scale. 
*******************************************************************************/
void updateCamera()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();            
    gluLookAt(gCameraPosWC[0], gCameraPosWC[1], gCameraPosWC[2],
              0, 0, 0,
              0, 1, 0);
    
    glTranslatef(gCameraTranslationX, gCameraTranslationY, 0);
    glMultMatrixd(gCameraRotation);

    glTranslatef(obj_pos[0], obj_pos[1], obj_pos[2]);
    glMultMatrixf(view_rotate);

    glScaled(gCameraScale, gCameraScale, gCameraScale);

    updateHapticMapping();

    glutPostRedisplay();
}

/*******************************************************************************
 Uses the current OpenGL viewing transforms to initialize a transform for the
 haptic device workspace so that it's properly mapped to world coordinates.
*******************************************************************************/
void updateHapticMapping(void)
{
    GLdouble modelview[16];
    GLdouble projection[16];
    GLint viewport[4];

    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glGetIntegerv(GL_VIEWPORT, viewport);
    
    // Modify the workspace dimensions mapped to the view so that the user
    // can move outside of the viewport and access the GLUI interface.
    double t = (double) gViewportWidth / gWindowWidth;
    double xMaxProportion = hduLerp(gInitWorkspace[0], gInitWorkspace[3], t);

    hlWorkspace(gInitWorkspace[0], gInitWorkspace[1], gInitWorkspace[2],
                xMaxProportion, gInitWorkspace[4], gInitWorkspace[5]);

    // Fit haptic workspace to view volume.
    hlMatrixMode(HL_TOUCHWORKSPACE);
    hlLoadIdentity();
    hluFitWorkspace(projection);    

    // Compute cursor scale.
    gCursorScale = hluScreenToModelScale(modelview, projection, viewport);
    gCursorScale *= CURSOR_SIZE_PIXELS;

    // Provide the current viewing transforms to HapticMouse so it can
    // map the device to the screen.
    hmSetMouseTransforms(modelview, projection, viewport);
}

/******************************************************************************
 Displays a cursor using the current haptic device proxy transform and the
 mapping between the workspace and world coordinates
******************************************************************************/
void redrawCursor()
{
    static const double kCursorRadius = 0.5;
    static const int kCursorTess = 15;
    HLdouble proxytransform[16];
    
    GLUquadricObj *qobj = 0;
    
    glPushAttrib(GL_CURRENT_BIT | GL_ENABLE_BIT | GL_LIGHTING_BIT);
    glPushMatrix();
    
    if (!gCursorDisplayList)
    {
        gCursorDisplayList = glGenLists(1);
        glNewList(gCursorDisplayList, GL_COMPILE);
        qobj = gluNewQuadric();
        
        gluSphere(qobj, kCursorRadius, kCursorTess, kCursorTess);
        
        gluDeleteQuadric(qobj);
        glEndList();
    }  
    
    // Apply the local position/rotation transform of the haptic device proxy.
    hlGetDoublev(HL_PROXY_TRANSFORM, proxytransform);
    glMultMatrixd(proxytransform);
    
    // Apply the local cursor scale factor.
    glScaled(gCursorScale, gCursorScale, gCursorScale);
    
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glColor3f(0.0, 0.5, 1.0);
    
    glCallList(gCursorDisplayList);
    
    glPopMatrix(); 
    glPopAttrib();
}

/******************************************************************************
 Draws coordinate axes using OpenGL. 
******************************************************************************/
void drawAxes(const hduVector3Dd& gAxisCenter)
{
    // Lines and points are best viewed without OpenGL lighting.
    glPushAttrib(GL_LIGHTING_BIT);
    glDisable(GL_LIGHTING);

    // Draw three lines - one along each coordinate axis from -1 to 1.
    glBegin(GL_LINES); 
    {
        glVertex3f(gAxisCenter[0] - 1,gAxisCenter[1],gAxisCenter[2]);
        glVertex3f(gAxisCenter[0] + 1,gAxisCenter[1],gAxisCenter[2]);
        glVertex3f(gAxisCenter[0],gAxisCenter[1] - 1,gAxisCenter[2]);
        glVertex3f(gAxisCenter[0],gAxisCenter[1] + 1,gAxisCenter[2]);
        glVertex3f(gAxisCenter[0],gAxisCenter[1],gAxisCenter[2] - 1);
        glVertex3f(gAxisCenter[0],gAxisCenter[1],gAxisCenter[2] + 1);
    }
    glEnd();

    // Draw the origin.
    glBegin(GL_POINTS); 
    {
        glVertex3f(gAxisCenter[0],gAxisCenter[1],gAxisCenter[2]);
    } 
    glEnd();

    glPopAttrib();
}

/******************************************************************************
 The main routine for displaying the scene.
******************************************************************************/
void drawScene()
{    
    hlBeginFrame();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Any client thread button callbacks get triggered here.
    hlCheckEvents();

    // Draw any haptic mouse scene objects.
    hmRenderMouseScene();

    // Check if button on stylus is down - if so draw the coordinate axes and 
    // move the drag object.
    HLboolean buttDown;
    hlGetBooleanv(HL_BUTTON1_STATE, &buttDown);

    if (buttDown && !hmIsMouseActive())
    {
        if (gAxisSnap)
        {
            // Graphically render the axes.
            drawAxes(gAxisCenter);

            // Make sure proxy resolution is on.  The event handler
            // turns it off but it must be on for shapes to be felt.
            hlEnable(HL_PROXY_RESOLUTION);

            // Haptically render the coordinate axes as a feedback buffer 
            // shape.
            hlBeginShape(HL_SHAPE_FEEDBACK_BUFFER, gAxisId);

            // Make it a constraint to the cursor will stick to the axes.
            hlTouchModel(HL_CONSTRAINT);

            // Snap distance allows user to pull off of the constraint
            // if the user moves beyond that snap distance.
            hlTouchModelf(HL_SNAP_DISTANCE, 1.5);

            // Call the OpenGL commands to draw the axes, but this time
            // they will be used for haptics.
            drawAxes(gAxisCenter);

            hlEndShape();
        }

        // "Drag" the current drag object, if one is current.
        if (gCurrentDragObj != -1)
        {
            updateDragObjectTransform();
        }
    }

    drawDraggableObjects();

    if (!hmIsMouseActive())
    {
        redrawCursor();
    }

    drawPrompts();

    glutSwapBuffers();

    hlEndFrame();

}

/******************************************************************************
 GLUT callback for responding to mouse button presses. Detecst whether to
 initiate a point snapping, view rotation or view scale.
******************************************************************************/
void glutMouse(int button, int state, int x, int y)
{
    if (state == GLUT_DOWN)
    {
        if (button == GLUT_LEFT_BUTTON)
        {
            gIsRotatingCamera = true;            
        }
        else if (button == GLUT_RIGHT_BUTTON)
        {
            gIsScalingCamera = true;            
        }
        else if (button == GLUT_MIDDLE_BUTTON)
        {
            gIsTranslatingCamera = true;
        }

        gLastMouseX = x;
        gLastMouseY = y;
    }
    else
    {
        gIsRotatingCamera = false;
        gIsScalingCamera = false;
        gIsTranslatingCamera = false;
    }
}

/******************************************************************************* 
 This routine is used by the view rotation code for simulating a virtual
 trackball.  This math computes the z height for a 2D projection onto the
 surface of a 2.5D sphere.  When the input point is near the center of the
 sphere, this routine computes the actual sphere intersection in Z.  When 
 the input point moves towards the outside of the sphere, this routine will 
 solve for a hyperbolic projection, so that it still yields a meaningful answer.
*******************************************************************************/
double projectToTrackball(double radius, double x, double y)
{
    static const double kUnitSphereRadius2D = sqrt(2.0);
    double z;

    double dist = sqrt(x * x + y * y);
    if (dist < radius * kUnitSphereRadius2D / 2.0)
    {
        // Solve for sphere case.
        z = sqrt(radius * radius - dist * dist);
    }
    else
    {
        // Solve for hyperbolic sheet case.
        double t = radius / kUnitSphereRadius2D;
        z = t * t / dist;
    }

    return z;
}

/*******************************************************************************
 GLUT callback for mouse motion, which is used for controlling the view
 rotation and scaling.
*******************************************************************************/
void glutMotion(int x, int y)
{
    if (gIsRotatingCamera)
    {
        static const double kTrackBallRadius = 0.8;   

        hduVector3Dd lastPos;
        lastPos[0] = gLastMouseX * 2.0 / gViewportWidth - 1.0;
        lastPos[1] = (gViewportHeight - gLastMouseY) * 2.0 / gViewportHeight - 1.0;
        lastPos[2] = projectToTrackball(kTrackBallRadius, lastPos[0], lastPos[1]);

        hduVector3Dd currPos;
        currPos[0] = x * 2.0 / gViewportWidth - 1.0;
        currPos[1] = (gViewportHeight - y) * 2.0 / gViewportHeight - 1.0;
        currPos[2] = projectToTrackball(kTrackBallRadius, currPos[0], currPos[1]);

        currPos.normalize();
        lastPos.normalize();

        hduVector3Dd rotateVec = lastPos.crossProduct(currPos);
        
        double rotateAngle = asin(rotateVec.magnitude());
        if (!hduIsEqual(rotateAngle, 0.0, DBL_EPSILON))
        {
            hduMatrix deltaRotation = hduMatrix::createRotation(
                rotateVec, rotateAngle);            
            gCameraRotation.multRight(deltaRotation);
            updateCamera();
        }
    }
    if (gIsTranslatingCamera)
    {
        gCameraTranslationX += 10 * double(x - gLastMouseX)/gViewportWidth;
        gCameraTranslationY -= 10 * double(y - gLastMouseY)/gViewportHeight;

        updateCamera();
    }
    else if (gIsScalingCamera)
    {
        float y1 = gViewportHeight - gLastMouseY;
        float y2 = gViewportHeight - y;

        gCameraScale *= 1 + (y1 - y2) / gViewportHeight;  

        updateCamera();
    }


    gLastMouseX = x;
    gLastMouseY = y;
}

/*******************************************************************************
 GLUT callback for key presses.
*******************************************************************************/
void glutKeyboard(unsigned char key, int x, int y)
{
    switch (key) {
    case 'r':
    case 'R':
        gRotate = !gRotate;
        break;
    case 'a':
    case 'A':
        gAxisSnap = !gAxisSnap;
        break;
    }
}

/******************************************************************************
 Event callback triggered when styus button is depressed.  Called in collision 
 thread to avoid kick.  (If the application instead waits to handle the button 
 down in the client thread, the user can get a kick when the constraint is
 set if the user's hand moved from the position that the button down was
 originally recorded at.)
******************************************************************************/
void HLCALLBACK buttonDownCollisionThreadCallback(HLenum event, HLuint object, 
                                                  HLenum thread, HLcache *cache, 
                                                  void *userdata)
{
    // Don't proceed if the haptic mouse is active.
    if (hmIsMouseActive())
        return;

    if (gAxisSnap)
    {
        // Use the state cache to get the proxy position at the time
        // the event occured.
        hlCacheGetDoublev(cache, HL_PROXY_POSITION, gAxisCenter);

        // Temporarily turn off proxy resolution and set the
        // proxy position at the proxy position from the time
        // the event occured.  With proxy resolution disabled, the
        // proxy will not move unless told, so it will stick
        // to this spot until we place the constraint in the drawScene
        // routine in the client thread.  This basically allows us to hold
        // the proxy in place until the constraint can hold it there.
        hlDisable(HL_PROXY_RESOLUTION);
        hlProxydv(HL_PROXY_POSITION, gAxisCenter);
    }
}


/******************************************************************************
 Event callback triggered when styus button is depressed and touching one of 
 the draggable objects.  This callback is always called in client thread.
******************************************************************************/
void HLCALLBACK buttonDownClientThreadCallback(HLenum event, HLuint object, 
                                               HLenum thread, HLcache *cache, 
                                               void *userdata)
{
    // Don't proceed if the haptic mouse is active.
    if (hmIsMouseActive())
        return;

    assert(gCurrentDragObj == -1);
    assert(object != HL_OBJECT_ANY);

    // Clicked while touching an object, set this object to be the current 
    // object being dragged.  When event callback was registered, we set the 
    // index of the drag object as the user data.
    gCurrentDragObj = reinterpret_cast<int>(userdata);

    // Store off proxy position so we can compute how much it moves each
    // frame (which is how much the drag object should also move).
    hlGetDoublev(HL_PROXY_POSITION, gStartDragProxyPos);
    hlGetDoublev(HL_PROXY_ROTATION, gStartDragProxyRot);

    // Store off initial position and orientation of drag object.
    gStartDragObjTransform = draggableObjects[gCurrentDragObj].transform;
}

/******************************************************************************
 Event callback triggered when styus button is depressed and touching one of 
 the draggable objects.
******************************************************************************/
void HLCALLBACK buttonUpClientThreadCallback(HLenum event, HLuint object, 
                                             HLenum thread, HLcache *cache, 
                                             void *userdata)
{
    // Button up, done dragging, clear current drag object.
    gCurrentDragObj = -1;
}

/******************************************************************************
 Calculates updated object transform for drag object based on changes to
 proxy transform.
******************************************************************************/
void updateDragObjectTransform()
{
    assert(gCurrentDragObj >= 0 && 
           gCurrentDragObj < draggableObjects.size());

    // Calculated delta between current proxy pos and proxy pos at start 
    // of drag.
    hduVector3Dd proxyPos;
    hlGetDoublev(HL_PROXY_POSITION, proxyPos);
    hduVector3Dd dragDeltaTransl = proxyPos - gStartDragProxyPos;

    // Same for rotation.
    hduMatrix deltaRotMat;
    if (gRotate)
    {
        hduQuaternion proxyRotq;
        hlGetDoublev(HL_PROXY_ROTATION, proxyRotq);
        hduQuaternion dragDeltaRot = gStartDragProxyRot.inverse() * proxyRotq;
        dragDeltaRot.normalize();
        dragDeltaRot.toRotationMatrix(deltaRotMat);

        // Want to rotate about the proxy position, not the origin,
        // so need to translate to/from proxy pos.
        hduMatrix toProxy = hduMatrix::createTranslation(-gStartDragProxyPos);
        hduMatrix fromProxy = hduMatrix::createTranslation(gStartDragProxyPos);
        deltaRotMat = toProxy * deltaRotMat * fromProxy;
    }

    // Compose rotation and translation deltas.
    hduMatrix deltaMat = deltaRotMat * hduMatrix::createTranslation(dragDeltaTransl);

    // Apply these deltas to the drag object transform.
    draggableObjects[gCurrentDragObj].transform = gStartDragObjTransform * deltaMat;
}

/******************************************************************************
 Draws a string using OpenGL.
******************************************************************************/
void drawString(const char* string)
{
    for (;*string != '\0';++string)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *string);
    }
}

/******************************************************************************
 Draws string prompts at the bottom of the screen.
******************************************************************************/
void drawPrompts()
{
    glPushAttrib(GL_ENABLE_BIT);
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, gViewportWidth, 0, gViewportHeight, 0, 1);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glRasterPos2f(4, 54);
    drawString("Touch an object and press button to drag.\n");
    glRasterPos2f(4, 36);
    drawString("Use mouse to rotate, pan, zoom.");
    glRasterPos2f(4, 18);
    drawString("(A) to toggle axis snap, (R) to toggle rotation.");
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glPopAttrib();
}

/******************************************************************************/
