import os
import platform
import json # Save the JSON file
from gvxrPython3 import gvxr # Simulate X-ray images

def getRelativePath(JSON_fname: str, target_fname: str):

    # Not using windows, use a relative path
    if platform.system() != "Windows":
        return os.path.relpath(target_fname, os.path.dirname(os.path.abspath(JSON_fname)))
    # Using windows, both files are on the same drive, use a relative path
    elif os.path.abspath(target_fname)[1] == ':' and os.path.abspath(JSON_fname)[1] == ':' and os.path.abspath(target_fname)[0] == os.path.abspath(JSON_fname)[0]:
        return os.path.relpath(target_fname, os.path.dirname(os.path.abspath(JSON_fname)))
    # Using windows, the files are not on the same drive, use an absolute path
    else:
        return os.path.abspath(target_fname);


    
def saveJSON(fname: str):
    params = {}
    
    params["File format version"] = [1, 0, 0]
    params["Window size"] = gvxr.getWindowSize()
    
    params["Source"] = {}
    
    position = gvxr.getSourcePosition("mm")
    params["Source"]["Position"] = [position[0], position[1], position[2], "mm"]
    
    params["Source"]["Shape"] = gvxr.getSourceShape();
    if gvxr.getSourceShape() == "FOCALSPOT":
        raise IOError('Exporter for gvxr.getSourceShape() == "FOCALSPOT" is not implemented')
    
    mAs = gvxr.getmAs()
    kVp = gvxr.getVoltage("kV")

    print("mAs", mAs, "kVp", kVp)
    if kVp > 0.0:
        params["Source"]["Beam"] = {}
        params["Source"]["Beam"]["Peak kilo voltage"] = kVp

        tube_angle = gvxr.getTubeAngle()
        params["Source"]["Beam"]["Tube angle"] = tube_angle
        
        if mAs > 0.0:
            params["Source"]["Beam"]["mAs"] = mAs
        
        filtration_material = gvxr.getFiltrationMaterial()
        filtration_thickness = gvxr.getFiltrationThickness("mm")

        if len(filtration_material) > 0:
            params["Source"]["Beam"]["filter"] = []
            
            for Z, thickness in zip(filtration_material, filtration_thickness):
                params["Source"]["Beam"]["filter"].append([Z, thickness, "mm"])
    else:
        params["Source"]["Beam"] = []
        energy_bins = gvxr.getEnergyBins("keV");
        photon_counts = gvxr.getPhotonCountEnergyBins();

        for energy, count in zip(energy_bins, photon_counts):
            params["Source"]["Beam"].append({
                "Energy": energy,
                "PhotonCount": count,
                "Unit": "keV"
            })
    
    params["Detector"] = {}
    
    position = gvxr.getDetectorPosition("mm")
    params["Detector"]["Position"] = [position[0], position[1], position[2], "mm"]
    
    v = gvxr.getDetectorUpVector()
    params["Detector"]["UpVector"] = [v[0], v[1], v[2]]

    u = gvxr.getDetectorRightVector()
    params["Detector"]["RightVector"] = [u[0], u[1], u[2]]

    cols, rows = gvxr.getDetectorNumberOfPixels()
    params["Detector"]["NumberOfPixels"] = [cols, rows]
    
    cols, rows = gvxr.getDetectorSize("mm")
    params["Detector"]["Size"] = [cols, rows, "mm"]

    oversampling_factor = gvxr.getOversamplingFactor()
    if oversampling_factor > 1:
        params["Detector"]["Oversampling"] = oversampling_factor

    response = gvxr.getLSF();
    if len(response) > 0:
        params["Detector"]["LSF"] = response
    
    # Save the scintillator if any was used
    scintillator_material = gvxr.getScintillatorMaterial();
    scintillator_thickness = gvxr.getScintillatorThickness("um");

    if len(scintillator_material) > 0 and scintillator_thickness > 0.01:
        params["Detector"]["Scintillator"] = {}
        params["Detector"]["Scintillator"]["Material"] = scintillator_material
        params["Detector"]["Scintillator"]["Thickness"] = scintillator_thickness
        params["Detector"]["Scintillator"]["Unit"] = "um"

    # Save the energy response if any was used
    else:
        response = gvxr.getEnergyResponse("keV");
        if len(response) > 0:
            params["Detector"]["Energy response"] = {}
            params["Detector"]["Energy response"]["Unit"] = "keV"
            params["Detector"]["Energy response"]["LUT"] = response
    
    # Save the CT scan properties
    if gvxr.getNumberOfProjectionsCT() > 1:
        params["Scan"] = {};

        if len(gvxr.getProjectionOutputPathCT()) > 0:
            params["Scan"]["OutFolder"] = getRelativePath(fname, gvxr.getProjectionOutputPathCT());

        if len(gvxr.getScreenshotOutputPathCT()) > 0:
            params["Scan"]["GifPath"] = getRelativePath(fname, gvxr.getScreenshotOutputPathCT());

        params["Scan"]["NumberOfProjections"] = gvxr.getNumberOfProjectionsCT();

        params["Scan"]["AngleStep"] = gvxr.getAngleSetCT()[1] - gvxr.getAngleSetCT()[0];
        params["Scan"]["StartAngle"] = gvxr.getFirstAngleCT();
        params["Scan"]["FinalAngle"] = gvxr.getLastAngleCT();
        params["Scan"]["IncludeLastAngle"] = gvxr.getIncludeLastAngleFlagCT();

        if gvxr.getWhiteImagesInFlatFieldCT() == 0:
            params["Scan"]["Flat-Field Correction"] = False;
        else:
            params["Scan"]["Flat-Field Correction"] = True;
            params["Scan"]["NumberOfWhiteImages"] = gvxr.getWhiteImagesInFlatFieldCT();

        params["Scan"]["CentreOfRotation"] = [
            gvxr.getCentreOfRotationPositionCT("mm")[0],
            gvxr.getCentreOfRotationPositionCT("mm")[1],
            gvxr.getCentreOfRotationPositionCT("mm")[2],
            "mm"];

        params["Scan"]["RotationAxis"] = gvxr.getRotationAxisCT()


    params["Samples"] = []
    
    for i in range(gvxr.getNumberOfChildren("root")):
        label = gvxr.getChildLabel("root", i);
        
        params["Samples"].append({})
        params["Samples"][-1]["Label"] = label
        params["Samples"][-1]["Path"] = getRelativePath(fname, gvxr.getMeshFilename(label));
        params["Samples"][-1]["Unit"] = gvxr.getMeshUnitOfLength(label)
                
        material_label = gvxr.getMaterialLabel(label)

        if "Element" in material_label or \
            "Compound" in material_label or \
            "Mixture" in material_label or \
            "HU" in material_label or \
            "mu" in material_label:

            material_type = material_label.split(": ")[0]
            material_properties = material_label.split(": ")[1].split(" ")
        
            params["Samples"][-1]["Material"] = [material_type]

            for material_property in material_properties:
                params["Samples"][-1]["Material"].append(material_property)

            if "HU" not in material_label:
                params["Samples"][-1]["Density"] = gvxr.getDensity(label)
        else:
            raise IOError('Exporter for Material ' + material_label + ' is not implemented')

        # Ignore the identity matrix
        matrix = gvxr.getLocalTransformationMatrix(label)
        if matrix != gvxr.getIdentityMatrix4():
            params["Samples"][-1]["Transform"] = [["Matrix", matrix]]

        # Outer or inner surface?
        if gvxr.isOuterSurface(label):
            params["Samples"][-1]["Type"] = "outer";
        else:
            params["Samples"][-1]["Type"] = "inner";
            
        # Flip normal if needed
        if gvxr.hasNormalVectorsInverted(label):
            params["Samples"][-1]["FlipNormals"] = True;

        # Colours
        params["Samples"][-1]["AmbientColour"] = gvxr.getAmbientColour(label);
        params["Samples"][-1]["DiffuseColour"] = gvxr.getDiffuseColour(label);
        params["Samples"][-1]["SpecularColour"] = gvxr.getSpecularColour(label);
        params["Samples"][-1]["Shininess"] = gvxr.getShininess(label);

    # Convert and write JSON object to file
    with open(fname, "w") as outfile: 
        json.dump(params, outfile, indent=4)
