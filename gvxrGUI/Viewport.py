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

class Viewport:
    # x,y,w,h in screen coordinates
    # GLdouble left,  GLdouble right,  GLdouble bottom,  GLdouble top,  GLdouble nearVal,  GLdouble farVal
    # the type 0 = xy-plane, 1 = yz-plane, 2 = zx-plane, 3 = perspective
    def __init__(self, x, y, w, h, type):
        self.x = x;
        self.y = y;
        self.w = w;
        self.h = h;

        self.type = type;

        self.use_perspective = False;
        self.use_orthographic = False;


    def zoomIn(self):
        if self.use_orthographic:
            self.left   *= 0.9;
            self.right  *= 0.9;
            self.bottom *= 0.9
            self.top    *= 0.9;
        self.loadProjectionMatrix();

    def zoomOut(self):
        if self.use_orthographic:
            self.left   *= 1.1;
            self.right  *= 1.1;
            self.bottom *= 1.1
            self.top    *= 1.1;
        self.loadProjectionMatrix();


    def lookAt(self, eye_x, eye_y, eye_z, centre_x, centre_y, centre_z, up_x, up_y, up_z):
        self.eye = [eye_x, eye_y, eye_z];
        self.centre = [centre_x, centre_y, centre_z];
        self.up = [up_x, up_y, up_z];


    def perspective(self, fovy, aspect, zNear, zFar):
        self.use_perspective = True;
        self.use_orthographic = False;

        self.fovy = fovy;
        self.aspect = aspect;
        self.zNear = zNear;
        self.zFar = zFar;

    def orthographic(self, left, right, bottom, top, near, far):
        self.use_perspective = False;
        self.use_orthographic = True;

        self.left = left;
        self.right = right;
        self.bottom = bottom;
        self.top = top;
        self.zNear = near;
        self.zFar = far;

    def glViewport(self):
        glViewport(self.x, self.y, self.w, self.h);


    def loadProjectionMatrix(self):
        if self.use_perspective:
            gvxr.perspective(self.fovy, self.aspect, self.zNear, self.zFar);
        elif self.use_orthographic:
            gvxr.orthographic(self.left,
                self.right,
                self.bottom,
                self.top,
                self.zNear,
                self.zFar);

    def loadModelviewMatrix(self):
        gvxr.lookAt(self.eye[0], self.eye[1], self.eye[2],
            self.centre[0], self.centre[1], self.centre[2],
            self.up[0], self.up[1], self.up[2]);

    def isEventInViewport(self, x, y):
        if self.x <= x and x <= self.x + self.w and \
                self.y <= y and y <= self.y + self.h:
            return True;
        else:
            return False;

    def screenCoords2worldCoords(self, x, y):
        X = 0;
        Y = 0;
        Z = 0;
        
        normalised_x = (x - self.x) / self.w;
        normalised_y = (y - self.y) / self.h;

        #print (self.type)
        if self.type == 0: # xy-plane
            X = normalised_x * (self.right - self.left) + self.left;
            Y = normalised_y * (self.top - self.bottom) + self.bottom;
        elif self.type == 1: # yz-plane
            Y = (1.0 - normalised_x) * (self.right - self.left) + self.left;
            Z = normalised_y * (self.top - self.bottom) + self.bottom;
        elif self.type == 2: # zx-plane
            Z = normalised_x * (self.right - self.left) + self.left;
            X = normalised_y * (self.top - self.bottom) + self.bottom;
        elif self.type == 3:
            raise ("Operation not permitted")

        return  X, Y, Z;

    def getType(self):
        return self.type;

    def getTypeLabel(self):
        if self.type == 0:
            return "xy-plane";
        elif self.type == 1:
            return "yz-plane";
        elif self.type == 2:
            return "zx-plane";
        elif self.type == 3:
            return "perspective";
        else:
            return "Unknown";
