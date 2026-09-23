 #!/usr/bin/env python3

import gvxrPython3 as gvxr

# Print the libraries' version
print (gvxr.getVersionOfSimpleGVXR());
print (gvxr.getVersionOfCoreGVXR());

# Create an OpenGL context
gvxr.createWindow();
gvxr.setWindowSize(512, 512);

# Set the position of the X-ray source
gvxr.setSourcePosition(-40.0,  0.0, 0.0, "cm");

# Set the shape of the X-ray source (here an infinitely small point)
gvxr.usePointSource();

# The spectrum of the X-ray beam
# (here a monochromatic spectrum)
gvxr.setMonoChromatic(0.08, "MeV", 1000);

# Set the position of the X-ray detector (film/cassette)
gvxr.setDetectorPosition(10.0, 0.0, 0.0, "cm");

# Set the orientation of the X-ray detector (film/cassette)
gvxr.setDetectorUpVector(0, 0, -1);

# Set the number of pixel of the X-ray detector
gvxr.setDetectorNumberOfPixels(640, 320);

# Set the distance between two successive pixels
gvxr.setDetectorPixelSize(0.5, 0.5, "mm");

# Load a polygon mesh from a STL file as a scenegraph
gvxr.loadSceneGraph("welsh-dragon-small.stl", "mm");

# Set the material properties of all the nodes within the scenegraph
for i in range(gvxr.getNumberOfChildren("root")):
    label = gvxr.getChildLabel("root", i);
    gvxr.moveToCentre(label);
    gvxr.setHU(label, 1000);

# Compute an X-ray image(planar projection/radiograph)
gvxr.computeXRayImage();

# Save the image into a image file
gvxr.saveLastXRayImage("welsh-dragon-xray_python.tif");
