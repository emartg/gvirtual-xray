#!/usr/bin/env tclsh

load ./gvxrTcl.so

# Print the libraries' version
puts [getVersionOfSimpleGVXR]
puts [getVersionOfCoreGVXR]

# Create an OpenGL context
puts "Create an OpenGL context"
createWindow
setWindowSize 512 512


# Set up the beam
puts "Set up the beam"
setSourcePosition -40.0  0.0 0.0 "cm"
usePointSource
#useParallelBeam
setMonoChromatic 0.08 "MeV" 1000

# Set up the detector
puts "Set up the detector"
setDetectorPosition 10.0 0.0 0.0 "cm"
setDetectorUpVector 0 0 -1
setDetectorNumberOfPixels 640 320
setDetectorPixelSize 0.5 0.5 "mm"

# Load the data
puts "Load the data"
loadSceneGraph "../data/welsh-dragon-small.stl" "mm"

# Process every node
for {set i 0} {$i < [getNumberOfChildren "root"]} {incr i} {
    # Get the label
    set label [getChildLabel "root" $i]

    puts "Move $label to the centre"
    moveToCentre $label

    #puts "Move the mesh to the center"
    #moveToCenter label

    puts "Set $label 's Hounsfield unit"
    setHU $label 1000
}

# Compute an X-ray image
puts "Compute an X-ray image"
disableArtefactFiltering
# Not working anymore enableArtefactFilteringOnGPU
# Not working anymore enableArtefactFilteringOnCPU
set x_ray_image [computeXRayImage]

# Save the last image into a file
puts "Save the last image into a file"
saveLastXRayImage
saveLastLBuffer

# Display the 3D scene  no event loop
displayScene

# Display the 3D scene  no event loop
# Run an interactive loop
#  can rotate the 3D scene and zoom-in
# Keys are:
# Q/Escape: to quit the event loop  does not close the window
# B: display/hide the X-ray beam
# W: display the polygon meshes in solid or wireframe
# N: display the X-ray image in negative or positive
# H: display/hide the X-ray detector
renderLoop
