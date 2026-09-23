from __future__ import annotations

from pathlib import Path

import json5

from . import DigitalTwin

_twin_list: list[dict] = []


def getDigitalTwinList():
    """Accessor on the list of digital twins currently supported by gVXR.

    If you'd like your device to be listed, please contact the project maintainer.

    Raises
    ------
    IOError
        If two twins share the same name.

    Returns
    -------
    list
        a list of dictionaries

    """
    global _twin_list
    if not _twin_list:
        directory_path = Path(__file__).parent

        _twin_list = []

        for fname in sorted((directory_path / "models").rglob("*.json5")):
            file_path = Path(fname)

            with file_path.open("r") as f:
                _twin_list.append(json5.load(f))

        # Make sure the name is new
        counter = 0
        for twin in _twin_list:
            if _twin_list[-1]["name"].upper() == twin["name"].upper():
                counter += 1

        # Two twins have the same name
        if counter != 1:
            raise OSError("Two twins have the same name (" + twin["name"].upper() + ")")

    return _twin_list


def createDigitalTwin(*, id: int | None = None, name: str | None = None) -> DigitalTwin:
    """Load a given digital twin and initialise gVXR.

    Parameters
    ----------
    id : int
        The index of the digital twin, as specified in the list provided by getDigitalTwinList().

    name : str
        The name of the digital twin, as specified in the list provided by getDigitalTwinList().
        Note that the case will be ignored.

    Raises
    ------
    ValueError
        If neither id and name are used.

    ValueError
        If both id and name are used.

    ValueError
        If the requested twin does not exist.

    Returns
    -------
    DigitalTwin
        a selected digital twin

    """
    # Test the validity of parameters
    if id is None and name is None:
        raise ValueError("Use either id or name to choose your digital twin")

    if id is not None and name is not None:
        raise ValueError("Use either id or name to choose your digital twin, not both")

    # Make sure the list of twin exists
    getDigitalTwinList()
    # And ID is used
    if id is not None:
        if id in range(len(_twin_list)):
            return DigitalTwin.from_json(_twin_list[id])
        raise ValueError(
            "The twin that you selected does not exist. Use `getDigitalTwinList()` to identify a suitable twin."
        )
    if name is not None:
        # A name is used
        for twin in _twin_list:
            if name.upper() == twin["name"].upper():
                return DigitalTwin.from_json(twin)

    # The twin was not found
    raise ValueError(
        "The twin that you selected does not exist. Use `getDigitalTwinList()` to identify a suitable twin."
    )

def getAvailableFilters(twin, beam_type) -> list[list[str | float]]:
    """
    Accessor on the available filters for a specific beam type of a given twin.

    Parameters
    ----------
    twin : DigitalTwin
        Digital twin to use
        
    beam_type : str
        Beam type
    
    Returns
    -------
    list[list[str | float]]
        Available filters
    """

    return twin.specification.beams[beam_type].filters

def getFilterMaterial(twin, beam_type) -> list[str]:
    """
    Create an array of strings that correspond to the filter materials supported by
    a specific beam type of a given twin.

    Parameters
    ----------
    twin : DigitalTwin
        Digital twin to use

    beam_type : str
        Beam type

    Returns
    -------
    list[str]
        Materials of the supported filters
    """

    filter_materials = ["None"]

    for item in getAvailableFilters(twin, beam_type):
        filter_materials.append(item[0])

    return filter_materials

def getFilterThickness(twin, beam_type, material) -> list[str]:
    """
    Create an array of strings that correspond to the thickness in mm of a filter supported by
    a specific beam type of a given twin.

    Parameters
    ----------
    twin : DigitalTwin
        Digital twin to use

    beam_type : str
        Beam type

    material: str
        Material of the filter

    Returns
    -------
    list[float]
        Thickness of the corresponding filter
    """

    thicknesses = [str(0.0)]

    filters = twin.specification.beams[beam_type].filters

    for item in filters:

        if item[0] == material:
            for thickness in item[1:]:
                thicknesses.append(str(thickness))

    return thicknesses
