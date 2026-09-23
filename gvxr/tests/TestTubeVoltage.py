#!/usr/bin/env python3


import numpy as np
import matplotlib.pyplot as plt

from gvxrPython3.utils import getXpecgenSpectrum
from gvxrPython3.utils import getSpekpySpectrum





voltage = 100
filtration = [["Al", 10], ["Cu", 5]]

k_xpecgen_unfiltered, f_xpecgen_unfiltered, units = getXpecgenSpectrum(voltage)
k_spekpy_unfiltered,  f_spekpy_unfiltered,  units = getSpekpySpectrum(voltage)

k_xpecgen_filtered, f_xpecgen_filtered, units = getXpecgenSpectrum(voltage, filtration)
k_spekpy_filtered,  f_spekpy_filtered,  units = getSpekpySpectrum(voltage, filtration)

xpecgen_unfiltered = np.column_stack((k_xpecgen_unfiltered, f_xpecgen_unfiltered))
spekpy_unfiltered = np.column_stack((k_spekpy_unfiltered,  f_spekpy_unfiltered))
xpecgen_filtered = np.column_stack((k_xpecgen_filtered, f_xpecgen_filtered))
spekpy_filtered = np.column_stack((k_spekpy_filtered,  f_spekpy_filtered))

np.savetxt("data/xpecgen_unfiltered.dat", xpecgen_unfiltered, comments='')
np.savetxt("data/spekpy_unfiltered.dat", spekpy_unfiltered, comments='')
np.savetxt("data/xpecgen_filtered.dat", xpecgen_filtered, comments='')
np.savetxt("data/spekpy_filtered.dat", spekpy_filtered, comments='')

print("Xpecgen, unfiltered, number of bins:", len(k_xpecgen_unfiltered))
print("Spekpy, unfiltered, number of bins:", len(k_spekpy_unfiltered))
print("Xpecgen, filtered, number of bins:", len(k_xpecgen_filtered))
print("Spekpy, filtered, number of bins:", len(k_spekpy_filtered))

plt.figure()
plt.plot(k_xpecgen_unfiltered, f_xpecgen_unfiltered, label="Xpecgen, unfiltered")
plt.plot(k_spekpy_unfiltered,  f_spekpy_unfiltered, '.', markersize=0.1, label="Spekpy, unfiltered")
plt.plot(k_xpecgen_filtered, f_xpecgen_filtered, label="Xpecgen, filtered")
plt.plot(k_spekpy_filtered, f_spekpy_filtered, '.', markersize=0.1, label="Spekpy, filtered")
plt.xlabel("Energy [" + units + "]")
plt.ylabel("Photon count")
plt.legend()
plt.title("Corresponding spectra")
plt.savefig("data/TestTubeSpectrum.pdf")
plt.show()