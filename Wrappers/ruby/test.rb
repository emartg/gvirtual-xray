#!/usr/bin/env ruby

# Import gVirtualXRay
require 'gvxrRuby'

require 'matplotlib'
Matplotlib.use("TkAgg")

require 'matplotlib/pyplot'
plt = Matplotlib::Pyplot

# Print the libraries' version
puts(GvxrRuby.getVersionOfSimpleGVXR())
puts(GvxrRuby.getVersionOfCoreGVXR())

# Create an OpenGL context
puts("Create an OpenGL context")
GvxrRuby.createWindow();
GvxrRuby.setWindowSize(512, 512);

# Set up the beam
puts("Set up the beam")
GvxrRuby.setSourcePosition(-40.0,  0.0, 0.0, "cm");
GvxrRuby.usePointSource();
#GvxrRuby.useParallelBeam();
GvxrRuby.setMonoChromatic(0.08, "MeV", 1000);

# Set up the detector
puts("Set up the detector");
GvxrRuby.setDetectorPosition(10.0, 0.0, 0.0, "cm");
GvxrRuby.setDetectorUpVector(0, 0, -1);
GvxrRuby.setDetectorNumberOfPixels(640, 320);
GvxrRuby.setDetectorPixelSize(0.5, 0.5, "mm");

# Load the data
puts("Load the data");
GvxrRuby.loadSceneGraph("../data/welsh-dragon-small.stl", "mm");

# Process every node
for i in 0..(GvxrRuby.getNumberOfChildren("root")-1) do
    # Get the label
    puts("#{i} / #{GvxrRuby.getNumberOfChildren("root")}")
    label = GvxrRuby.getChildLabel('root', i);

    puts("Move #{label} to the centre");
    GvxrRuby.moveToCentre(label);

    #put("Move the mesh to the center");
    #GvxrRuby.moveToCenter(label);

    puts("Set #{label} 's Hounsfield unit");
    GvxrRuby.setHU(label, 1000);
end

# Compute an X-ray image
puts("Compute an X-ray image");
GvxrRuby.disableArtefactFiltering();
# Not working anymore GvxrRuby.enableArtefactFilteringOnGPU();
# Not working anymore GvxrRuby.enableArtefactFilteringOnCPU();
x_ray_image = GvxrRuby.computeXRayImage();

# Save the last image into a file
puts("Save the last image into a file");
GvxrRuby.saveLastXRayImage();
GvxrRuby.saveLastLBuffer();

# Display the image with Matplotlib
plt.imshow(x_ray_image, cmap="gray");
plt.colorbar();
plt.show();

# Display the 3D scene (no event loop)
GvxrRuby.displayScene();

# Display the 3D scene (no event loop)
# Run an interactive loop
# (can rotate the 3D scene and zoom-in)
# Keys are:
# Q/Escape: to quit the event loop (does not close the window)
# B: display/hide the X-ray beam
# W: display the polygon meshes in solid or wireframe
# N: display the X-ray image in negative or positive
# H: display/hide the X-ray detector
GvxrRuby.renderLoop();
