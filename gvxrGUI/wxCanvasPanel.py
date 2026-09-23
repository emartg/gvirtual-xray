
import numpy as np

# To get FLT_MAX
import sys

# wxPython is used to create the GUI
import wx

# Matplotlib is used to display the 2D images an the histogram
import matplotlib

# Matplotlib relies on the wxPython backend
matplotlib.use('WXAgg')
from matplotlib.backends.backend_wxagg import FigureCanvasWxAgg as FigureCanvas
from matplotlib.backends.backend_wx import NavigationToolbar2Wx

from matplotlib.figure import Figure
from mpl_toolkits.axes_grid1 import make_axes_locatable

# Use different scales to display the X-ray image (log and power)
from matplotlib.colors import LogNorm
from matplotlib.colors import PowerNorm

# gVirtualXRay's wrapper
from gvxrPython3 import gvxr;

# The main GUI application
import wxApp;
import GlobalParameters as GP;


NoneType = type(None);

class wxCanvasPanel(wx.Frame):
    def __init__(self, parent, aParamContainer):
        wx.Frame.__init__(self, parent)

        menu_bar = wx.MenuBar()
        file_menu = wx.Menu()
        menu_bar.Append(file_menu, '&File')

        self.im = None;
        self.colorbar = None;
        self.x_ray_image = None;
        self.shift = 0.0;
        self.scale = 1.0;
        self.last_x_px = 0;
        self.last_y_px = 0;
        '''
        save_item = wx.MenuItem(file_menu, wx.ID_SAVE, text = "&Save\tCtrl+S",kind = wx.ITEM_NORMAL)
        file_menu.Append(save_item)

        save_as_item = wx.MenuItem(file_menu, wx.ID_SAVEAS, text = "&Save as...\tCtrl+Shift+S",kind = wx.ITEM_NORMAL)
        file_menu.Append(save_as_item)

        file_menu.AppendSeparator()

        close_item = wx.MenuItem(file_menu, wx.ID_CLOSE, text = "&Close\tCtrl+W", kind = wx.ITEM_NORMAL)
        file_menu.Append(close_item)
        '''

        self.SetMenuBar(menu_bar)

        self.status_bar = wx.StatusBar(self, -1)
        self.SetStatusBar(self.status_bar)

        #self.Bind(wx.EVT_MENU, self.menuhandler)

        self.figure = Figure()
        self.axes = self.figure.add_subplot(111)
        self.canvas = FigureCanvas(self, -1, self.figure)

        self.left_button_down = False;
        self.previous_cursor = wx.CURSOR_ARROW;
        self.current_cursor = wx.CURSOR_ARROW;
        self.canvas.Bind(wx.EVT_ENTER_WINDOW, self.onMouseOver)
        self.canvas.Bind(wx.EVT_LEAVE_WINDOW, self.onMouseLeave)
        self.canvas.Bind(wx.EVT_LEFT_DOWN, self.onLeftButtonDown)
        self.canvas.Bind(wx.EVT_LEFT_UP, self.onLeftButtonUp)

        self.canvas.mpl_connect('motion_notify_event', self.onMouseMotion)

        self.sizer = wx.BoxSizer(wx.VERTICAL)
        self.sizer.Add(self.canvas, 1, wx.LEFT | wx.TOP | wx.GROW)

        self.rb1 = wx.RadioButton(self, 11, label = 'Linear colour scale')
        self.rb2 = wx.RadioButton(self, 22, label = 'Log colour scale')
        self.rb3 = wx.RadioButton(self, 33, label = 'Power-law colour scale')

        self.sizer.Add(self.rb1, 0, wx.LEFT | wx.TOP | wx.GROW)
        self.sizer.Add(self.rb2, 0, wx.LEFT | wx.TOP | wx.GROW)
        self.sizer.Add(self.rb3, 0, wx.LEFT | wx.TOP | wx.GROW)

        self.color_scale = 0;
        self.Bind(wx.EVT_RADIOBUTTON, self.onRadiogroup);

        self.SetSizer(self.sizer)
        self.Fit()

        self.data_ready = False;
        self.current_energy_unit = aParamContainer;

        #self.draw();
        #self.Show();
    def onRadiogroup(self, event):
        id = event.GetId()
        if id == 11:
            self.color_scale = 0;
        elif id == 22:
            self.color_scale = 1;
        elif id == 33:
            self.color_scale = 2;

        self.draw();
        self.figure.canvas.draw();
        self.Refresh(True);
        self.Update()
        self.Show();

    def onMouseMotion(self, event):
        if event.inaxes:
            unit_of_length = "cm";

            x_px, y_px = int(event.xdata), int(event.ydata);

            if self.left_button_down:
                self.shift += 0.00000001 * (self.last_x_px - x_px);
                self.scale += 0.000001 * (self.last_y_px - y_px);

                self.updateImage(self.x_ray_image);
                self.Refresh(True);
                self.Update();

            else:
                if not isinstance(self.x_ray_image, NoneType):
                    if x_px >= self.x_ray_image.shape[1]:
                        x_px = self.x_ray_image.shape[1] - 1;

                    if y_px >= self.x_ray_image.shape[0]:
                        y_px = self.x_ray_image.shape[0] - 1;

                    x_unit_of_length = x_px * gvxr.getDetectorSize(unit_of_length)[0] / gvxr.getDetectorNumberOfPixels()[0];
                    y_unit_of_length = y_px * gvxr.getDetectorSize(unit_of_length)[1] / gvxr.getDetectorNumberOfPixels()[1];

                    x_unit_of_length = "{:.3f}".format(x_unit_of_length)
                    y_unit_of_length = "{:.3f}".format(y_unit_of_length)

                    self.status_bar.SetStatusText("x=" +
                        x_unit_of_length + unit_of_length +
                        " (" +
                        str(x_px) +
                        "px), y=" +
                        y_unit_of_length + unit_of_length +
                        " (" +
                        str(y_px) +
                        "px), value= " +
                        str(self.x_ray_image[y_px, x_px]) +
                        self.current_energy_unit);

            self.last_x_px = x_px;
            self.last_y_px = y_px;

    def onLeftButtonDown(self, event):
        self.previous_cursor = self.current_cursor;
        self.current_cursor  = wx.CURSOR_ARROW;
        self.canvas.SetCursor(wx.Cursor(self.current_cursor));
        self.left_button_down = True;
        event.Skip();

    def onLeftButtonUp(self, event):
        self.current_cursor  = self.previous_cursor;
        self.previous_cursor = self.current_cursor;
        self.canvas.SetCursor(wx.Cursor(self.current_cursor));
        self.left_button_down = False;
        event.Skip();

    def onMouseOver(self, event):
        self.previous_cursor = self.current_cursor;
        self.current_cursor  = wx.CURSOR_BULLSEYE;
        self.canvas.SetCursor(wx.Cursor(self.current_cursor));
        event.Skip();

    def onMouseLeave(self, event):
        self.previous_cursor = self.current_cursor;
        self.current_cursor  = wx.CURSOR_ARROW;
        self.canvas.SetCursor(wx.Cursor(self.current_cursor));
        self.status_bar.SetStatusText("");
        event.Skip();

    def draw(self):

        if self.data_ready:
            print("DRAW")
            self.axes.clear();

            if isinstance(self.colorbar, NoneType) == False:
                self.colorbar.remove();

            self.x_ray_image = np.divide(gvxr.computeXRayImage(), gvxr.getUnitOfEnergy(self.current_energy_unit));

            min_value = np.min(self.x_ray_image);
            max_value = np.max(self.x_ray_image);

            min_value += self.shift;
            min_value *= self.scale;

            max_value += self.shift;
            max_value *= self.scale;

            wxApp.g_need_refresh_xray_image = False;

            cmap = "gray";
            norm = None;
            if self.color_scale == 0:
                self.axes.set_title("Using a linear colour scale");
            elif self.color_scale == 1:
                self.axes.set_title("Using a log colour scale");
                norm=LogNorm();
            elif self.color_scale == 2:
                self.axes.set_title("Using a Power-law colour scale");
                norm=PowerNorm(gamma=1./2.);

            self.im = self.axes.imshow(self.x_ray_image, cmap=cmap, norm=norm, vmin=min_value, vmax=max_value);

            divider = make_axes_locatable(self.axes)
            cax = divider.append_axes('bottom', size='25%', pad=0.5)
            self.colorbar = self.figure.colorbar(self.im, cax=cax, orientation='horizontal');


            self.figure.canvas.draw();
            self.Refresh(True);
            self.Update()
            self.Show();

    def updateImage(self, image):
        print("updateImage")
        if self.data_ready:

            if isinstance(self.im, NoneType):
                print("YEAH")
                self.draw();
            else:
                self.draw();
                self.im.set_data(image);
                self.figure.canvas.Refresh(True);
                self.figure.canvas.Update()
                self.Refresh(True);
                self.Update()
