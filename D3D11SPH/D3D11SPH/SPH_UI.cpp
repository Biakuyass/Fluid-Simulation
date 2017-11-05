#include "SPH_UI.h"


CEGUI::DefaultWindow * SPH_UIControl::m_root = 0;

MarchingCube* SPH_UIControl::m_pMarchingCube = 0;

SPHSystem* SPH_UIControl::m_pSPHSystem = 0;

CEGUI::Window* SPH_UIControl::m_pSPHPanel = 0;

CEGUI::Window* SPH_UIControl::ControlPanel = 0;

CEGUI::Window* SPH_UIControl::MarchingCubePanel = 0;

ID3D11Device * SPH_UIControl::m_pd3dDevice = 0;

CEGUI::ListboxItem* SPH_UIControl::RenderMode = 0;

CEGUI::ListboxItem* SPH_UIControl::MarchingCubeMode = 0;

CEGUI::ListboxItem* SPH_UIControl::BillBoardMode = 0;

CEGUI::ListboxItem* SPH_UIControl::ParticleMode = 0;

void CeGUIBasicInit(ID3D11Device * pd3dDevice,ID3D11DeviceContext * m_pd3dDevContext)
{
	CEGUI::Direct3D11Renderer& myRenderer =
		CEGUI::Direct3D11Renderer::bootstrapSystem( pd3dDevice,m_pd3dDevContext );

	CEGUI::DefaultResourceProvider* rp = static_cast<CEGUI::DefaultResourceProvider*>
		(CEGUI::System::getSingleton().getResourceProvider());

	rp->setResourceGroupDirectory("schemes", "datafiles/schemes/");
	rp->setResourceGroupDirectory("imagesets", "datafiles/imagesets/");
	rp->setResourceGroupDirectory("fonts", "datafiles/fonts/");
	rp->setResourceGroupDirectory("layouts", "datafiles/layouts/");
	rp->setResourceGroupDirectory("looknfeels", "datafiles/looknfeel/");
	rp->setResourceGroupDirectory("lua_scripts", "datafiles/lua_scripts/");

	CEGUI::ImageManager::setImagesetDefaultResourceGroup("imagesets");
	CEGUI::Font::setDefaultResourceGroup("fonts");
	CEGUI::Scheme::setDefaultResourceGroup("schemes");
	CEGUI::WidgetLookManager::setDefaultResourceGroup("looknfeels");
	CEGUI::WindowManager::setDefaultResourceGroup("layouts");
	CEGUI::ScriptModule::setDefaultResourceGroup("lua_scripts");


	CEGUI::SchemeManager::getSingleton().createFromFile( "TaharezLook.scheme" );
	CEGUI::SchemeManager::getSingleton().createFromFile("AlfiskoSkin.scheme");
	CEGUI::SchemeManager::getSingleton().createFromFile("WindowsLook.scheme");
	CEGUI::SchemeManager::getSingleton().createFromFile("VanillaSkin.scheme");
	CEGUI::SchemeManager::getSingleton().createFromFile("OgreTray.scheme");
}
void SPH_UIControl::Init(ID3D11Device * pd3dDevice,ID3D11DeviceContext * m_pd3dDevContext,SPHSystem & sph_system,MarchingCube & sph_marchingcube)//
{
	m_pd3dDevice = pd3dDevice;
	m_pSPHSystem = &sph_system;
	m_pMarchingCube = &sph_marchingcube;
	//Init

	CeGUIBasicInit(pd3dDevice,m_pd3dDevContext);

	CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();

	m_root = (CEGUI::DefaultWindow*)winMgr.createWindow("DefaultWindow","root");

	CEGUI::Font& defaultFont = CEGUI::FontManager::getSingleton().createFromFile("DejaVuSans-10.font");

	CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow( m_root );
	CEGUI::System::getSingleton().getDefaultGUIContext().setDefaultFont(&defaultFont);




	// column control section
	CEGUI::FrameWindow* fwnd = static_cast<CEGUI::FrameWindow*>(winMgr.createWindow("TaharezLook/FrameWindow", "SPHFrame"));
	fwnd->setPosition(CEGUI::UVector2(cegui_reldim(0), cegui_reldim( 0)));
	fwnd->setMaxSize(CEGUI::USize(cegui_reldim(1.0f), cegui_reldim( 1.0f)));
	fwnd->setSize(CEGUI::USize(cegui_reldim(0.25f), cegui_reldim( 0.50f)));
	fwnd->setText("SPH Control Panel");
	m_root->addChild(fwnd);

	m_pSPHPanel = winMgr.createWindow("TaharezLook/StaticText", "SPHPanel");
	m_pSPHPanel->setPosition(CEGUI::UVector2(cegui_reldim(0.02f), cegui_reldim(0.02f)));
	m_pSPHPanel->setSize(CEGUI::USize(cegui_reldim(0.97f), cegui_reldim( 0.9f)));
	m_pSPHPanel->setText("SPH Variable");
	m_pSPHPanel->setProperty("VertFormatting", "TopAligned");
	fwnd->addChild(m_pSPHPanel);



	CEGUI::Window* Label;

	CEGUI::Editbox* editbox;

	CEGUI::String LabelName[15] = {
		"Particle N_Label","WallLength_Label","UnitScale_Label",
		"Point_Mass_Label","SmoothRadius_Label","Viscosity_Label",
		"RestDensity_Label","GasConstantK_Label","Gravity_Label",
		"BoundaryDampening_Label","BoundaryCoe_Label","UpdateInterval_Label",
		"CubeSize_Label","CubeRange_Label","IsoValue_Label"
		
	};

	CEGUI::String LabelText[15] = {
		"Particle N","WallLength","UnitScale",
		"Point_Mass","SmoothRadius","Viscosity",
		"RestDensity","GasConstantK","Gravity",
		"BoundaryDampening","BoundaryCoe","UpdateInterval",
		"CubeSize","CubeRange","IsoValue"
		
	};
	

	CEGUI::String EditBoxName[15] = {
		"Particle N_Editbox","WallLength_Editbox","UnitScale_Editbox",
		"Point_Mass_Editbox","SmoothRadius_Editbox","Viscosity_Editbox",
		"RestDensity_Editbox","GasConstantK_Editbox","Gravity_Editbox",
		"BoundaryDampening_Editbox","BoundaryCoe_Editbox","UpdateInterval_Editbox",
		"CubeSize_Editbox","CubeRange_Editbox","IsoValue_Editbox"
		
	};

	CEGUI::String EditBoxText[15];
	EditBoxText[0] = CEGUI::PropertyHelper<float>::toString((float)m_pSPHSystem->m_iParticleCount);
	EditBoxText[1] = CEGUI::PropertyHelper<float>::toString(m_pSPHSystem->WallLength());
	EditBoxText[2] = CEGUI::PropertyHelper<float>::toString(m_pSPHSystem->UnitScale());
	EditBoxText[3] = CEGUI::PropertyHelper<float>::toString(m_pSPHSystem->PointMass());
	EditBoxText[4] = CEGUI::PropertyHelper<float>::toString(m_pSPHSystem->SmoothRadius());
	EditBoxText[5] = CEGUI::PropertyHelper<float>::toString(m_pSPHSystem->Viscosity());
	EditBoxText[6] = CEGUI::PropertyHelper<float>::toString(m_pSPHSystem->RestDensity());
	EditBoxText[7] = CEGUI::PropertyHelper<float>::toString(m_pSPHSystem->GasConstantK());
	EditBoxText[8] = CEGUI::PropertyHelper<float>::toString(m_pSPHSystem->Gravity());
	EditBoxText[9] = CEGUI::PropertyHelper<float>::toString(m_pSPHSystem->BoundaryDampening());
	EditBoxText[10] = CEGUI::PropertyHelper<float>::toString(m_pSPHSystem->BoundaryCoefficient());
	EditBoxText[11] = CEGUI::PropertyHelper<float>::toString(m_pSPHSystem->Updateinteval());
	EditBoxText[12] = CEGUI::PropertyHelper<float>::toString(m_pMarchingCube->CubeLength());
	EditBoxText[13] = CEGUI::PropertyHelper<float>::toString(m_pMarchingCube->CubeRange());
	EditBoxText[14] = CEGUI::PropertyHelper<float>::toString(m_pMarchingCube->Thresholdvalue());


	for(int i = 0;i < 4;i++)
		for(int m = 0; m < 3; m++)
		{
			if(i < 3)
			{
				Label = winMgr.createWindow("TaharezLook/Label",LabelName[i * 3 + m]);
				Label->setPosition(CEGUI::UVector2(cegui_reldim(0.05f + m * 0.34f), cegui_reldim( 0.085f + i * 0.165f)));
				Label->setSize(CEGUI::USize(cegui_reldim(0.4f), cegui_reldim(0.065f)));
				Label->setText(LabelText[i * 3 + m]);
				Label->setProperty("HorzFormatting", "LeftAligned");
				m_pSPHPanel->addChild(Label);

				editbox = static_cast<CEGUI::Editbox*>(winMgr.createWindow("TaharezLook/Editbox", EditBoxName[i * 3 + m]));
				editbox->setPosition(CEGUI::UVector2(cegui_reldim(0.05f + m * 0.34f), cegui_reldim( 0.16f + i * 0.165f)));
				editbox->setSize(CEGUI::USize(cegui_reldim(0.25f), cegui_reldim(0.063f)));
				editbox->setText(EditBoxText[i * 3 + m]);//
				editbox->setReadOnly(false);

				if("Particle N_Editbox" == EditBoxName[i * 3 + m])
					editbox->setValidationString("^([1-9]\\d*)?$");
				else if("Gravity_Editbox" == EditBoxName[i * 3 + m])
				    editbox->setValidationString("^-?(\\d+[.]?)?\\d*$");
				else 
					editbox->setValidationString("^(\\d+[.]?)?\\d*$");


				m_pSPHPanel->addChild(editbox);

			}
			else if(i * 3 + m <= 10)
			{
				Label = winMgr.createWindow("TaharezLook/Label", LabelName[i * 3 + m]);
				Label->setPosition(CEGUI::UVector2(cegui_reldim(0.05f + m * 0.5f), cegui_reldim( 0.085f + i * 0.165f)));
				Label->setSize(CEGUI::USize(cegui_reldim(0.48f), cegui_reldim(0.07f)));
				Label->setText(LabelText[i * 3 + m]);
				Label->setProperty("HorzFormatting", "LeftAligned");
				m_pSPHPanel->addChild(Label);

				editbox = static_cast<CEGUI::Editbox*>(winMgr.createWindow("TaharezLook/Editbox", EditBoxName[i * 3 + m]));
				editbox->setPosition(CEGUI::UVector2(cegui_reldim(0.05f + m * 0.5f), cegui_reldim( 0.16f + i * 0.165f)));
				editbox->setSize(CEGUI::USize(cegui_reldim(0.25f), cegui_reldim(0.063f)));
				editbox->setText(EditBoxText[i * 3 + m]);
				editbox->setReadOnly(false);
				editbox->setValidationString("^(\\d+[.]?)?\\d*$");

				m_pSPHPanel->addChild(editbox);
			}
			else
			{
				Label = winMgr.createWindow("TaharezLook/Label", LabelName[i * 3 + m]);
				Label->setPosition(CEGUI::UVector2(cegui_reldim(0.05f ), cegui_reldim( 0.25f + i * 0.165f)));
				Label->setSize(CEGUI::USize(cegui_reldim(0.48f), cegui_reldim(0.07f)));
				Label->setText(LabelText[i * 3 + m]);
				Label->setProperty("HorzFormatting", "LeftAligned");
				m_pSPHPanel->addChild(Label);

				editbox = static_cast<CEGUI::Editbox*>(winMgr.createWindow("TaharezLook/Editbox", EditBoxName[i * 3 + m]));
				editbox->setPosition(CEGUI::UVector2(cegui_reldim(0.05f ), cegui_reldim( 0.335f + i * 0.165f)));
				editbox->setSize(CEGUI::USize(cegui_reldim(0.25f), cegui_reldim(0.063f)));
				editbox->setText(EditBoxText[i * 3 + m]);
				editbox->setReadOnly(false);
				editbox->setValidationString("^(\\d+[.]?)?\\d*$");
				m_pSPHPanel->addChild(editbox);
			}
			


		}
		CEGUI::ButtonBase* button = static_cast<CEGUI::ButtonBase *>(winMgr.createWindow("TaharezLook/Button", "SaveChangeButton"));
		button->setPosition(CEGUI::UVector2(cegui_reldim(0.42f), cegui_reldim( 0.8f)));
		button->setSize(CEGUI::USize(cegui_reldim(0.4f), cegui_reldim(0.1f)));
		button->setText("Save Changes");
		m_pSPHPanel->addChild(button);
		button->subscribeEvent(CEGUI::ButtonBase::EventMouseClick,CEGUI::Event::Subscriber(SaveChangesButtonClicked));


		CEGUI::FrameWindow* fwndright = static_cast<CEGUI::FrameWindow*>(winMgr.createWindow("TaharezLook/FrameWindow", "MarchingCubeFrame"));
		fwndright->setPosition(CEGUI::UVector2(cegui_reldim(0.75f), cegui_reldim( 0.31f)));
		fwndright->setMaxSize(CEGUI::USize(cegui_reldim(1.0f), cegui_reldim( 1.0f)));
		fwndright->setSize(CEGUI::USize(cegui_reldim(0.25f), cegui_reldim( 0.3f)));
		fwndright->setText("Marching Cube Panel");
		m_root->addChild(fwndright);

		MarchingCubePanel = winMgr.createWindow("TaharezLook/StaticText", "MarchingCubePanel");
		MarchingCubePanel->setPosition(CEGUI::UVector2(cegui_reldim(0.02f), cegui_reldim(0.02f)));
		MarchingCubePanel->setSize(CEGUI::USize(cegui_reldim(0.97f), cegui_reldim( 0.83f)));
		MarchingCubePanel->setText("Marching Cube Control");
		MarchingCubePanel->setProperty("VertFormatting", "TopAligned");
		fwndright->addChild(MarchingCubePanel);

		for(int i = 0;i < 3; i++)
		{
			Label = winMgr.createWindow("TaharezLook/Label",LabelName[i + 12]);
			Label->setPosition(CEGUI::UVector2(cegui_reldim(0.05f + i * 0.34f), cegui_reldim( 0.15f)));
			Label->setSize(CEGUI::USize(cegui_reldim(0.4f), cegui_reldim(0.2f)));
			Label->setText(LabelText[i + 12 ]);
			Label->setProperty("HorzFormatting", "LeftAligned");
			MarchingCubePanel->addChild(Label);

			editbox = static_cast<CEGUI::Editbox*>(winMgr.createWindow("TaharezLook/Editbox", EditBoxName[i + 12]));
			editbox->setPosition(CEGUI::UVector2(cegui_reldim(0.05f + i * 0.34f), cegui_reldim( 0.4f)));
			editbox->setSize(CEGUI::USize(cegui_reldim(0.25f), cegui_reldim(0.2f)));
			editbox->setText(EditBoxText[i + 12]);//
			editbox->setReadOnly(false);
			editbox->setValidationString("^(\\d+[.]?)?\\d*$");
			MarchingCubePanel->addChild(editbox);
		}

		button = static_cast<CEGUI::ButtonBase *>(winMgr.createWindow("TaharezLook/Button", "MarchingCubeSaveButton"));
		button->setPosition(CEGUI::UVector2(cegui_reldim(0.55f), cegui_reldim( 0.7f)));
		button->setSize(CEGUI::USize(cegui_reldim(0.4f), cegui_reldim(0.2f)));
		button->setText("Save Changes");
		MarchingCubePanel->addChild(button);
		button->subscribeEvent(CEGUI::ButtonBase::EventMouseClick,CEGUI::Event::Subscriber(MarchingCubeSaveClicked));

		CEGUI::FrameWindow* fwndrightdown = static_cast<CEGUI::FrameWindow*>(winMgr.createWindow("TaharezLook/FrameWindow", "ControlFrame"));
		fwndrightdown->setPosition(CEGUI::UVector2(cegui_reldim(0.75), cegui_reldim( 0)));
		fwndrightdown->setMaxSize(CEGUI::USize(cegui_reldim(1.0f), cegui_reldim( 1.0f)));
		fwndrightdown->setSize(CEGUI::USize(cegui_reldim(0.25f), cegui_reldim( 0.3f)));
		fwndrightdown->setText("Main Control");
		m_root->addChild(fwndrightdown);

		ControlPanel = winMgr.createWindow("TaharezLook/StaticText", "ControlPanel");
		ControlPanel->setPosition(CEGUI::UVector2(cegui_reldim(0.02f), cegui_reldim(0.02f)));
		ControlPanel->setSize(CEGUI::USize(cegui_reldim(0.97f), cegui_reldim( 0.83f)));
		ControlPanel->setText("Control Panel");
		ControlPanel->setProperty("VertFormatting", "TopAligned");
		fwndrightdown->addChild(ControlPanel);

		button = static_cast<CEGUI::ButtonBase *>(winMgr.createWindow("TaharezLook/Button", "ResetButton"));
		button->setPosition(CEGUI::UVector2(cegui_reldim(0.05f), cegui_reldim( 0.25f)));
		button->setSize(CEGUI::USize(cegui_reldim(0.25f), cegui_reldim(0.15f)));
		button->setText("Reset");
		ControlPanel->addChild(button);
		button->subscribeEvent(CEGUI::ButtonBase::EventMouseClick,CEGUI::Event::Subscriber(ResetButtonClicked));

		button = static_cast<CEGUI::ButtonBase *>(winMgr.createWindow("TaharezLook/Button", "AddBarrierButton"));
		button->setPosition(CEGUI::UVector2(cegui_reldim(0.38f), cegui_reldim( 0.25f)));
		button->setSize(CEGUI::USize(cegui_reldim(0.25f), cegui_reldim(0.15f)));
		button->setText("+Barrier");
		ControlPanel->addChild(button);
		button->subscribeEvent(CEGUI::ButtonBase::EventMouseClick,CEGUI::Event::Subscriber(AddBarrier));

		button = static_cast<CEGUI::ButtonBase *>(winMgr.createWindow("TaharezLook/Button", "DeleteBarrierButton"));
		button->setPosition(CEGUI::UVector2(cegui_reldim(0.71f), cegui_reldim( 0.25f)));
		button->setSize(CEGUI::USize(cegui_reldim(0.25f), cegui_reldim(0.15f)));
		button->setText("-Barrier");
		ControlPanel->addChild(button);
		button->subscribeEvent(CEGUI::ButtonBase::EventMouseClick,CEGUI::Event::Subscriber(DeleteBarrier));

		button = static_cast<CEGUI::ButtonBase *>(winMgr.createWindow("TaharezLook/Button", "DefaultButton"));
		button->setPosition(CEGUI::UVector2(cegui_reldim(0.5f), cegui_reldim( 0.54f)));
		button->setSize(CEGUI::USize(cegui_reldim(0.4f), cegui_reldim(0.25f)));
		button->setText("Default Value");
		ControlPanel->addChild(button);
		button->subscribeEvent(CEGUI::ButtonBase::EventMouseClick,CEGUI::Event::Subscriber(DefaultValueButtonClicked));

		Label = winMgr.createWindow("TaharezLook/Label","FrameCount");
		Label->setPosition(CEGUI::UVector2(cegui_reldim(0.75f ), cegui_reldim( 0)));
		Label->setSize(CEGUI::USize(cegui_reldim(0.28f), cegui_reldim(0.2f)));
		Label->setText("0");
		Label->setProperty("HorzFormatting", "LeftAligned");
		ControlPanel->addChild(Label);

		Label = winMgr.createWindow("TaharezLook/Label","RenderModeLabel");
		Label->setPosition(CEGUI::UVector2(cegui_reldim(0.03f ), cegui_reldim(0.47f)));
		Label->setSize(CEGUI::USize(cegui_reldim(0.3f), cegui_reldim(0.08f)));
		Label->setText("Render Mode");
		Label->setProperty("HorzFormatting", "LeftAligned");
		ControlPanel->addChild(Label);

		//ComboBox
		CEGUI::Combobox* RenderModeCombobox = static_cast<CEGUI::Combobox*>(winMgr.createWindow("TaharezLook/Combobox", "RenderModeCombobox"));

		RenderModeCombobox->setPosition(CEGUI::UVector2(cegui_reldim(0.03f), cegui_reldim(0.6f)));
		RenderModeCombobox->setSize(CEGUI::USize(cegui_reldim(0.4f), cegui_reldim(0.4f)));
		RenderModeCombobox->setReadOnly(true);
		RenderModeCombobox->setSortingEnabled(false);

		RenderModeCombobox->subscribeEvent(CEGUI::Combobox::EventListSelectionAccepted, CEGUI::Event::Subscriber(handleRenderSelectionAccepted));

		ParticleMode = new CEGUI::ListboxTextItem("Particle");
		RenderModeCombobox->addItem(ParticleMode);

		BillBoardMode = new CEGUI::ListboxTextItem("BillBoard");
		RenderModeCombobox->addItem(BillBoardMode);

		MarchingCubeMode = new CEGUI::ListboxTextItem("MarchingCube");
		RenderModeCombobox->addItem(MarchingCubeMode);

		RenderMode = new CEGUI::ListboxTextItem("Render");
		RenderModeCombobox->addItem(RenderMode);

		RenderModeCombobox->setItemSelectState(BillBoardMode, true);

		ControlPanel->addChild(RenderModeCombobox);


}


void SPH_UIControl::Update(int framecount)
{
	CEGUI::Window * Label = (CEGUI::Window *)ControlPanel->getChild("FrameCount");
	CEGUI::String sframe = CEGUI::PropertyHelper<float>::toString((float)framecount);

	Label->setText("Frame: " +sframe);
}


void SPH_UIControl::Render()
{
	CEGUI::System::getSingleton().renderAllGUIContexts();
}

UINT SPH_UIControl::virtualkey2scancode(WPARAM wParam, LPARAM lParam)
{
	if(HIWORD(lParam) & 0x0F00)
	{
		UINT scancode = MapVirtualKey(wParam, 0); 
		return scancode | 0x80;
	}
	else
	{
		return HIWORD(lParam) & 0x00FF;
	}
}


bool SPH_UIControl::handleRenderSelectionAccepted(const CEGUI::EventArgs& args)
{
	//const CEGUI::WindowEventArgs& winEventArgs = static_cast<const CEGUI::WindowEventArgs&>(args);
	CEGUI::Combobox* RenderModeCombobox = (CEGUI::Combobox*)ControlPanel->getChild("RenderModeCombobox");

	CEGUI::ListboxItem* selectedItem = RenderModeCombobox->getSelectedItem();




	if(ParticleMode== selectedItem)
	{
		m_pSPHSystem->SPHRenderState(eParticle);
		m_pMarchingCube->IsRender(false);
	}

	if(BillBoardMode == selectedItem)
	{
		m_pSPHSystem->SPHRenderState(eBillPartilce);
		m_pMarchingCube->IsRender(false);
	}

	if(MarchingCubeMode == selectedItem)
	{
		m_pSPHSystem->SPHRenderState(eMarchingCube);
		m_pMarchingCube->IsRender(false);
	}

	if(RenderMode == selectedItem)
	{
		m_pSPHSystem->SPHRenderState(eRender);
		m_pMarchingCube->IsRender(true);
	}
	return false;
}

void SPH_UIControl::ReturnToDefault()
{
	m_pSPHSystem->SPH_Init();
	m_pSPHSystem->SPH_Reset();
	m_pSPHSystem->SPH_CreateParticlePool();
	m_pMarchingCube->Reset();
	VertexIndexControl::SPHWallInit(m_pd3dDevice,*m_pSPHSystem);


	m_pMarchingCube->SetDefaultValue();


	CEGUI::String EditBoxName[15] = {
		"Particle N_Editbox","WallLength_Editbox","UnitScale_Editbox",
		"Point_Mass_Editbox","SmoothRadius_Editbox","Viscosity_Editbox",
		"RestDensity_Editbox","GasConstantK_Editbox","Gravity_Editbox",
		"BoundaryDampening_Editbox","BoundaryCoe_Editbox","UpdateInterval_Editbox",
		"CubeSize_Editbox","CubeRange_Editbox","IsoValue_Editbox"

	};


	CEGUI::String EditBoxText[15];
	EditBoxText[0] = CEGUI::PropertyHelper<float>::toString((float)m_pSPHSystem->m_iParticleCount);
	EditBoxText[1] = CEGUI::PropertyHelper<float>::toString(m_pSPHSystem->WallLength());
	EditBoxText[2] = CEGUI::PropertyHelper<float>::toString(m_pSPHSystem->UnitScale());
	EditBoxText[3] = CEGUI::PropertyHelper<float>::toString(m_pSPHSystem->PointMass());
	EditBoxText[4] = CEGUI::PropertyHelper<float>::toString(m_pSPHSystem->SmoothRadius());
	EditBoxText[5] = CEGUI::PropertyHelper<float>::toString(m_pSPHSystem->Viscosity());
	EditBoxText[6] = CEGUI::PropertyHelper<float>::toString(m_pSPHSystem->RestDensity());
	EditBoxText[7] = CEGUI::PropertyHelper<float>::toString(m_pSPHSystem->GasConstantK());
	EditBoxText[8] = CEGUI::PropertyHelper<float>::toString(m_pSPHSystem->Gravity());
	EditBoxText[9] = CEGUI::PropertyHelper<float>::toString(m_pSPHSystem->BoundaryDampening());
	EditBoxText[10] = CEGUI::PropertyHelper<float>::toString(m_pSPHSystem->BoundaryCoefficient());
	EditBoxText[11] = CEGUI::PropertyHelper<float>::toString(m_pSPHSystem->Updateinteval());
	EditBoxText[12] = CEGUI::PropertyHelper<float>::toString(m_pMarchingCube->CubeLength());
	EditBoxText[13] = CEGUI::PropertyHelper<float>::toString(m_pMarchingCube->CubeRange());
	EditBoxText[14] = CEGUI::PropertyHelper<float>::toString(m_pMarchingCube->Thresholdvalue());





	CEGUI::Editbox* tempbox[15];

	for(int i = 0; i < 12; i++)
	{
		tempbox[i] = (CEGUI::Editbox*)m_pSPHPanel->getChild(EditBoxName[i]);
		tempbox[i]->setText(EditBoxText[i]);
	}
	for(int i = 12; i < 15; i++)
	{
		tempbox[i] = (CEGUI::Editbox*)MarchingCubePanel->getChild(EditBoxName[i]);
		tempbox[i]->setText(EditBoxText[i]);
	}
}


bool SPH_UIControl::DefaultValueButtonClicked(const CEGUI::EventArgs& args)
{

	ReturnToDefault();

	return false;

}

bool SPH_UIControl::ResetButtonClicked(const CEGUI::EventArgs& args)
{
	m_pSPHSystem->SPH_Reset();
	m_pSPHSystem->SPH_CreateParticlePool();
	m_pMarchingCube->Reset();
	return false;
}

bool SPH_UIControl::AddBarrier(const CEGUI::EventArgs& args)
{
	//MessageBoxA(0,"AddBarrier",0,0);
	//避免奇怪的状况发生所以自动重置
	m_pSPHSystem->AddBarrier();

	m_pSPHSystem->SPH_Reset();
	m_pSPHSystem->SPH_CreateParticlePool();
	m_pMarchingCube->Reset();

	return false;
}

bool SPH_UIControl::DeleteBarrier(const CEGUI::EventArgs& args)
{
	//MessageBoxA(0,"DeleteBarrier",0,0);
	m_pSPHSystem->DeleteBarrier();
	return false;
}

bool SPH_UIControl::SaveChangesButtonClicked(const CEGUI::EventArgs& args)
{


	CEGUI::Editbox* ParticleNumberBox = (CEGUI::Editbox*)m_pSPHPanel->getChild("Particle N_Editbox");
	int iPointN = CEGUI::PropertyHelper<int>::fromString(ParticleNumberBox->getText());

	if(m_pSPHSystem->ParticleMaxNumber() != iPointN)
	{
		if( iPointN - m_pSPHSystem->ParticleMaxNumber() > 10000 )
		{
			MessageBoxA(0,"Particles added each time should be fewer than 10000","Add Particle Error",0);
		}
		else
		{
			 m_pSPHSystem->ParticleMaxNumber(iPointN);
			 m_pSPHSystem->SPH_CreateParticlePool();
		}
	}

	CEGUI::String test_string;

	CEGUI::Editbox* WallLengthBox = (CEGUI::Editbox*)m_pSPHPanel->getChild("WallLength_Editbox");
	test_string = WallLengthBox->getText();
	if(SafeString == StringTest(test_string,"WallLength_Editbox"))
	{
		float fWallLength = CEGUI::PropertyHelper<float>::fromString(WallLengthBox->getText());

		if(fWallLength > 0 && fWallLength < 0.151)
			m_pSPHSystem->WallLength(fWallLength);
		else
		{
			MessageBoxA(0,"Wall Length should be : 0 < walllength <= 0.151","Wall Length Error",0);
		}
	}

	CEGUI::Editbox* PointMassBox = (CEGUI::Editbox*)m_pSPHPanel->getChild("Point_Mass_Editbox");
	test_string = PointMassBox->getText();
	if(SafeString == StringTest(test_string,"Point_Mass_Editbox"))
	{
		float fpointmass = CEGUI::PropertyHelper<float>::fromString(PointMassBox->getText());

		if(fpointmass > 0)
			m_pSPHSystem->PointMass(fpointmass);
		else
		{
			MessageBoxA(0,"Point Mass should be : Point Mass > 0","Point Mass Error",0);
		}
	}



	CEGUI::Editbox* GravityBox = (CEGUI::Editbox*)m_pSPHPanel->getChild("Gravity_Editbox");
	test_string = GravityBox->getText();
	if(SafeString == StringTest(test_string,"Gravity_Editbox"))
	{
		float fGravity = CEGUI::PropertyHelper<float>::fromString(GravityBox->getText());
		m_pSPHSystem->Gravity(fGravity);
	}



	CEGUI::Editbox* UnitScaleBox = (CEGUI::Editbox*)m_pSPHPanel->getChild("UnitScale_Editbox");
	test_string = UnitScaleBox->getText();
	if(SafeString == StringTest(test_string,"UnitScale_Editbox"))
	{
		float fUnitScale = CEGUI::PropertyHelper<float>::fromString(UnitScaleBox->getText());

		if(fUnitScale > 0)
			m_pSPHSystem->UnitScale(fUnitScale);
		else
		{
			MessageBoxA(0,"UnitScale  should be : UnitScale > 0","UnitScale Error",0);
		}
		
	}



	CEGUI::Editbox* SmoothRadiusBox = (CEGUI::Editbox*)m_pSPHPanel->getChild("SmoothRadius_Editbox");
	test_string = SmoothRadiusBox->getText();
	if(SafeString == StringTest(test_string,"SmoothRadius_Editbox"))
	{
		float fSmoothRadius = CEGUI::PropertyHelper<float>::fromString(SmoothRadiusBox->getText());


		if(fSmoothRadius > 0)
			m_pSPHSystem->SmoothRadius(fSmoothRadius);
		else
		{
			MessageBoxA(0,"SmoothRadius  should be : SmoothRadius > 0","SmoothRadius Error",0);
		}

		
	}


	CEGUI::Editbox* Viscosity_Editbox = (CEGUI::Editbox*)m_pSPHPanel->getChild("Viscosity_Editbox");
	test_string = Viscosity_Editbox->getText();
	if(SafeString == StringTest(test_string,"Viscosity_Editbox"))
	{
		float fViscosity = CEGUI::PropertyHelper<float>::fromString(Viscosity_Editbox->getText());
		m_pSPHSystem->Viscosity(fViscosity);
	}



	CEGUI::Editbox* RestDensity_Editbox = (CEGUI::Editbox*)m_pSPHPanel->getChild("RestDensity_Editbox");
	test_string = RestDensity_Editbox->getText();
	if(SafeString == StringTest(test_string,"RestDensity_Editbox"))
	{
		float fRestDensity = CEGUI::PropertyHelper<float>::fromString(RestDensity_Editbox->getText());
		m_pSPHSystem->RestDensity(fRestDensity);
	}



	CEGUI::Editbox* GasConstantK_Editbox = (CEGUI::Editbox*)m_pSPHPanel->getChild("GasConstantK_Editbox");
	test_string = GasConstantK_Editbox->getText();
	if(SafeString == StringTest(test_string,"GasConstantK_Editbox"))
	{
		float fGasConstantK = CEGUI::PropertyHelper<float>::fromString(GasConstantK_Editbox->getText());
		m_pSPHSystem->GasConstantK(fGasConstantK);
	}



	CEGUI::Editbox* BoundaryDampening_Editbox = (CEGUI::Editbox*)m_pSPHPanel->getChild("BoundaryDampening_Editbox");
	test_string = BoundaryDampening_Editbox->getText();
	if(SafeString == StringTest(test_string,"BoundaryDampening_Editbox"))
	{
		float fBoundaryDampening = CEGUI::PropertyHelper<float>::fromString(BoundaryDampening_Editbox->getText());
		m_pSPHSystem->BoundaryDampening(fBoundaryDampening);
	}



	CEGUI::Editbox* BoundaryCoe_Editbox = (CEGUI::Editbox*)m_pSPHPanel->getChild("BoundaryCoe_Editbox");
	test_string = BoundaryCoe_Editbox->getText();
	if(SafeString == StringTest(test_string,"BoundaryCoe_Editbox"))
	{
		float fBoundaryCoe = CEGUI::PropertyHelper<float>::fromString(BoundaryCoe_Editbox->getText());
		m_pSPHSystem->BoundaryCoefficient(fBoundaryCoe);
	}



	CEGUI::Editbox* UpdateInterval_Editbox = (CEGUI::Editbox*)m_pSPHPanel->getChild("UpdateInterval_Editbox");
	test_string = UpdateInterval_Editbox->getText();
	if(SafeString == StringTest(test_string,"UpdateInterval_Editbox"))
	{
		float fUpdateInterval = CEGUI::PropertyHelper<float>::fromString(UpdateInterval_Editbox->getText());

		if(fUpdateInterval > 0)
			m_pSPHSystem->Updateinteval(fUpdateInterval);
		else
		{
			MessageBoxA(0,"UpdateInterval  should be : UpdateInterval > 0","UpdateInterval Error",0);
		}

		
	}

	VertexIndexControl::SPHWallInit(m_pd3dDevice,*m_pSPHSystem);



	return false;
}

bool SPH_UIControl::MarchingCubeSaveClicked(const CEGUI::EventArgs& args)
{

	CEGUI::String test_string;

	CEGUI::Editbox* CubeSize_Editbox = (CEGUI::Editbox*)MarchingCubePanel->getChild("CubeSize_Editbox");
	test_string = CubeSize_Editbox->getText();
	if(SafeString == StringTest(test_string,"CubeSize_Editbox"))
	{
		float fCubeSize = CEGUI::PropertyHelper<float>::fromString(CubeSize_Editbox->getText());

		if(fCubeSize > 0)
		m_pMarchingCube->CubeLength(fCubeSize);
		else 
		{
			MessageBoxA(0,"Cube Size cannot be 0","Cube Size Error",0);
		}
	}



	CEGUI::Editbox* CubeRange_Editbox = (CEGUI::Editbox*)MarchingCubePanel->getChild("CubeRange_Editbox");
	test_string = CubeRange_Editbox->getText();
	if(SafeString == StringTest(test_string,"CubeRange_Editbox"))
	{

		float fCubeRange = CEGUI::PropertyHelper<float>::fromString(CubeRange_Editbox->getText());

		if(fCubeRange < 0.156)
		m_pMarchingCube->CubeRange(fCubeRange);
		else
		{
			MessageBoxA(0,"CubeRange Exceeds the Maxinum: 0.155","Cube Range Error",0);
		}
	}



	CEGUI::Editbox* IsoValue_Editbox = (CEGUI::Editbox*)MarchingCubePanel->getChild("IsoValue_Editbox");
	test_string = IsoValue_Editbox->getText();
	if(SafeString == StringTest(test_string,"IsoValue_Editbox"))
	{
		float fIsoValue = CEGUI::PropertyHelper<float>::fromString(IsoValue_Editbox->getText());
		m_pMarchingCube->Thresholdvalue(fIsoValue);
	}



	return false;
}

StringError SPH_UIControl::StringTest(CEGUI::String str,char * boxname)
{
	char emptyerror[100] = "Input cannot be empty : ";
	char cstarterror[100] = "Input cannot begin with these characters : ";
	char enderror[100] = "Input cannot end with these characters : ";


	if(str.empty())
	{
		strcat_s(emptyerror,boxname);
		MessageBoxA(0,emptyerror,"Empty Error",0);
		return blankstring;
	}

	if('.' == str[str.size() - 1] || '-' == str[str.size() - 1])
	{
		strcat_s(enderror,boxname);
		MessageBoxA(0,enderror,"Input Error",0);
		return endwithoperator;
	}

	if(str.size() > 1 && '0' == str[0] && '.' != str[1])
	{
		strcat_s(cstarterror,boxname);
		MessageBoxA(0,cstarterror,"Input Error",0);
		return starterror;
	}

	if('-' == str[0] && '0' == str[1] && '.' != str[2])
	{
		strcat_s(cstarterror,boxname);
		MessageBoxA(0,cstarterror,"Input Error",0);
		return starterror;
	}
	return SafeString;

}

void SPH_UIControl::setRenderModeBoxState(RenderState val)
{		
	CEGUI::Combobox* RenderModeCombobox = (CEGUI::Combobox*)ControlPanel->getChild("RenderModeCombobox");

	if(eParticle == val)
	{

		RenderModeCombobox->setItemSelectState(ParticleMode,true);

	}

	if(eBillPartilce == val)
	{
		RenderModeCombobox->setItemSelectState(BillBoardMode,true);
	}

	if(eMarchingCube == val)
	{
		RenderModeCombobox->setItemSelectState(MarchingCubeMode,true);
	}

	if(eRender == val)
	{
		RenderModeCombobox->setItemSelectState(RenderMode,true);
	}
}

















