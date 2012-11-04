#include "StageBackgroundVCPU.h"

#include <MAPIL/MAPIL.h>

#include "StageBackground.h"
#include "Stage.h"


namespace GameEngine
{
	StageBackgroundVCPU::StageBackgroundVCPU() : VM::VCPU()
	{
	}

	StageBackgroundVCPU::~StageBackgroundVCPU()
	{
	}

	void StageBackgroundVCPU::SysStageBackgroundGetCounter()
	{
		Pop();
		Push( m_pStageBGData->m_Counter );
	}

	void StageBackgroundVCPU::SysEnableCamera()
	{
		Pop();
		MAPIL::EnableCamera();
	}

	void StageBackgroundVCPU::SysSetCameraProj()
	{
		Pop();
		float farClip = Top().m_Float;
		Pop();
		float nearClip = Top().m_Float;
		Pop();
		float aspect = Top().m_Float;
		Pop();
		float fovy = Top().m_Float;
		Pop();

		MAPIL::SetCameraProjTrans( fovy, aspect, nearClip, farClip );
	}

	void StageBackgroundVCPU::SysSetCameraView()
	{
		Pop();
		float uz = Top().m_Float;
		Pop();
		float uy = Top().m_Float;
		Pop();
		float ux = Top().m_Float;
		Pop();
		float lz = Top().m_Float;
		Pop();
		float ly = Top().m_Float;
		Pop();
		float lx = Top().m_Float;
		Pop();
		float ez = Top().m_Float;
		Pop();
		float ey = Top().m_Float;
		Pop();
		float ex = Top().m_Float;
		Pop();

		MAPIL::SetCameraViewTrans( ex, ey, ez, lx, ly, lz, ux, uy, uz );
	}

	void StageBackgroundVCPU::SysDrawRect3DP()
	{
		Pop();

		MAPIL::Polygon3DVertexFormat fmt[ 4 ];
		for( int i = 3; i >= 0; --i ){
			fmt[ i ].m_Pos.m_Z = Top().m_Float;
			Pop();
			fmt[ i ].m_Pos.m_Y = Top().m_Float;
			Pop();
			fmt[ i ].m_Pos.m_X = Top().m_Float;
			Pop();
			fmt[ i ].m_Color = 0xFFFFFFFF;
		}
		fmt[ 0 ].m_Tex.m_U = 0.0f;
		fmt[ 0 ].m_Tex.m_V = 0.0f;
		fmt[ 1 ].m_Tex.m_U = 0.0f;
		fmt[ 1 ].m_Tex.m_V = 1.0f;
		fmt[ 2 ].m_Tex.m_U = 1.0f;
		fmt[ 2 ].m_Tex.m_V = 0.0f;
		fmt[ 3 ].m_Tex.m_U = 1.0f;
		fmt[ 3 ].m_Tex.m_V = 1.0f;

		int texID = Top().m_Integer;
		Pop();

		MAPIL::DrawPolygon3D(	fmt, 2,
								m_pStageBGData->m_pStageData->m_ResourceMap.m_pStageResourceMap->m_TextureMap[ texID ] );
	}

	void StageBackgroundVCPU::SysBegin3DGraphics()
	{
		Pop();
		MAPIL::BeginRendering3DGraphics();
	}

	void StageBackgroundVCPU::SysEnd3DGraphics()
	{
		Pop();
		MAPIL::EndRendering3DGraphics();
	}

	void StageBackgroundVCPU::SysEnableFog()
	{
		Pop();
		MAPIL::EnableFog();
	}

	void StageBackgroundVCPU::Init( VM::Data* pData, StageBackgroundData* pStageBGData )
	{
		m_pData = pData;
		m_pStageBGData = pStageBGData;
		VM::VCPU::Init();
	}

	void StageBackgroundVCPU::OpSysCall( int val )
	{
		switch( val ){
			case VM::SYS_STAGE_BACKGROUND_GET_COUNTER:
				SysStageBackgroundGetCounter();
				break;

			case VM::SYS_ENABLE_CAMERA:			// Enable camera.
				SysEnableCamera();
				break;
			case VM::SYS_SET_CAMERA_PROJ:		// Set camera projection matrix.
				SysSetCameraProj();
				break;
			case VM::SYS_SET_CAMERA_VIEW:		// Set camera view matrix.
				SysSetCameraView();
				break;
			case VM::SYS_DRAW_RECTANGLE_3D_FIXED_COLOR_TEXCOORD:		// Draw polygon 3D. (Fixed color and texture coordinate.)
				SysDrawRect3DP();
				break;
			case VM::SYS_BEGIN_3D_GRAPHICS:		// Begin 3D Graphics.
				SysBegin3DGraphics();
				break;
			case VM::SYS_END_3D_GRAPHICS:		// End 3D Graphics.
				SysEnd3DGraphics();
				break;
			case VM::SYS_ENABLE_FOG:			// Enable fog effect.
				SysEnableFog();
				break;

			default:
				VM::VCPU::OpSysCall( val );
				break;
		}
	}
}