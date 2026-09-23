has_spekpy = False
has_xpecgen = False

try:
    import spekpy as sp
    has_spekpy = True
except:
    has_spekpy = False

try:
    from xpecgen import xpecgen as xg
    has_xpecgen = True
except:
    has_xpecgen = False


def hasSpekpy():
    global has_spekpy
    return has_spekpy

def hasXpecgen():
    global has_xpecgen
    return has_xpecgen


def getSpectrumSpekpy(args, dummy=0):
    import numpy as np
    import spekpy as sp

    global has_spekpy

    k_list = []
    f_list = []

    kvp_in_kV = args[0]
    filter_material_set = args[1]
    filter_thickness_set = args[2]
    th_in_deg = args[3]

    # Spekpy is installed
    if has_spekpy:

        if len(filter_material_set) != len(filter_thickness_set):
            raise IOError("The filtration has an issue")

        # Generate a spectrum
        s = sp.Spek(kvp=kvp_in_kV, th=th_in_deg)

        #Inherent filtration: 1.2mm Al + 100cm Air
        s.filter("Al", 1.2)
        # s.filter("air", 1000)

        # Additional filters
        for filter_material, filter_thickness_in_mm in zip(filter_material_set, filter_thickness_set):
            if filter_material is not None and filter_thickness_in_mm is not None:
                s.filter(filter_material, filter_thickness_in_mm)

        # Get the spectrum
        k, f = s.get_spectrum(edges=True)

        # Normalise the area under the curve
        area = 0.0
        prev_bin = None
        for energy, count in zip(k, f):
            if prev_bin is not None:
                area += count * (energy - prev_bin)
            prev_bin = energy

        f = np.array(f) / area
        k_list = k.tolist()
        f_list = f.tolist()

    return k_list, f_list


# def getSpekpySpectrum(kvp_in_kV, filters=None, th_in_deg=12):
#     return getSpectrumSpekpy(kvp_in_kV, filters, th_in_deg)


def getSpectrumXpecgen(args, dummy=0):

    import numpy as np
    from xpecgen import xpecgen as xg

    global has_xpecgen

    k_list = []
    f_list = []

    kvp_in_kV = args[0]
    filter_material_set = args[1]
    filter_thickness_set = args[2]
    th_in_deg = args[3]

    # xpecgen is installed
    if has_xpecgen:

        if len(filter_material_set) != len(filter_thickness_set):
            raise IOError("The filtration has an issue")

        # Generate a spectrum
        xrs = xg.calculate_spectrum(kvp_in_kV, th_in_deg, 1, 100, epsrel=0.5, monitor=None, z=74)

        #Inherent filtration: 1.2mm Al + 100cm Air
        mu_Al = xg.get_mu(13)
        xrs.attenuate(0.12,mu_Al)
        # xrs.attenuate(100,xg.get_mu("air"))

        # Additional filters
        for filter_material, filter_thickness_in_mm in zip(filter_material_set, filter_thickness_set):
            if filter_material is not None and filter_thickness_in_mm is not None:
                if type(filter_material) == str:
                    mu = xg.get_mu(gvxr.getElementAtomicNumber(filter_material))
                else:
                    mu = xg.get_mu(filter_material)

                xrs.attenuate(filter_thickness_in_mm / 10, mu)

        # Get the spectrum
        (k_list,f) = xrs.get_points()

        # Normalise the area under the curve
        area = 0.0
        prev_bin = None
        for energy, count in zip(k_list, f):
            if prev_bin is not None:
                area += count * (energy - prev_bin)
            prev_bin = energy

        f = np.array(f) / area
        # k_list = k.tolist()
        f_list = f.tolist()

    return k_list, f_list


# def getXpecgenSpectrum(kvp_in_kV, filters=None, th_in_deg=12):
#     return getSpectrumXpecgen(kvp_in_kV, filters, th_in_deg)

def getInterpretor():
    import sys
    return sys.executable
