# The main GUI application
import wxApp;

import math;

import numpy as np

# Matplotlib is used to display the 2D images an the histogram
import matplotlib

# wxPython is used to create the GUI
import wx
import wx.grid

import csv # Save csv files

# Matplotlib relies on the wxPython backend
matplotlib.use('WXAgg')
from matplotlib.figure import Figure
from matplotlib.backends.backend_wxagg import FigureCanvasWxAgg as FigureCanvas
from matplotlib.backends.backend_wx import NavigationToolbar2Wx

# gVirtualXRay's wrapper
from gvxrPython3 import gvxr;

import os; # Get the current path

import scipy;
from scipy import stats;

NoneType = type(None);

class HistogramFrame(wx.Frame):
    def __init__(self, parent):
        wx.Frame.__init__(self, parent)

        self.current_filename = None;

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

        self.status_bar = wx.StatusBar(self, -1)
        self.SetStatusBar(self.status_bar)

        self.Bind(wx.EVT_MENU, self.menuhandler)


        # Computethe X-ray image and its histogram
        xray_image = wxApp.g_wxapp.updateXRayImage();
        bins = 256;
        self.hist, self.bins = np.histogram(xray_image, bins = 256)


        self.figure = Figure()
        self.axes = self.figure.add_subplot(111)
        self.canvas = FigureCanvas(self, -1, self.figure)


        self.canvas.Bind(wx.EVT_ENTER_WINDOW, self.onMouseOver)
        self.canvas.Bind(wx.EVT_LEAVE_WINDOW, self.onMouseLeave)
        self.canvas.mpl_connect('motion_notify_event', self.onMouseMotion)

        self.sizer = wx.BoxSizer(wx.VERTICAL)
        self.sizer.Add(self.canvas, 1, wx.LEFT | wx.TOP | wx.GROW)

        hsizer = wx.BoxSizer(wx.HORIZONTAL)

        self.sizer.Add(hsizer, 0, wx.ALIGN_CENTER)
        vsizer1 = wx.BoxSizer(wx.VERTICAL);
        hsizer.Add(vsizer1, 0, wx.ALIGN_LEFT)

        vsizer2 = wx.BoxSizer(wx.VERTICAL);
        hsizer.Add(vsizer2, 0, wx.ALIGN_LEFT)

        #
        count = xray_image.shape[1] * xray_image.shape[0];
        mean_value = np.mean(xray_image);
        std = np.std(xray_image);
        bins = 256;
        min_value = np.min(xray_image);
        max_value = np.max(xray_image);
        self.bin_width = self.bins[1]-self.bins[0];

        font = wx.Font(10, wx.ROMAN, wx.NORMAL, wx.NORMAL)

        lbl = wx.StaticText(self,-1,style = wx.ALIGN_LEFT)
        lbl.SetFont(font)
        lbl.SetLabel("Total count: " + str(count) + "    ")
        vsizer1.Add(lbl, 0,wx.ALIGN_LEFT)

        lbl = wx.StaticText(self,-1,style = wx.ALIGN_LEFT)
        lbl.SetFont(font)
        lbl.SetLabel("Mean: " + str(mean_value) + "    ")
        vsizer1.Add(lbl, 0,wx.ALIGN_LEFT)

        lbl = wx.StaticText(self,-1,style = wx.ALIGN_LEFT)
        lbl.SetFont(font)
        lbl.SetLabel("StdDev: " + str(std) + "    ")
        vsizer1.Add(lbl, 0,wx.ALIGN_LEFT)

        lbl = wx.StaticText(self,-1,style = wx.ALIGN_LEFT)
        lbl.SetFont(font)
        lbl.SetLabel("Bins: " + str(bins) + "    ")
        vsizer1.Add(lbl, 0,wx.ALIGN_LEFT)

        lbl = wx.StaticText(self,-1,style = wx.ALIGN_LEFT)
        lbl.SetFont(font)
        lbl.SetLabel("Min: " + str(min_value))
        vsizer2.Add(lbl, 0,wx.ALIGN_LEFT)

        lbl = wx.StaticText(self,-1,style = wx.ALIGN_LEFT)
        lbl.SetFont(font)
        lbl.SetLabel("Max: " + str(max_value))
        vsizer2.Add(lbl, 0,wx.ALIGN_LEFT)

        lbl = wx.StaticText(self,-1,style = wx.ALIGN_LEFT)
        lbl.SetFont(font)
        lbl.SetLabel("Bin width: " + str(self.bin_width))
        vsizer2.Add(lbl, 0,wx.ALIGN_LEFT)


        log_button = wx.ToggleButton(self, -1, label="Log");
        self.sizer.Add(log_button, 0,wx.ALIGN_CENTRE)
        self.plot_log = False;
        log_button.Bind(wx.EVT_TOGGLEBUTTON, self.onToggle)

        self.SetSizer(self.sizer)
        self.Fit()

        self.draw();
        self.Show();

    def onMouseMotion(self, event):
        if event.inaxes:
            x, y = event.xdata, event.ydata

            value = self.bins[0];
            count = self.hist[0];

            for temp_value, temp_count in zip(self.bins[:-1], self.hist):

                if value <= x and x <= temp_value:
                    value = temp_value;
                    count = temp_count;

            if x >= self.bins[len(self.hist)-1]:
                value = self.bins[len(self.hist)-1];
                count = self.hist[-1];

            self.status_bar.SetStatusText(("value= " + str(value) + "   count=" + str(count)))

    def onMouseOver(self, event):
        self.canvas.SetCursor(wx.Cursor(wx.CURSOR_BULLSEYE));
        event.Skip();

    def onMouseLeave(self, event):
        self.canvas.SetCursor(wx.Cursor(wx.CURSOR_ARROW));
        event.Skip();

    def onToggle(self, event):
        self.plot_log = event.GetEventObject().GetValue();
        self.draw();

    def draw(self):
        self.axes.clear();
        self.axes.set_title("Histogram");
        self.axes.set_ylabel("Count");
        self.axes.set_xlabel("Bin value in " + wxApp.g_wxapp.current_energy_unit);

        max_value1 = np.max(self.hist);
        max_value2 = np.max(self.hist[:-1]);


        if max_value1 / max_value2 > 1:
            ylim = max_value2 * 1.5;
        else:
            ylim = max_value1;

        axes = None;


        if self.plot_log:

            Y = np.log(self.hist + 1);

            max_value1 = np.max(Y);
            max_value2 = np.max(Y[:-1]);


            if max_value1 / max_value2 > 1:
                ylim2 = max_value2 * 1.125;
            else:
                ylim2 = max_value1;

            color = 'tab:red'
            self.axes.bar(self.bins[:-1], Y, width=self.bin_width, color=color)
            self.axes.set_ylim((np.min(Y), ylim2))
            self.axes.tick_params(axis='y', labelcolor=color)
            self.axes.axes.set_yticks([], [])
            axes = self.axes.twinx();
        else:
            axes = self.axes;



        color = 'tab:blue';
        axes.bar(self.bins[:-1], self.hist, width=self.bin_width, color=color)
        axes.set_ylim((np.min(self.hist), ylim))
        axes.set_yticks([], [])



        self.figure.tight_layout()


        self.figure.canvas.draw();
        self.Refresh(True);

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

        table = [];
        table.append(["value", "count"]);

        for value, count in zip(self.bins[:-1], self.hist):
            table.append([value, count]);

        # first cleanout empty rows
        with open(aFileName, 'w', newline='') as csvfile:
            writer = csv.writer(csvfile, dialect='excel', delimiter=separator);
            writer.writerows(table);

def createHistogramWindow():
    HistogramFrame(None);
