#ifndef SPH_UI_H_
#define SPH_UI_H_

#include "SPH_Math.h"
#include "SPH_System.h"
#include "SPH_MarchingCube.h"
#include <CEGUI/RendererModules/Direct3D11/Renderer.h>
#include <CEGUI/CEGUI.h>
#include <CEGUI/PropertyHelper.h>

class SPHSystem; //why
class MarchingCube;
class CubeCell;
enum RenderState;
enum StringError
{
	starterror,
	endwithoperator,
	blankstring,
	SafeString,

};
class SPH_UIControl
{
private:
    static CEGUI::DefaultWindow * m_root;
	static SPHSystem* m_pSPHSystem;
	static MarchingCube* m_pMarchingCube;
	static CEGUI::Window* m_pSPHPanel;
	static CEGUI::Window* MarchingCubePanel;
	static CEGUI::Window* ControlPanel;
	static ID3D11Device * m_pd3dDevice;
	static StringError StringTest(CEGUI::String str,char * boxname);

	static CEGUI::ListboxItem* ParticleMode;
	static CEGUI::ListboxItem* BillBoardMode;
	static CEGUI::ListboxItem* MarchingCubeMode;
	static CEGUI::ListboxItem* RenderMode;


public:
	static void Init(ID3D11Device * pd3dDevice,ID3D11DeviceContext * m_pd3dDevContext,SPHSystem & sph_system,MarchingCube & sph_marchingcube);
	static bool DefaultValueButtonClicked(const CEGUI::EventArgs& args);
	static void ReturnToDefault();
	static bool SaveChangesButtonClicked(const CEGUI::EventArgs& args);
	static bool MarchingCubeSaveClicked(const CEGUI::EventArgs& args);
	static bool ResetButtonClicked(const CEGUI::EventArgs& args);
	static bool handleRenderSelectionAccepted(const CEGUI::EventArgs& args);

	static void setRenderModeBoxState(RenderState val);

	static bool AddBarrier(const CEGUI::EventArgs& args);
	static bool DeleteBarrier(const CEGUI::EventArgs& args);
	static void Update(int framecount);
	static void Render();
	static UINT virtualkey2scancode(WPARAM wParam, LPARAM lParam);
};


#endif 