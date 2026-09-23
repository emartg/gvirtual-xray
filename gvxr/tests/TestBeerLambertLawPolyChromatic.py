#!/usr/bin/env python3


import unittest
import gvxrPython3 as gvxr

#from PIL import Image

import matplotlib
import matplotlib.pyplot as plt
import matplotlib.image as mpimg

import numpy as np


backend = matplotlib.get_backend()
if backend is 'agg':
    print ("Change Matplotlib backend from", backend, "to", "TkAgg");
    plt.switch_backend('TkAgg');
    backend = matplotlib.get_backend()

# Create an OpenGL context
print("Create an OpenGL context")
gvxr.createWindow();
gvxr.setWindowSize(1280, 800);


# Set up the beam
print("Set up the beam")
gvxr.setSourcePosition(0.0,  0.0, -10.0, "cm");
#gvxr.usePointSource();
gvxr.useParallelBeam();
#gvxr.setMonoChromatic(0.08, "MeV", 1000);

# Set up the detector
print("Set up the detector");
gvxr.setDetectorPosition(0.0, 0.0, 10.0, "cm");
gvxr.setDetectorUpVector(0, 1, 0);
gvxr.setDetectorNumberOfPixels(600, 600);
gvxr.setDetectorPixelSize(0.15, 0.15, "mm");

# Load the data
print("Load the data");

gvxr.makeCube("outside",
        3.0,
        "cm",
        "root");

gvxr.makeCylinder("inside",
		20,
        3.0,
        1.0,
		"cm",
        "root");

gvxr.setElement("outside", "C")
gvxr.setElement( "inside", "O")

gvxr.moveToCenter();

gvxr.addPolygonMeshAsInnerSurface("inside");
gvxr.addPolygonMeshAsOuterSurface("outside");

# Compute an X-ray image
print("Compute an X-ray image");
# Not working anymore gvxr.enableArtefactFilteringOnGPU();
gvxr.enableArtefactFilteringOnCPU();

# Set the beam spectrum
gvxr.loadSpectrumFromTSV("data/55kV.tsv", "keV", True);
x_55kV = gvxr.getEnergyBins("keV");
y_55kV = np.divide(gvxr.getPhotonCountEnergyBins(), 1000);

# Compute the X-ray image
x_ray_image = gvxr.computeXRayImage();

# Retrieve the image in Numpy's 2D array format
np_image_55_kV = np.array(x_ray_image);

# Save the last image into a file
gvxr.saveLastXRayImage("55_kV.mha");


# Set the beam spectrum
gvxr.loadSpectrumFromTSV("data/125kV.tsv", "keV", True);
x_125kV = gvxr.getEnergyBins("keV");
y_125kV = np.divide(gvxr.getPhotonCountEnergyBins(), 1000);

# Compute the X-ray image
x_ray_image = gvxr.computeXRayImage();

# Retrieve the image in Numpy's 2D array format
np_image_125_kV = np.array(x_ray_image);

# Save the last image into a file
gvxr.saveLastXRayImage("125_kV.mha");



# Save the last image into a file
print("Save the last image into a file");
#gvxr.saveLastXRayImage();
#gvxr.saveLastLBuffer();


# Normalise the image between 0 and 255 (this is for PIL)
#np_normalised_image = (255 * (np_image-np_image.min())/np_image.max()).astype(np.int8);

# Convert the Numpy array into a PIL image
#img = Image.fromarray(np_normalised_image.astype(np.ubyte));

# Save the PIL image
#img.save('my.png')

# Show the image
#img.show()


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


# Get profile
num = 600;
x0 = 0;
x1 = np_image_55_kV.shape[1] - 1;
y0 = np_image_55_kV.shape[0] / 2;
y1 = np_image_55_kV.shape[0] / 2;

x, y = np.linspace(x0, x1, num), np.linspace(y0, y1, num)

normalised_np_image_55_kV  = np.subtract( np_image_55_kV,  np_image_55_kV.mean());
normalised_np_image_125_kV = np.subtract(np_image_125_kV, np_image_125_kV.mean());

normalised_np_image_55_kV  = np.divide( normalised_np_image_55_kV,  normalised_np_image_55_kV.std())
normalised_np_image_125_kV = np.divide(normalised_np_image_125_kV, normalised_np_image_125_kV.std())

profile_55kV  =  normalised_np_image_55_kV[y.astype(np.int), x.astype(np.int)];
profile_125kV = normalised_np_image_125_kV[y.astype(np.int), x.astype(np.int)];

profile_55kV  =  np_image_55_kV[y.astype(np.int), x.astype(np.int)];
profile_125kV = np_image_125_kV[y.astype(np.int), x.astype(np.int)];

plt.figure()
plt.subplot(3, 1, 1)
plt.imshow((normalised_np_image_55_kV), cmap="gray")
plt.plot([x0, x1], [y0, y1], 'ro-')
plt.axis('image')
plt.title('55 kV')

plt.subplot(3, 1, 2)
plt.imshow((normalised_np_image_125_kV), cmap="gray")
plt.plot([x0, x1], [y0, y1], 'ro-')
plt.axis('image')
plt.title('125 kV')

plt.subplot(3, 1, 3)
plt.plot(np.divide( profile_55kV,  profile_55kV.max()), label="55 kV")
plt.plot(np.divide(profile_125kV, profile_125kV.max()), label="125 kV")
plt.title('Intensity profile')
plt.ylabel('Distance along profile')
plt.ylabel('Normalised intensity')
plt.legend()


plt.figure();


plt.plot( x_55kV,  y_55kV, label="55 kV");
plt.plot(x_125kV, y_125kV, label="125 kV");
plt.title('Spectrum of the incident X-ray beam');
plt.xlabel('Energy (keV)');
plt.ylabel('Normalised intensity');
plt.legend()








plt.show()


# Display the 3D scene (no event loop)
gvxr.displayScene();










'''


class TestStringMethods(unittest.TestCase):

    def test_upper(self):
        self.assertEqual('foo'.upper(), 'FOO')

    def test_isupper(self):
        self.assertTrue('FOO'.isupper())
        self.assertFalse('Foo'.isupper())

    def test_split(self):
        s = 'hello world'
        self.assertEqual(s.split(), ['hello', 'world'])
        # check that s.split fails when the separator is not a string
        with self.assertRaises(TypeError):
            s.split(2)

if __name__ == '__main__':
    unittest.main()

'''
