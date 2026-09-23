#!/bin/sh

doxygen doc/Doxyfile && doxy2swig/doxy2swig.py doc/xml/index.xml SimpleGVXR-doc.i
