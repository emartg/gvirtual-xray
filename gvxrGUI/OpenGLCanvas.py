import os.path
import errno, sys

from math import sqrt, pow, floor
import random
import numpy as np

import wx
from wx import glcanvas

# The main GUI application
import wxApp;

import GlobalParameters as GP;

#import ParamBox as PB
# TODO
# Handle the object in user interaction
# Save the L-buffer of node
# Save the L-buffer of branch

# Material property
# Scale the visualisation to input object
# Set scale of data, cm, mm, m
# Set position by hand using the GUI
# Set position by hand using the command line
# Load a scenegraph using the GUI
# Load a scenegraph using the command line
# Build an EXE file
# Simulate CT scan acquisition parallel beam
# Simulate CT scan acquisition cone beam

if getattr(sys, 'frozen', False): # we are running in a bundle
    dir_path = sys._MEIPASS # This is where the files are unpacked to
else: # normal Python environment
    dir_path = os.path.dirname(os.path.abspath(__file__))

try:
    # The Python OpenGL package can be found at
    # http://PyOpenGL.sourceforge.net/
    from OpenGL.GL import *
    from OpenGL.GLUT import *
    from OpenGL.GLU import *
except ImportError:
    print("Could not import the Python OpenGL package")
    exit()

from gvxrPython3 import gvxr

import Viewport as vp;


input_3D_filename = dir_path + '/welsh-dragon-small.stl'


class OpenGlCanvas(glcanvas.GLCanvas):
    def __init__(self, parent): #, slider_container, plot_panel):
        print("OpenGlCanvas.__init__")

        self.GLinitialized = False
        self.no_load_data_error_detected = False;
        self.current_viewport = None;
        self.closest_object = 0; # Unknown

        attribList = (glcanvas.WX_GL_RGBA, # RGBA
                      glcanvas.WX_GL_DOUBLEBUFFER, # Double Buffered
                      glcanvas.WX_GL_DEPTH_SIZE, 24, # 24 bit
                      glcanvas.WX_GL_SAMPLE_BUFFERS, True)

        # Create the canvas
        glcanvas.GLCanvas.__init__(self, parent, -1, size = wx.Size(400,400), attribList=attribList);

        # Create the context
        self.context = glcanvas.GLContext(self)

        self.GLinitialized = False;
        self.zoom = 0.0;

        #
        # Set the event handlers.
        self.Bind(wx.EVT_SIZE,       self.processSizeEvent)
        self.Bind(wx.EVT_PAINT,      self.processPaintEvent)
        self.Bind(wx.EVT_LEFT_DOWN,  self.processLeftButtonDownEvent)
        self.Bind(wx.EVT_LEFT_UP,    self.processLeftButtonUpEvent)
        self.Bind(wx.EVT_MOTION,     self.processMouseMotionEvent)
        self.Bind(wx.EVT_MOUSEWHEEL, self.processMouseWheelEvent)
        self.Bind(wx.EVT_CLOSE,      self.processCanvasCloseEvent)

    def setObjectScale(self, aScale):
        if GP.mesh_unit_of_length != aScale:
            print ("Set scale from ", GP.mesh_unit_of_length, " to ", aScale);
            original_scale = gvxr.getUnitOfLength(GP.mesh_unit_of_length);
            target_scale = gvxr.getUnitOfLength(aScale);
            print ("from ", original_scale, " to ", target_scale);
            GP.mesh_unit_of_length = aScale;
            ratio = target_scale / original_scale;
            print (ratio)
            gvxr.scaleScene(ratio, ratio, ratio);
            self.Refresh(True);
            self.Update();

    def resetViewports(self):
        if self.GLinitialized:
            self.border = 5;

            self.viewport_set = [];

            view_width  = floor(self.Size.x / 2)
            view_height = floor(self.Size.y / 2);

            ratio = view_width / view_height;
            offset = 5.1;

            source_position    = gvxr.getSourcePosition("mm");
            detector_position  = gvxr.getDetectorPosition("mm");
            detector_up_vector = gvxr.getDetectorUpVector();

            detector_width  = gvxr.getDetectorSize("mm")[0];
            detector_height = gvxr.getDetectorSize("mm")[1] ;

            length = sqrt(
                pow(source_position[0] - detector_position[0], 2) +
                pow(source_position[1] - detector_position[1], 2) +
                pow(source_position[2] - detector_position[2], 2));

            centre = [(source_position[0] + detector_position[0]) / 2.0,
                (source_position[1] + detector_position[1]) / 2.0,
                (source_position[2] + detector_position[2]) / 2.0];

            print("detector_width:  ", detector_width);
            print("detector_height: ", detector_height);
            print("length:          ", length);

            print("source_position:    ", source_position);
            print("detector_position:  ", detector_position);

            scale = 1.75
            if ratio > 1.0:
                xmin = centre[0] - scale * length * ratio;
                xmax = centre[0] + scale * length * ratio;
                ymin = centre[1] - scale * length;
                ymax = centre[1] + scale * length;
                zmin = centre[2] - scale * length;
                zmax = centre[2] + scale * length;
            else:
                xmin = centre[0] - scale * length;
                xmax = centre[0] + scale * length;
                ymin = centre[1] - scale * length / ratio;
                ymax = centre[1] + scale * length / ratio;
                zmin = centre[2] - scale * length * length;
                zmax = centre[2] + scale * length * length;

            if self.zoom < 0.000001:
                self.zoom = length * scale;

            # Bottom left
            # xy-plane
            self.viewport_set.append(
                vp.Viewport(0, 0, view_width - self.border, view_height  - self.border,
                0)
            );
            self.viewport_set[-1].lookAt(centre[0], centre[1], centre[2] + length * 1.5,
                centre[0], centre[1], centre[2] - length * 1.5,
                0, 1, 0);

            self.viewport_set[-1].orthographic(xmin, xmax, ymin, ymax, zmin, zmax);

            # Top left
            # yz-plane
            self.viewport_set.append(
                vp.Viewport(0, view_height + self.border, view_width - self.border, view_height - self.border,
                1)
            );
            self.viewport_set[-1].lookAt(centre[0] - length * 1.5, centre[1], centre[2],
                centre[0] + length * 1.5, centre[1], centre[2],
                0, 0, 1);
            self.viewport_set[-1].orthographic(xmin, xmax, ymin, ymax, zmin, zmax);

            # Bottom right
            # Perspective
            self.viewport_set.append(
                vp.Viewport(view_width + self.border, 0, view_width - self.border, view_height - self.border,
                3)
            );

            self.viewport_set[-1].lookAt(0,
                0,
                self.zoom,
                0,
                0,
                0,
                1, 0, 0);

            self.viewport_set[-1].perspective(45,
                (self.Size.x/self.Size.y),
                0.01 * length, 50.0 * length);

            # Top right
            # zx-plane
            self.viewport_set.append(
                vp.Viewport(view_width + self.border, view_height + self.border, view_width - self.border, view_height - self.border,
                2)
            );
            self.viewport_set[-1].lookAt(centre[0], centre[1] + length * 1.5, centre[2],
                centre[0], centre[1] - length * 1.5, centre[2],
                1, 0, 0);
            self.viewport_set[-1].orthographic(xmin, xmax, ymin, ymax, zmin, zmax);

    def GetGLExtents(self):
        """Get the extents of the OpenGL canvas."""
        print("OpenGlCanvas.GetGLExtents")
        return self.GetClientSize()

    def OnReshape(self, width, height):
        """Reshape the OpenGL viewport based on the dimensions of the window."""
        print("OpenGlCanvas.OnReshape")
        self.resetViewports();
        pass # Do nothing, to avoid flashing on MSWin


    def processSizeEvent(self, event):
        print("OpenGlCanvas.processSizeEvent")
        """Process the resize event."""
        if self.context:
            # Make sure the frame is shown before calling SetCurrent.
            self.Show()
            self.SetCurrent(self.context)

            size = self.GetGLExtents()
            self.OnReshape(size.width, size.height)
            self.Refresh(True);
            self.Update();
        event.Skip()

    def processPaintEvent(self, event):
        print("OpenGlCanvas.processPaintEvent")
        """Process the drawing event."""
        self.SetCurrent(self.context)

        # This is a 'perfect' time to initialize OpenGL ... only if we need to
        self.initGL()

        self.__OnDraw__(None)
        event.Skip()


    def processLeftButtonDownEvent(self, event):

        # Get the x/y position (express y in OpenGL' system)
        x = event.GetX();
        y = self.Size.y - event.GetY();

        # Parse all the viewports
        self.current_viewport = None;
        for viewport in self.viewport_set:
            if viewport.isEventInViewport(x, y):
                self.current_viewport = viewport;

                if viewport.getType() == 3:
                    gvxr.startArcBallRotation(y - viewport.y, x - viewport.x);
                else:
                    X, Y, Z = self.current_viewport.screenCoords2worldCoords(x, y);
                    self.detectClosestObject(X, Y, Z);

        #print(self.current_viewport.getTypeLabel())

        # Mark the event as processed
        event.Skip()

    def moveClosestObject(self, X, Y, Z):
        detector_position = np.array(gvxr.getDetectorPosition("mm"));
        source_position   = np.array(gvxr.getSourcePosition("mm"));

        if self.closest_object == 0: # Unknown
            return

        elif self.closest_object == 1: # Detector
            if self.current_viewport.getType() == 0: # xy-plane
                detector_position[0] = X;
                detector_position[1] = Y;
            elif self.current_viewport.getType() == 1: # yz-plane
                detector_position[1] = Y;
                detector_position[2] = Z;
            elif self.current_viewport.getType() == 2: # zx-plane
                detector_position[0] = X;
                detector_position[2] = Z;

            gvxr.setDetectorPosition(detector_position[0], detector_position[1], detector_position[2], "mm");

        elif self.closest_object == 2: # Source

            if self.current_viewport.getType() == 0: # xy-plane
                source_position[0] = X;
                source_position[1] = Y;
            elif self.current_viewport.getType() == 1: # yz-plane
                source_position[1] = Y;
                source_position[2] = Z;
            elif self.current_viewport.getType() == 2: # zx-plane
                source_position[0] = X;
                source_position[2] = Z;

            gvxr.setSourcePosition(source_position[0], source_position[1], source_position[2], "mm");

        elif self.closest_object == 3: # Object
            return


        up_vector = np.array(gvxr.getDetectorUpVector());
        look_at = np.subtract(detector_position, source_position);
        look_at /= np.linalg.norm(look_at);
        cross1 = np.cross(up_vector, look_at);
        cross2 = np.cross(cross1, look_at);

        # Try to preserve consistency
        main_up_vector_axis = 0;

        if abs(up_vector[main_up_vector_axis]) < abs(up_vector[1]):
            main_up_vector_axis = 1;
        if abs(up_vector[main_up_vector_axis]) < abs(up_vector[2]):
            main_up_vector_axis = 2;

        main_cross2_axis = 0;

        if abs(cross2[main_cross2_axis]) < abs(cross2[1]):
            main_cross2_axis = 1;
        if abs(cross2[main_cross2_axis]) < abs(cross2[2]):
            main_cross2_axis = 2;

        if main_cross2_axis == main_up_vector_axis:
            if np.sign(cross2[main_cross2_axis]) != np.sign(up_vector[main_up_vector_axis]):

                cross2[0] *= -1;
                cross2[1] *= -1;
                cross2[2] *= -1;

        gvxr.setDetectorUpVector(cross2[0], cross2[1], cross2[2]);
        #print(cross2, up_vector);

        # Redisplay
        wxApp.g_wxapp.updateXRayImage();

    def detectClosestObject(self, X, Y, Z):

        source_position    = gvxr.getSourcePosition("mm");
        detector_position  = gvxr.getDetectorPosition("mm");

        squared_cursor_to_object_distance   = float('Inf');

        squared_cursor_to_detector_distance = (X - detector_position[0]) * (X - detector_position[0]) + (Y - detector_position[1]) * (Y - detector_position[1]) + (Z - detector_position[2]) * (Z - detector_position[2]);

        squared_cursor_to_source_distance   = (X - source_position[0]) * (X - source_position[0]) + (Y - source_position[1]) * (Y - source_position[1]) + (Z - source_position[2]) * (Z - source_position[2]);

        self.closest_object = 0; # Unknown

        #print (X, Y, Z)
        if squared_cursor_to_detector_distance < squared_cursor_to_source_distance:
            self.closest_object = 1; # Detector
            #print ("Detector closer", squared_cursor_to_detector_distance, squared_cursor_to_source_distance);

            if squared_cursor_to_object_distance < squared_cursor_to_detector_distance:
                self.closest_object = 3; # Object

        else:
            self.closest_object = 2; # Source
            #print ("Source closer", squared_cursor_to_detector_distance, squared_cursor_to_source_distance);

            if squared_cursor_to_object_distance < squared_cursor_to_source_distance:
                self.closest_object = 3; # Object



    def processLeftButtonUpEvent(self, event):

        # Get the x/y position (express y in OpenGL' system)
        x = event.GetX();
        y = self.Size.y - event.GetY();

        if self.current_viewport != None:
            if self.current_viewport.getType() == 3:
                gvxr.stopArcBallRotation();
            else:
                self.closest_object = 0;

            self.current_viewport = None;

        # Parse all the viewports
        '''for viewport in self.viewport_set:

            # The event occur in the 3-D visualisation view
            if viewport.isEventInViewport(x, y) and viewport.getType() == 3:
                gvxr.stopArcBallRotation();'''

        self.current_viewport = None;

        # Mark the event as processed
        event.Skip()


    def processMouseMotionEvent(self, event):

        # Get the x/y position (express y in OpenGL' system)
        x = event.GetX();
        y = self.Size.y - event.GetY();

        if self.current_viewport != None:
            if self.current_viewport.getType() == 3:

                # Arc-ball rotation is in use
                if gvxr.usingArcBallRotation():

                    gvxr.cursorPositionCallback(y - self.current_viewport.y, x - self.current_viewport.x, self.current_viewport.h, self.current_viewport.w);
            else:
                X, Y, Z = self.current_viewport.screenCoords2worldCoords(x, y);
                self.moveClosestObject(X, Y, Z);

            # Redisplay
            self.Refresh(True)
            self.Update()

        # Mark the event as processed
        event.Skip()

    # Zoom in and out in the 3-D visualisation view
    def processMouseWheelEvent(self, event):

        # Get the x/y position (express y in OpenGL' system)
        x = event.GetX();
        y = self.Size.y - event.GetY();

        # Parse all the viewports
        for viewport in self.viewport_set:
            if viewport.isEventInViewport(x, y):
                if viewport.getType() == 3:

                    # Zoom in and out
                    if event.GetWheelRotation() > 0:
                        self.zoom += 10;
                    else:
                        self.zoom -= 10;

                    viewport.lookAt(0,
                        0,
                        self.zoom,
                        0,
                        0,
                        0,
                        1, 0, 0);
                else:
                    if event.GetWheelRotation() > 0:
                        viewport.zoomOut();
                    else:
                        viewport.zoomIn();
                # Redisplay
                self.Refresh(True)
                self.Update()

        # Mark the event as processed
        event.Skip()

    # Close the application
    def processCanvasCloseEvent(self, event):
        event.Skip()
        exit();

    def initGL(self):

        if self.GLinitialized == False:
            self.GLinitialized = True;

            print("initGL")
            self.SetCurrent(self.context)
            gvxr.autoCreateOpenGLContext(False);
            # gvxr.initializeGLEW();
            # gvxr.initShader();
            glClearColor(0.5, 0.5, 0.5, 1.0)
            self.Bind(wx.EVT_PAINT, self.__OnDraw__)

            # Init X-rays
            gvxr.setSourcePosition(GP.source_position[0],
                    GP.source_position[1],
                    GP.source_position[2],
                    GP.detector_unit_of_length);

            gvxr.setDetectorPosition (GP.detector_position[0],
                    GP.detector_position[1],
                    GP.detector_position[2],
                    GP.detector_unit_of_length);

            gvxr.setDetectorUpVector (GP.detector_up_vector[0],
                    GP.detector_up_vector[1],
                    GP.detector_up_vector[2]);

            gvxr.setDetectorNumberOfPixels (GP.detector_number_of_pixels[0],
                    GP.detector_number_of_pixels[1]);

            gvxr.setDetectorPixelSize(GP.detector_pixel_size[0],
                    GP.detector_pixel_size[1],
                    GP.detector_unit_of_length);

            gvxr.usePointSource();
            #gvxr.useParallelBeam();
            gvxr.setMonoChromatic(GP.beam_energy, GP.beam_energy_unit, 1);
            #PB.g_param_box.m_slider1.SetValue(GP.beam_energy);
            #PB.g_param_box.m_choice1.SetSelection(1);


            # Load the data
            print("Load the data");

            # The input file does not exist
            if os.path.exists(input_3D_filename) == False:
                raise FileNotFoundError(errno.ENOENT, os.strerror(errno.ENOENT), input_3D_filename);

            gvxr.loadSceneGraph(input_3D_filename, GP.mesh_unit_of_length);
            wxApp.g_wxapp.setObjectScale(GP.mesh_unit_of_length);

            node_label_set = [];
            node_label_set.append('root');

            # The list is not empty
            while (len(node_label_set)):

                # Get the last node
                last_node = node_label_set[-1];

                # Initialise the material properties
                #print("Set ", label, "'s Hounsfield unit");
                #gvxr.setHU(label, 1000)
                Z = gvxr.getElementAtomicNumber("H");
                gvxr.setElement(last_node, gvxr.getElementName(Z));

                # Change the node colour to a random colour
                gvxr.setColour(last_node, random.uniform(0, 1), random.uniform(0, 1), random.uniform(0, 1), 1.0);

                # Remove it from the list
                node_label_set.pop();

                # Add its Children
                for i in range(gvxr.getNumberOfChildren(last_node)):
                    node_label_set.append(gvxr.getChildLabel(last_node, i));

            #self.slider_container.loadTreeView();
            #self.slider_container.parent = self;

            print("Move root to the centre");
            gvxr.moveToCentre('root');

            # Compute an X-ray image
            print("Compute an X-ray image");
            gvxr.enableArtefactFilteringOnGPU();


            wxApp.g_wxapp.updateXRayImage();
            self.no_load_data_error_detected = True;

            #gvxr.renderLoop();

            self.resetViewports();



    def __OnDraw__(self, event):
        self.initGL();

        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT)

        # The data is loaded properly
        if self.no_load_data_error_detected:
            # Display the 4 views
            for viewport in self.viewport_set:
                viewport.glViewport();
                viewport.loadProjectionMatrix();
                viewport.loadModelviewMatrix();

                if viewport.getType() != 3:
                    gvxr.displayScene(False);
                else:
                    gvxr.displayScene(True);

        self.SwapBuffers()
