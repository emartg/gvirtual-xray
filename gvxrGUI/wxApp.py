# numpy is used to save images in ASCII
import numpy as np

# numpy is used to save images as image files
from PIL import Image

# wxPython is used to create the GUI
import wx
from wx import glcanvas

# gVirtualXRay's wrapper
from gvxrPython3 import gvxr;

from OpenGLCanvas import OpenGlCanvas;
#from ParamBox import ParamBox;
from wxCanvasPanel import wxCanvasPanel;
import MeasureCanvas;
import HistogramCanvas;
import GlobalParameters as GP;


################################################################################
# Global variables
################################################################################
g_dir_path = "";
g_wxapp = None;

# The main window
class wxApp(wx.Frame):
    def __init__(self,
                 parent = None, #slider_container, plot_panel,
                 title = "gvxrGUI"):

        global g_wxapp;

        # Call the constructor of wx.Frame
        super(wxApp, self).__init__(parent, title=title, size = wx.Size( 661,480 ), style = wx.DEFAULT_FRAME_STYLE|wx.TAB_TRAVERSAL )

        g_wxapp = self;

        self.current_energy_unit = "keV";

        #self.param_box = ParamBox(None)

        # Add the OpenGL viewer
        self.canvas = OpenGlCanvas(self);#, slider_container, plot_panel);



        # Add menubar
        self.addMenuBar();

        # Show the window on screen
        self.Show();

        self.image_viewer = wxCanvasPanel(None,  "MeV");


        '''self.InitUI();
        self.Update();'''
        self.Bind(wx.EVT_CLOSE,     self.processCanvasCloseEvent);
        self.redisplay();
        #self.param_box.Show(True)

    def addMenuBar(self):
        menu_bar = wx.MenuBar()

        file_menu = wx.Menu()

        save_item = wx.MenuItem(file_menu, wx.ID_SAVE, text = "&Save as...\tCtrl+S",kind = wx.ITEM_NORMAL)
        save_item.SetBitmap(wx.Bitmap(g_dir_path + "/saveAs.png"))
        file_menu.Append(save_item)


        '''newitem = wx.MenuItem(file_menu,wx.ID_NEW, text = "New",kind = wx.ITEM_NORMAL)
        #newitem.SetBitmap(wx.Bitmap("new.bmp"))
        file_menu.AppendItem(newitem)

        file_menu.AppendSeparator()
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

        #file_menu.AppendMenu(wx.ID_ANY, "Edit", editMenu)
        #file_menu.AppendSeparator()

        self.scale_menu = wx.Menu()
        self.scale_menu.AppendRadioItem(100, "um")
        self.scale_menu.AppendRadioItem(101, "mm")
        self.scale_menu.AppendRadioItem(102, "cm")
        self.scale_menu.AppendRadioItem(103, "dm")
        self.scale_menu.AppendRadioItem(104, "m")

        self.scale_menu.Check(100, False);
        self.scale_menu.Check(101, False);
        self.scale_menu.Check(102, False);
        self.scale_menu.Check(103, False);
        self.scale_menu.Check(104, False);

        self.setObjectScale(GP.mesh_unit_of_length);

        self.unit_of_energy = wx.Menu()
        self.unit_of_energy.AppendRadioItem(200, "eV")
        self.unit_of_energy.AppendRadioItem(201, "keV")
        self.unit_of_energy.AppendRadioItem(202, "MeV")

        self.unit_of_energy.Check(200, False);
        self.unit_of_energy.Check(201, False);
        self.unit_of_energy.Check(202, False);

        self.setImageUnitOfEnergy(self.current_energy_unit);

        #editMenu.AppendSeparator()

        #file_menu.AppendCheckItem(103,"Checkable")
        #editMenu.AppendMenu(wx.ID_ANY, "Scale", scale_menu)

        analyse_menu = wx.Menu()
        measure_menu = wx.MenuItem(analyse_menu, 601, text = "&Measure\tCtrl+M", kind = wx.ITEM_NORMAL)
        histogram_menu = wx.MenuItem(analyse_menu, 602, text = "&Histogram\tCtrl+H", kind = wx.ITEM_NORMAL)
        analyse_menu.Append(measure_menu)
        analyse_menu.Append(histogram_menu)

        menu_bar.Append(file_menu, '&File')
        menu_bar.Append(self.scale_menu, 'Scale')
        menu_bar.Append(self.unit_of_energy, 'Units of Energy')
        menu_bar.Append(analyse_menu, 'Analyse')

        quit_item = wx.MenuItem(file_menu, wx.ID_EXIT, text = "&Quit\tCtrl+Q", kind = wx.ITEM_NORMAL)
        quit_item.SetBitmap(wx.Bitmap(g_dir_path + "/quit.png"))
        file_menu.Append(quit_item)



        self.SetMenuBar(menu_bar)
        #self.text = wx.TextCtrl(self,-1, style = wx.EXPAND|wx.TE_MULTILINE)
        self.Bind(wx.EVT_MENU, self.menuhandler)


    def setImageUnitOfEnergy(self, aUnitOfEnergy):
        print("Update the unit of energy to: ", aUnitOfEnergy)

        self.current_energy_unit = aUnitOfEnergy;

        self.unit_of_energy.Check(200, False);
        self.unit_of_energy.Check(201, False);
        self.unit_of_energy.Check(202, False);

        if aUnitOfEnergy == "eV":
            self.unit_of_energy.Check(200, True);
        elif aUnitOfEnergy == "keV":
            self.unit_of_energy.Check(201, True);
        elif aUnitOfEnergy == "MeV":
            self.unit_of_energy.Check(202, True);
        else:
            print("Unknown scale")

    def setObjectScale(self, aScale):
        print("Update the scale to: ", aScale)

        self.scale_menu.Check(100, False);
        self.scale_menu.Check(101, False);
        self.scale_menu.Check(102, False);
        self.scale_menu.Check(103, False);
        self.scale_menu.Check(104, False);

        if aScale == "um":
            self.scale_menu.Check(100, True);
        elif aScale == "mm":
            self.scale_menu.Check(101, True);
        elif aScale == "cm":
            self.scale_menu.Check(102, True);
        elif aScale == "dm":
            self.scale_menu.Check(103, True);
        elif aScale == "m":
            self.scale_menu.Check(104, True);
        else:
            print("Unknown scale")

    def redisplay(self):
        #size = self.canvas.GetGLExtents()
        #self.canvas.OnReshape(size.width, size.height)
        #self.canvas.Refresh(True)
        self.Refresh(True)
        #self.canvas.Update()

    def updateXRayImage(self):

        xray_image = np.divide(gvxr.computeXRayImage(), gvxr.getUnitOfEnergy(self.current_energy_unit));

        self.canvas.Show()
        self.canvas.Refresh(True)
        self.canvas.Update()

        self.image_viewer.data_ready = True;
        #self.image_viewer.draw();
        self.image_viewer.updateImage(xray_image);

        self.image_viewer.Show();
        self.image_viewer.Refresh(True);
        self.image_viewer.Update();

        return xray_image;


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
            self.canvas.setObjectScale("um");
        elif id == 101:
            print ("Change scale to mm");
            self.canvas.setObjectScale("mm");
        elif id == 102:
            print ("Change scale to cm");
            self.canvas.setObjectScale("cm");
        elif id == 103:
            print ("Change scale to dm");
            self.canvas.setObjectScale("dm");
        elif id == 104:
            print ("Change scale to m");
            self.canvas.setObjectScale("m");
        elif id == 200:
            print ("Change units of energy to eV");
            self.current_energy_unit = "eV";
            self.updateXRayImage();
        elif id == 201:
            print ("Change units of energy to keV");
            self.current_energy_unit = "keV";
            self.updateXRayImage();
        elif id == 202:
            print ("Change units of energy to eV");
            self.current_energy_unit = "MeV";
            self.updateXRayImage();
        elif id == 601:
            print ("Analyse->Measure");
            canvas = MeasureCanvas.createMeasureWindow();

        elif id == 602:
            print ("Analyse->Histogram");
            canvas = HistogramCanvas.createHistogramWindow();

        # Unknown ID
        else:
            print ("Event on widget ID (", id, "), not handled.")

    # Close the application
    def processCanvasCloseEvent(self, event):
        event.Skip()
        exit();
