#!/usr/bin/env python3

# These two packages are used to know if we are running the program from a MacOSX bundle
import sys, os

# These two packages are used for debugging purposes (catching exceptions)
import traceback, logging

# wxPython is used to create the GUI
import wx
from wx import glcanvas

# numpy is used to save images in ASCII
import numpy as np

# numpy is used to save images as image files
from PIL import Image

# Matplotlib is used to display the 2D images an the histogram
import matplotlib

# Matplotlib relies on the wxPython backend
matplotlib.use('WXAgg')

import OpenGLCanvas as glc;
import slider_code as SC;

# gVirtualXRay's wrapper
from gvxrPython3 import gvxr;
from wxCanvasPanel import wxCanvasPanel;

if getattr(sys, 'frozen', False): # we are running in a bundle
    dir_path = sys._MEIPASS # This is where the files are unpacked to
else: # normal Python environment
    dir_path = os.path.dirname(os.path.abspath(__file__))


class Interface(wx.Frame):
    def __init__(self, parent, slider_container, plot_panel, title):
        super(Interface, self).__init__(parent, title=title, size = wx.Size( 661,480 ), style = wx.DEFAULT_FRAME_STYLE|wx.TAB_TRAVERSAL )


        # Show the window on screen
        self.canvas = glc.OpenGlCanvas(self, slider_container, plot_panel);
        self.InitUI();
        print("Show window")
        self.Show();
        size = self.canvas.GetGLExtents()
        self.canvas.OnReshape(size.width, size.height)
        self.canvas.Refresh(True)
        self.Refresh(True)
        self.canvas.Update()
        self.Update()
        self.Bind(wx.EVT_CLOSE,     self.processCanvasCloseEvent)

    def InitUI(self):
        menubar = wx.MenuBar()

        fileMenu = wx.Menu()

        save_item = wx.MenuItem(fileMenu, wx.ID_SAVE, text = "&Save as...\tCtrl+S",kind = wx.ITEM_NORMAL)
        save_item.SetBitmap(wx.Bitmap(dir_path + "/saveAs.png"))
        fileMenu.Append(save_item)


        # NEW,
        # ADD MESH


        '''newitem = wx.MenuItem(fileMenu,wx.ID_NEW, text = "New",kind = wx.ITEM_NORMAL)
        #newitem.SetBitmap(wx.Bitmap("new.bmp"))
        fileMenu.AppendItem(newitem)

        fileMenu.AppendSeparator()
        '''

        #editMenu = wx.Menu()
        #copyItem = wx.MenuItem(editMenu, 100,text = "copy",kind = wx.ITEM_NORMAL)
        #copyItem.SetBitmap(wx.Bitmap("copy.bmp"))
        #editMenu.AppendItem(copyItem)

        #cutItem = wx.MenuItem(editMenu, 101,text = "cut",kind = wx.ITEM_NORMAL)
        #cutItem.SetBitmap(wx.Bitmap("cut.bmp"))
        #editMenu.AppendItem(cutItem)

        #pasteItem = wx.MenuItem(editMenu, 102,text = "paste",kind = wx.ITEM_NORMAL)
        #pasteItem.SetBitmap(wx.Bitmap("paste.bmp"))
        #editMenu.AppendItem(pasteItem)

        #fileMenu.AppendMenu(wx.ID_ANY, "Edit", editMenu)
        #fileMenu.AppendSeparator()

        scaleMenu = wx.Menu()
        scaleMenu.AppendRadioItem(100, "um")
        scaleMenu.AppendRadioItem(200, "mm")
        scaleMenu.AppendRadioItem(300, "cm")
        scaleMenu.AppendRadioItem(400, "dm")
        scaleMenu.AppendRadioItem(500, "m")

        scaleMenu.Check(100, False);
        scaleMenu.Check(200, False);
        scaleMenu.Check(300, False);
        scaleMenu.Check(400, False);
        scaleMenu.Check(500, False);

        if self.canvas.getObjectScale() == "um":
            scaleMenu.Check(100, True);
        elif self.canvas.getObjectScale() == "mm":
            scaleMenu.Check(200, True);
        elif self.canvas.getObjectScale() == "cm":
            scaleMenu.Check(300, True);
        elif self.canvas.getObjectScale() == "dm":
            scaleMenu.Check(400, True);
        elif self.canvas.getObjectScale() == "m":
            scaleMenu.Check(500, True);


        #editMenu.AppendSeparator()

        #fileMenu.AppendCheckItem(103,"Checkable")
        #editMenu.AppendMenu(wx.ID_ANY, "Scale", scaleMenu)

        menubar.Append(fileMenu, '&File')
        menubar.Append(scaleMenu, 'Scale')

        quit_item = wx.MenuItem(fileMenu, wx.ID_EXIT, text = "&Quit\tCtrl+Q", kind = wx.ITEM_NORMAL)
        quit_item.SetBitmap(wx.Bitmap(dir_path + "/quit.png"))
        fileMenu.Append(quit_item)



        self.SetMenuBar(menubar)
        #self.text = wx.TextCtrl(self,-1, style = wx.EXPAND|wx.TE_MULTILINE)
        self.Bind(wx.EVT_MENU, self.menuhandler)

    def menuhandler(self, event):
        id = event.GetId()
        if id == wx.ID_SAVE:
            print("SAVE");
            with wx.FileDialog(self, "Save image file", wildcard = "BMP and GIF files (*.bmp;*.gif)|*.bmp;*.gif|PNG files (*.png)|*.png|JPEG files (*.jpg;*.jpeg)|*.jpg;*.jpeg|TIFF files (*.tif;*.tiff)|*.tif;*.tiff|MHA files (*.mha)|*.mha|Text files (*.txt)|*.txt", style=wx.FD_SAVE | wx.FD_OVERWRITE_PROMPT) as fileDialog:

                if fileDialog.ShowModal() == wx.ID_CANCEL:
                    return     # the user changed their mind

                # save the current contents in the file
                pathname = fileDialog.GetPath()
                try:
                    with open(pathname, 'w') as file:
                        #self.doSaveData(file)
                        file.close();
                        x_ray_image = gvxr.computeXRayImage();
                        file_extension = pathname[-4:].upper();

                        if file_extension == ".TXT" or file_extension == ".MHA":
                            gvxr.saveLastXRayImage(pathname);
                        else:
                            np_image = np.array(x_ray_image);
                            np_normalised_image = (255 * (np_image-np_image.min()) / (np_image.max()-np_image.min())).astype(np.int8);
                            img = Image.fromarray(np_normalised_image.astype(np.ubyte));
                            img.save(pathname);
                except IOError:
                    wx.LogError("Cannot save current data in file '%s'." % pathname)

        elif id == wx.ID_EXIT:
            exit();
        elif id == 100:
            print ("Change scale to um");
            self.setObjectScale("um");
        elif id == 200:
            print ("Change scale to mm");
            self.setObjectScale("mm");
        elif id == 300:
            print ("Change scale to cm");
            self.setObjectScale("cm");
        elif id == 400:
            print ("Change scale to dm");
            self.setObjectScale("dm");
        elif id == 500:
            print ("Change scale to m");
            self.setObjectScale("m");
        # Unknown ID
        else:
            print ("Event on widget ID (", id, "), not handled.")

    # Close the application
    def processCanvasCloseEvent(self, event):
        event.Skip()
        exit();

    def setObjectScale(self, aScale):
        self.canvas.setObjectScale(aScale);

if __name__ == '__main__':
    try:

        # Print the libraries' version
        print (gvxr.getVersionOfSimpleGVXR())
        print (gvxr.getVersionOfCoreGVXR())

        # Create the application object
        app = wx.App()

        slider_container = SC.slider_code(None, "Material properties");

        plot_frame = wx.Frame(None, title='X-ray projection')
        plot_panel = wxCanvasPanel(plot_frame, slider_container)

        opengl_interface = Interface(None, slider_container=slider_container, plot_panel=plot_panel, title='My Test application');

        plot_panel.data_ready = True;
        plot_panel.draw()
        plot_frame.Show()

        app.MainLoop()
    except Exception as e:
        logging.error(traceback.format_exc())

# eaxmple fo bottom right :
# glViewport(WindowWidth*0.5, WindowWidth*0.5, WindowHeight*0.5);
# draw this

# problems displaying canvas after alterations
