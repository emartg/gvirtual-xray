#!/usr/bin/env python3

import numpy as np
import xraylib
import sys

element_set = [
    #125kV,
    ["hydrogen", "H"],
    ["carbon", "C"],
    ["nitrogen", "N"],
    ["oxygen", "O"],
    ["sodium", "Na"],
    ["magnesium", "Mg"],
    ["phosphor", "P"],
    ["sulfur", "S"],
    ["clorine", "Cl"],
    ["argon", "Ar"],
    ["potassium", "K"],
    ["calcium", "Ca"],
    ["titanium", "Ti"],
    ["iron", "Fe"],
    ["copper", "Cu"],
    ["zinc", "Zn"],
    ["silver", "Ag"],
    ["tin", "Sn"],
    ["iodine", "I"],
]

compound_set = [
    "SiC",
    "H2O"
]

mixture_set = [
    ["Ti90Al6V4", "Ti0.9Al0.06V0.04"]
]

if __name__ == '__main__' :
    xraylib.XRayInit()

    print("xraylib version: {}".format(xraylib.__version__))

    energies = np.arange(10,80000,dtype=np.double)/100.0
    
    for element in element_set:
        Z = xraylib.SymbolToAtomicNumber(element[1])

        print("Process", element[0], element[1])

        with open(element[0] + ".tsv", 'w') as f:

            f.write("Photon Energy (in MeV)\tTot. w/ Coherent (in cm2 / g)\n")

            for E in energies:
                f.write("{:.5f}".format(E / 1000.0) + "\t" + "{:.8f}".format(xraylib.CS_Total(Z, E)) + "\n")

            f.close()

    for compound in compound_set:

        print("Process", compound)

        with open(compound + ".tsv", 'w') as f:

            f.write("Photon Energy (in MeV)\tTot. w/ Coherent (in cm2 / g)\n")

            for E in energies:
                f.write("{:.5f}".format(E / 1000.0) + "\t" + "{:.8f}".format(xraylib.CS_Total_CP(compound, E)) + "\n")

            f.close()

    for mixture in mixture_set:

        print("Process", mixture[0])

        # compound_data = xraylib.CompoundParser(mixture[1])
        # print(compound_data)
        with open(mixture[0] + ".tsv", 'w') as f:

            f.write("Photon Energy (in MeV)\tTot. w/ Coherent (in cm2 / g)\n")

            for E in energies:
                f.write("{:.5f}".format(E / 1000.0) + "\t" + "{:.8f}".format(xraylib.CS_Total_CP(mixture[1], E)) + "\n")

            f.close()

    # Z = np.arange(1,94,dtype=int)
    # CS = xrl_np.CS_Total_Kissel(Z,energies)


    sys.exit(0)
