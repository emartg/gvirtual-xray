#!/usr/bin/env python3


from PIL import Image
import numpy as np

import gvxrPython as gvxr

# Create an OpenGL context
print("Create an OpenGL context")
gvxr.createWindow();
gvxr.setWindowSize(512, 512);


# Set up the beam
print("Set up the beam")
gvxr.setSourcePosition(-40.0,  0.0, 0.0, "cm");
gvxr.usePointSource();
#gvxr.useParallelBeam();
gvxr.setMonoChromatic(0.08, "MeV", 1000);

# Set up the detector
print("Set up the detector");
gvxr.setDetectorPosition(40.0, 0.0, 0.0, "cm");
gvxr.setDetectorUpVector(0, 1, 0);
gvxr.setDetectorNumberOfPixels(640, 640);
gvxr.setDetectorPixelSize(0.5, 0.5, "mm");

# Load the data
print("Load the data");
gvxr.loadSceneGraph("hand.dae", "mm");

print ("Number of meshes in scenegraph:\t" + str(len(gvxr.getMeshLabelSet())))
for mesh in gvxr.getMeshLabelSet():
    #print("Set the mesh's Hounsfield unit");
    print(mesh)
    gvxr.setHU(mesh, 1000)

#exit()

gvxr.rotateNode("node-Mid_Prox", -45, 0.0, 1.0, 0.0);
gvxr.rotateNode("node-Mid_Midd", -45, 0.0, 1.0, 0.0);
gvxr.rotateNode("node-Mid_Dist", -45, 0.0, 1.0, 0.0);

gvxr.rotateNode("node-Ind_Prox", -45, 0.0, 1.0, 0.0);
gvxr.rotateNode("node-Ind_Midd", -45, 0.0, 1.0, 0.0);
gvxr.rotateNode("node-Ind_Dist", -45, 0.0, 1.0, 0.0);

gvxr.rotateNode("node-Lit_Prox", -45, 0.0, 1.0, 0.0);
gvxr.rotateNode("node-Lit_Midd", -45, 0.0, 1.0, 0.0);
gvxr.rotateNode("node-Lit_Dist", -45, 0.0, 1.0, 0.0);

gvxr.rotateNode("node-Thi_Prox", -45, 0.0, 1.0, 0.0);
gvxr.rotateNode("node-Thi_Midd", -45, 0.0, 1.0, 0.0);
gvxr.rotateNode("node-Thi_Dist", -45, 0.0, 1.0, 0.0);

gvxr.rotateNode("node-Thu_Prox", -45, 0.0, 1.0, 0.0);
gvxr.rotateNode("node-Thu_Dist", -45, 0.0, 1.0, 0.0);

gvxr.moveToCentre();
gvxr.scaleScene(600, 600, 600);


# Compute an X-ray image
print("Compute an X-ray image");
gvxr.computeXRayImage();

# Save the last image into a file
print("Save the last image into a file");
gvxr.saveLastXRayImage();

# Retrieve the image in Numpy's 2D array format
np_image = gvxr.getNumpyImage();

# Normalise the image between 0 and 255 (this is for PIL)
np_normalised_image = (255 * (np_image-np_image.min())/np_image.max()).astype(np.int8);

# Convert the Numpy array into a PIL image
img = Image.fromarray(np_normalised_image.astype(np.ubyte));

# Save the PIL image
img.save('my.png')

# Show the image
img.show()

# Display the 3D scene (no event loop)
gvxr.displayScene();


# Display the 3D scene (no event loop)
# Run an interactive loop 
# (can rotate the 3D scene and zoom-in)
# Keys are:
# Q/Escape: to quit the event loop (does not close the window)
# B: display/hide the X-ray beam
# W: display the polygon meshes in solid or wireframe
# N: display the X-ray image in negative or positive
# H: display/hide the X-ray detector
gvxr.renderLoop();
