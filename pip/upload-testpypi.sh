#!/usr/bin/sh

python3 -m pip install twine

python3 -m twine check wheelhouse/repaired/gvxr-*.*.*-*-*-*.manylinux*.whl
python3 -m twine check wheelhouse/gvxr-*.*.*-*-*-win_amd64.whl


python3 -m twine upload -r testpypi wheelhouse/repaired/gvxr-*.*.*-*-*-*.manylinux*.whl
python3 -m twine upload -r testpypi wheelhouse/gvxr-*.*.*-*-*-win_amd64.whl
