#!/usr/bin/env python3

# These two packages are used to know if we are running the program from a MacOSX bundle
import sys, os

# These two packages are used for debugging purposes (catching exceptions)
import traceback, logging

# Matplotlib is used to display the 2D images an the histogram
import matplotlib

# wxPython is used to create the GUI
import wx

# gVirtualXRay's wrapper
from gvxrPython3 import gvxr;

# The main GUI application
import wxApp;

# import slider_code as SC;
from wxCanvasPanel import wxCanvasPanel;




# Initialise the environment
def init():
    # Matplotlib relies on the wxPython backend
    matplotlib.use('WXAgg')

    if getattr(sys, 'frozen', False): # we are running in a bundle
        wxApp.g_dir_path = sys._MEIPASS # This is where the files are unpacked to
    else: # normal Python environment
        wxApp.g_dir_path = os.path.dirname(os.path.abspath(__file__))

    # Print the libraries' version
    print (gvxr.getVersionOfSimpleGVXR())
    print (gvxr.getVersionOfCoreGVXR())


# The main function
if __name__ == '__main__':
    try:

        # Initialise the environment
        init();

        # Create the application object
        app = wx.App();

        gvxrApp = wxApp.wxApp(parent = None, title = "gvxrGUI-0.0.1");

        # Launch the event loop
        app.MainLoop()

    # Catch exceptions if any
    except Exception as e:

        # Log an error message
        logging.error(traceback.format_exc())
