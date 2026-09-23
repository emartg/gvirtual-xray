# The main GUI application
import wxApp;


import numpy as np

# Matplotlib is used to display the 2D images an the histogram
import matplotlib

# wxPython is used to create the GUI
import wx
import wx.grid

import csv # Save csv files

# Matplotlib relies on the wxPython backend
matplotlib.use('WXAgg')
from matplotlib.backends.backend_wxagg import FigureCanvasWxAgg as FigureCanvas
from matplotlib.backends.backend_wx import NavigationToolbar2Wx

# gVirtualXRay's wrapper
from gvxrPython3 import gvxr;

import os; # Get the current path

import scipy;
from scipy import stats;

NoneType = type(None);
measure_grid_frame = None;

class MeasureGridFrame(wx.Frame):
    def __init__(self, parent):
        measure_grid_frame = self;
        wx.Frame.__init__(self, parent)

        self.current_filename = None;

        # Create a wxGrid object
        self.grid = wx.grid.Grid(self, -1)

        self.column_titles = [];

        self.column_titles.append("Width");
        self.column_titles.append("Height");
        self.column_titles.append("Area");
        self.column_titles.append("Perimetre");

        self.column_titles.append("Units of energy");
        self.column_titles.append("Min");
        self.column_titles.append("Max");
        self.column_titles.append("Median");
        self.column_titles.append("Mean");
        self.column_titles.append("Std dev");

        self.column_titles.append("Skewness");
        self.column_titles.append("Kurtosis");
        self.column_titles.append("Shannon entropy");

        self.table = [];
        self.table.append(self.column_titles);

        # Then we call CreateGrid to set the dimensions of the grid
        # (1 rows and 13 columns in this example)
        self.grid.CreateGrid(1, len(self.column_titles))

        # We can set the sizes of individual rows and columns
        # in pixels
        #self.grid.SetRowSize(0, 60)
        #self.grid.SetColSize(0, 120)

        # Add title in grid
        for title, i in zip(self.column_titles, range(len(self.column_titles))):
            self.grid.SetCellValue(0, i, title);
            self.grid.SetReadOnly(0, i);
            self.grid.SetCellBackgroundColour(0, i, wx.LIGHT_GREY);
            self.grid.SetCellTextColour(0, i, wx.BLACK);

            if i >= 5:
                self.grid.SetColFormatFloat(i, 6, 3);



        # Colours can be specified for grid cell contents
        #self.grid.SetCellValue(3, 3, 'green on grey')
        #self.grid.SetCellTextColour(3, 3, wx.GREEN)

        # We can specify the some cells will store numeric
        # values rather than strings. Here we set grid column 5
        # to hold floating point values displayed with width of 6
        # and precision of 2
        #self.grid.SetColFormatFloat(5, 6, 2)
        #self.grid.SetCellValue(0, 6, '3.1415')

        self.addRow();

        # Create the file menu
        menu_bar = wx.MenuBar()
        file_menu = wx.Menu()
        menu_bar.Append(file_menu, '&File')

        save_item = wx.MenuItem(file_menu, wx.ID_SAVE, text = "&Save\tCtrl+S",kind = wx.ITEM_NORMAL)
        file_menu.Append(save_item)

        save_as_item = wx.MenuItem(file_menu, wx.ID_SAVEAS, text = "&Save as...\tCtrl+Shift+S",kind = wx.ITEM_NORMAL)
        file_menu.Append(save_as_item)

        file_menu.AppendSeparator()

        close_item = wx.MenuItem(file_menu, wx.ID_CLOSE, text = "&Close\tCtrl+W", kind = wx.ITEM_NORMAL)
        file_menu.Append(close_item)


        self.SetMenuBar(menu_bar)

        self.Bind(wx.EVT_MENU, self.menuhandler)
        self.Bind(wx.EVT_CLOSE, self.processCloseEvent);

        self.Show();

    def menuhandler(self, event):
        id = event.GetId()
        if id == wx.ID_SAVE:
            if isinstance(self.current_filename, NoneType):
                self.onSaveAs();
            else:
                self.onSave();
        elif id == wx.ID_SAVEAS:
            self.onSaveAs();
        elif id == wx.ID_CLOSE:
            self.Destroy();

    def processCloseEvent(self, event):
        global measure_grid_frame;
        measure_grid_frame = None;
        event.Skip()
        print("Close");

    def onSave(self, event=None):

        # Is there a filename already?
        if isinstance(self.current_filename, NoneType):
            self.onSaveAs();
        else:
            self.saveFile(self.current_filename);

    def onSaveAs(self, event=None):

        # Is there a filename already?
        if isinstance(self.current_filename, NoneType):
            cur_dir = os.getcwd()
            cur_name = ""
        else:
            cur_dir, cur_name = os.path.split(self.current_filename)

        # Open a file dialog box
        dlg = wx.FileDialog(self, 'Filename to save',
                            cur_dir,
                            cur_name,
                            wildcard='CSV and TSV files (*.csv;*.tsv)|*.csv;*.tsv|Comma separated files (*.csv)|*.csv|Tab separated files (*.tsv)|*.tsv',
                            style=wx.FD_SAVE | wx.FD_OVERWRITE_PROMPT);

        # Show the file dialog box
        if dlg.ShowModal() == wx.ID_OK:
            self.current_filename = dlg.GetPath();
            self.saveFile(self.current_filename);

        # Destroy the file dialog box
        dlg.Destroy();

    def saveFile(self, aFileName):
        """
        save the table to the given filename
        """

        # Get the extension
        name, ext = os.path.splitext(aFileName);

        if ext.lower() == '.csv':
            separator=',';
        elif ext.lower() == '.tsv':
            separator='\t';
        else:
            print("unknow file extension")
            return;

        # first cleanout empty rows
        with open(aFileName, 'w', newline='') as csvfile:
            writer = csv.writer(csvfile, dialect='excel', delimiter=separator);
            writer.writerows(self.table);

    def addRow(self):

        xray_image = wxApp.g_wxapp.updateXRayImage();

        self.grid.AppendRows();
        number_of_rows = self.grid.GetNumberRows();

        row = [];

        row.append(xray_image.shape[1]);
        row.append(xray_image.shape[0]);
        row.append(xray_image.shape[0] * xray_image.shape[1]);
        row.append(2 * (xray_image.shape[0] + xray_image.shape[1]));
        row.append(wxApp.g_wxapp.current_energy_unit);
        row.append(np.min(xray_image));
        row.append(np.max(xray_image));
        row.append(np.median(xray_image));
        row.append(np.mean(xray_image));
        row.append(np.std(xray_image));
        row.append(stats.skew(xray_image.reshape(-1)));
        row.append(stats.kurtosis(xray_image.reshape(-1)));
        row.append(stats.entropy(xray_image.reshape(-1)));

        self.table.append(row);

        for i, cell in zip(range(len(row)), row):
            self.grid.SetReadOnly(number_of_rows - 1, i);
            self.grid.SetCellValue(number_of_rows - 1, i, str(cell));

def createMeasureWindow():
    global measure_grid_frame;

    if isinstance(measure_grid_frame, NoneType):
        measure_grid_frame = MeasureGridFrame(None);
    else:
        measure_grid_frame.addRow();
