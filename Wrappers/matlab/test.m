% Print the libraries' version
disp(gvxrMatlab.getVersionOfCoreGVXR());
disp(gvxrMatlab.getVersionOfSimpleGVXR());

% Create an OpenGL context
disp("Create an OpenGL context");
gvxrMatlab.createWindow();
gvxrMatlab.setWindowSize(512, 512);

% Set up the beam
disp("Set up the beam")
gvxrMatlab.setSourcePosition(-40.0,  0.0, 0.0, "cm");
gvxrMatlab.usePointSource();
%gvxrMatlab.useParallelBeam();
gvxrMatlab.setMonoChromatic(0.08, "MeV", 1000);

% Set up the detector
disp("Set up the detector");
gvxrMatlab.setDetectorPosition(10.0, 0.0, 0.0, "cm");
gvxrMatlab.setDetectorUpVector(0, 1, 0);
gvxrMatlab.setDetectorNumberOfPixels(640, 320);
gvxrMatlab.setDetectorPixelSize(0.5, 0.5, "mm");

% Load the data
disp("Load the data");
gvxrMatlab.loadSceneGraph("../data/welsh-dragon-small.stl", "mm");

for i = gvxrMatlab.getNumberOfChildren("root")
    label = gvxrMatlab.getChildLabel("root", i - 1);
    disp(strcat("Move ", label, " to the centre"));
    gvxrMatlab.moveToCentre(label);

    %disp("Move the mesh to the center");
    %gvxrMatlab.moveToCenter(label);

    disp(strcat("Set ", label, "'s Hounsfield unit"));
    gvxrMatlab.setHU(label, 1000)
endfor

% Compute an X-ray image
disp("Compute an X-ray image");
gvxrMatlab.disableArtefactFiltering();
% Not working anymore gvxrMatlab.enableArtefactFilteringOnGPU();
% Not working anymore gvxrMatlab.enableArtefactFilteringOnCPU();
temp_x_ray_image = gvxrMatlab.computeXRayImage();
x_ray_image = flipud(cell2mat(cell2mat(temp_x_ray_image'(1,:))));

% Save the last image into a file
disp("Save the last image into a file");
gvxrMatlab.saveLastXRayImage();
gvxrMatlab.saveLastLBuffer();

% Normalise the image between 0 and 1
np_normalised_image = ((x_ray_image-min(x_ray_image(:)))/max(x_ray_image(:)));

% Save the normalised image
imwrite(np_normalised_image, 'x_ray_image.png', "png");

% Show the image
imagesc(x_ray_image);
colorbar("location", "South");
colormap("gray");
set(gca,'dataAspectRatio',[1 1 1]);

% Display the 3D scene (no event loop)
gvxrMatlab.displayScene();


% Display the 3D scene (no event loop)
% Run an interactive loop
% (can rotate the 3D scene and zoom-in)
% Keys are:
% Q/Escape: to quit the event loop (does not close the window)
% B: display/hide the X-ray beam
% W: display the polygon meshes in solid or wireframe
% N: display the X-ray image in negative or positive
% H: display/hide the X-ray detector
gvxrMatlab.renderLoop();

exit;
