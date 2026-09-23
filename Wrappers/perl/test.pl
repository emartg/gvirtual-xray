#!/usr/bin/env perl

use gvxrPerl;

# Print the libraries' version
print gvxrPerl::getVersionOfCoreGVXR(), "\n";
print gvxrPerl::getVersionOfSimpleGVXR(), "\n";

# Create an OpenGL context
print "Create an OpenGL context\n";
gvxrPerl::createWindow();
gvxrPerl::setWindowSize(512, 512);


# Set up the beam
print "Set up the beam\n";
gvxrPerl::setSourcePosition(-40.0,  0.0, 0.0, "cm");
gvxrPerl::usePointSource();
#gvxrPerl::useParallelBeam();
gvxrPerl::setMonoChromatic(0.08, "MeV", 1000);

# Set up the detector
print "Set up the detector\n";
gvxrPerl::setDetectorPosition(10.0, 0.0, 0.0, "cm");
gvxrPerl::setDetectorUpVector(0, 1, 0);
gvxrPerl::setDetectorNumberOfPixels(640, 320);
gvxrPerl::setDetectorPixelSize(0.5, 0.5, "mm");

# Load the data
print "Load the data\n";
gvxrPerl::loadSceneGraph("../data/welsh-dragon-small.stl", "mm");

# Process every node
for ($count = 0 ; $count < gvxrPerl::getNumberOfChildren("root"); $count++)
{
    $label = gvxrPerl::getChildLabel("root", $count);

    print "Move ", $label, " to the centre\n";
    gvxrPerl::moveToCentre($label);

    #print "Move ", $label, " to the center\n";
    #gvxrPerl::moveToCenter($label);

    print "Set ", $label, "'s Hounsfield unit\n";
    gvxrPerl::setHU($label, 1000);
}

# Compute an X-ray image
print("Compute an X-ray image\n");
gvxrPerl::disableArtefactFiltering();
$x_ray_image = gvxrPerl::computeXRayImage();

# Save the last image into a file
print("Save the last image into a file\n");
gvxrPerl::saveLastXRayImage();
gvxrPerl::saveLastLBuffer();

# Display the 3D scene (no event loop)
gvxrPerl::displayScene();

# Display the 3D scene (no event loop)
# Run an interactive loop
# (can rotate the 3D scene and zoom-in)
# Keys are:
# Q/Escape: to quit the event loop (does not close the window)
# B: display/hide the X-ray beam
# W: display the polygon meshes in solid or wireframe
# N: display the X-ray image in negative or positive
# H: display/hide the X-ray detector
gvxrPerl::renderLoop();
