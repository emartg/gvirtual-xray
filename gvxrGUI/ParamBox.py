import wx
import wx.dataview as dv

from gvxrPython3 import gvxr

# The main GUI application
import wxApp;

g_param_box = None;

class ParamBox( wx.Frame ):

	def __init__( self, parent = None, title = "Control Panel" ):

		super(ParamBox, self).__init__(parent, id = wx.ID_ANY, title = title, pos = wx.DefaultPosition, size = wx.Size( 600,500 ), style = wx.DEFAULT_FRAME_STYLE|wx.TAB_TRAVERSAL )

		global g_param_box;

		g_param_box = self;


		self.SetSizeHints( wx.DefaultSize, wx.DefaultSize )

		gSizer2 = wx.GridSizer( 1, 1, 0, 0 )

		bSizer1 = wx.BoxSizer( wx.VERTICAL )

		#bSizer1.AddSpacer( ( 0, 0), 1, wx.EXPAND, 5 )
		self.Mat_lbl1 = wx.StaticText( self, wx.ID_ANY, u"Beam Intensity :", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.Mat_lbl1.Wrap( -1 )
		bSizer1.Add( self.Mat_lbl1, 0, wx.ALL, 5 )
		self.m_slider1 = wx.Slider( self, wx.ID_ANY, 50, 1, 1000, wx.DefaultPosition, wx.Size( 215,20 ), wx.SL_HORIZONTAL|wx.SL_LABELS )
		bSizer1.Add( self.m_slider1, 0, wx.ALL, 5 )
		self.m_slider1.Bind(wx.EVT_SLIDER, self.OnSliderScroll)
		self.unit_of_energy_choices = [ u"eV", u"keV", u"MeV", wx.EmptyString ]

		self.m_choice1 = wx.Choice( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, self.unit_of_energy_choices, 0 )
		self.m_choice1.SetSelection( 1 )
		self.m_choice1.Bind(wx.EVT_CHOICE, self.onUnitOfEnergy)
		self.current_energy_unit = self.unit_of_energy_choices[self.m_choice1.GetSelection()];
		bSizer1.Add( self.m_choice1, 0, wx.ALL, 5 )

		self.Mat_lbl = wx.StaticText( self, wx.ID_ANY, u"Material :", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.Mat_lbl.Wrap( -1 )
		bSizer1.Add( self.Mat_lbl, 0, wx.ALL, 5 )

		m_choice2Choices = [ wx.EmptyString ]
		self.m_choice2 = wx.Choice( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, m_choice2Choices, 0 )
		self.m_choice2.SetSelection( 1 )
		#bSizer1.Add( self.m_choice2, 0, wx.ALL, 5 )

		m_choice2Choices = [];
		for i in range(1, 100):
			temp_string = "\"";
			temp_string += gvxr.getElementName(i);
			temp_string += "\"";

			m_choice2Choices.append(temp_string);

		self.m_choice2 = wx.Choice( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, m_choice2Choices, 0 )
		self.m_choice2.SetSelection( 0 )
		bSizer1.Add( self.m_choice2, 0, wx.ALL, 5 )

		bSizer5 = wx.BoxSizer( wx.VERTICAL )

		gSizer21 = wx.GridSizer( 4, 3, 0, 0 )

		self.HU = wx.RadioButton( self, wx.ID_ANY, u"Hounsfield", wx.DefaultPosition, wx.DefaultSize, style = wx.RB_GROUP )
		gSizer21.Add( self.HU, 0, wx.ALIGN_CENTER_HORIZONTAL|wx.ALL, 5 )

		self.Mixture = wx.RadioButton( self, wx.ID_ANY, u"Mixture", wx.DefaultPosition, wx.DefaultSize, 0 )
		gSizer21.Add( self.Mixture, 0, wx.ALIGN_CENTER_HORIZONTAL|wx.ALL, 5 )

		self.Compound = wx.RadioButton( self, wx.ID_ANY, u"Compound", wx.DefaultPosition, wx.DefaultSize, 0 )
		gSizer21.Add( self.Compound, 0, wx.ALIGN_CENTER_HORIZONTAL|wx.ALL, 5 )

		self.m_slider11 = wx.Slider( self, wx.ID_ANY, 50, -1000, 3000, wx.DefaultPosition, wx.Size( 215,20 ), wx.SL_HORIZONTAL|wx.SL_LABELS )
		gSizer21.Add( self.m_slider11, 0, wx.ALL, 5 )
		self.m_slider11.Bind(wx.EVT_SLIDER, self.OnSliderScroll2)
		self.Bind(wx.EVT_RADIOBUTTON, self.OnRadiogroup)
		#huBool = self.HU.GetValue()
		#if huBool(True):
			#self.Mixture.Disable();
			#self.Compound.Disable();
		#if self.Mixture.GetValue:
		#	self.Compound.Disable();
		#	self.HU.Disable();

		m_choice3Choices = []
		self.m_choice3 = wx.Choice( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, m_choice3Choices, 0 )
		self.m_choice3.SetSelection( 0 )
		gSizer21.Add( self.m_choice3, 0, wx.ALIGN_CENTER, 5 )



		m_choice4Choices = []
		self.m_choice4 = wx.Choice( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, m_choice4Choices, 0 )
		self.m_choice4.SetSelection( 0 )
		gSizer21.Add( self.m_choice4, 0, wx.ALIGN_CENTER|wx.ALIGN_LEFT, 5 )


		bSizer5.Add( gSizer21, 1, 0, 10 )


		bSizer1.Add( bSizer5, 1, wx.EXPAND, 5 )

		self.m_tree = wx.TreeCtrl( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, wx.TR_DEFAULT_STYLE )
		self.m_tree.SetMinSize( wx.Size( 250,80 ) )

		bSizer1.Add( self.m_tree, 0, wx.ALIGN_CENTER|wx.ALL, 5 )

		#self.m_TreeCtr = dv.DataViewTreeCtrl( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, 0 )
		#bSizer1.Add( self.m_TreeCtr, 0, wx.ALIGN_CENTER|wx.ALL, 5 )





		gSizer2.Add( bSizer1, 1, wx.EXPAND, 5 )


		self.SetSizer( gSizer2 )
		self.Layout()
		self.m_menubar1 = wx.MenuBar( 0 )
		self.File = wx.Menu()
		self.Open_Menu = wx.MenuItem( self.File, wx.ID_ANY, u"Open", wx.EmptyString, wx.ITEM_NORMAL )
		self.File.Append( self.Open_Menu )

		self.m_menubar1.Append( self.File, u"File" )

		self.SetMenuBar( self.m_menubar1 )


		self.Centre( wx.BOTH )




		# Update X-ray image
		#gvxr.computeXRayImage();

			#REFRESH/UPDATE the widget/windows
		self.Show(True)

		self.Bind(wx.EVT_CLOSE,     self.processCanvasCloseEvent)

	# Close the application
	def processCanvasCloseEvent(self, event):
		event.Skip()
		exit();

	def loadTreeView(self):
		node_label='root';
		children=gvxr.getNumberOfChildren(node_label);
		node_id = self.m_tree.AddRoot(node_label)

		list_of_parents = [];

		if children:
			list_of_parents.append((node_label, node_id));

		while len(list_of_parents):
			(parent_label, parent_id) = list_of_parents[-1];
			list_of_parents.pop();

			for i in range(gvxr.getNumberOfChildren(parent_label)):
				child_label = gvxr.getChildLabel(parent_label, i);
				child_children = gvxr.getNumberOfChildren(child_label);

				# Get the mesh colour in float
				colour_float = gvxr.getAmbientColour(child_label)[0:3];

				# Convert it in UCHAR
				colour_int = [int(i * 255) for i in colour_float];

				node_id = self.m_tree.AppendItem(parent_id, child_label)
				self.m_tree.SetItemTextColour(node_id, wx.Colour(colour_int[0], colour_int[1], colour_int[2]));

				# Initialise the material properties
				if self.HU:
					gvxr.setHU(child_label, self.m_slider11.GetValue());
				else:
					Z = 1 + self.m_choice2.GetValue;
					gvxr.setElement(last_node, gvxr.getElementName(Z));

				if child_children:
					list_of_parents.append((child_label, node_id));

	def OnSliderScroll(self, e):
		obj = e.GetEventObject()
		val = obj.GetValue()

		# Update the energy
		gvxr.setMonoChromatic(val, self.current_energy_unit, 1);

		wxApp.g_wxapp.updateXRayImage();
		print(self.m_slider1.GetValue(), self.current_energy_unit)

		# Redraw
		#self.parent.__OnDraw__(None);
		#self.parent.Show()
		#self.parent.Refresh(True);
		#self.parent.Update();

	def onUnitOfEnergy(self, e):

		# Update the unit of energy
		self.current_energy_unit = self.unit_of_energy_choices[self.m_choice1.GetSelection()];

		print(self.m_slider1.GetValue(), self.current_energy_unit)
		gvxr.setMonoChromatic(self.m_slider1.GetValue(), self.current_energy_unit, 1);

		wxApp.g_wxapp.image_viewer.current_energy_unit = self.current_energy_unit;

		wxApp.g_wxapp.updateXRayImage();
		# Redraw
		#self.parent.__OnDraw__(None);
		#self.parent.Show()
		#self.parent.Refresh(True);
		#self.parent.Update();


	def OnSliderScroll2(self, e):
		obj = e.GetEventObject()
		val = obj.GetValue()

	def OnRadiogroup(self, e):
		rb = e.GetEventObject()
		print (rb.GetLabel(),' is clicked from Radio Group')
		if rb.GetLabel("Hounsfield"):
			#self.Mixture.Disable();
			#self.Compound.Disable();
			self.m_choice4.Disable();
			self.m_choice3.Disable();
		#if rb.GetLabel("Mixture"):
			#self.Hounsfield.Disable();
			#self.Compound.Disable();
