#include "StageBackgroundVCPU.h"

#include <MAPIL/MAPIL.h>

#include "StageBackground.h"
#include "Stage.h"

#include "SpriteBatch.h"


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
		Push( m_pStageBGData->m_pStageData->m_Frame );
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

	void StageBackgroundVCPU::SysSetFogParam()
	{
		Pop();
		float density = Top().m_Float;
		Pop();
		float end = Top().m_Float;
		Pop();
		float begin = Top().m_Float;
		Pop();
		int mode = Top().m_Integer;
		Pop();
		int colorB = Top().m_Integer;
		Pop();
		int colorG = Top().m_Integer;
		Pop();
		int colorR = Top().m_Integer;
		Pop();
		int colorA = Top().m_Integer;
		Pop();

		MAPIL::SetFogParam(	MAPIL::GetColorARGBValue( static_cast < unsigned char > ( colorA & 0xFF ), colorR & 0xFF, colorG & 0xFF, colorB & 0xFF ),
							mode, begin, end, density );
	}

	void StageBackgroundVCPU::SysDrawModelP()
	{
		Pop();
		float z = Top().m_Float;
		Pop();
		float y = Top().m_Float;
		Pop();
		float x = Top().m_Float;
		Pop();
		int id = Top().m_Integer;
		Pop();

		MAPIL::Matrix4x4 < float >  mat;
		MAPIL::CreateTranslationMat( &mat, x, y, z );
		MAPIL::DrawModel( m_pStageBGData->m_pStageData->m_ResourceMap.m_pStageResourceMap->m_ModelMap[ id ], mat );
	}

	void StageBackgroundVCPU::SysDrawModelPS()
	{
		Pop();
		float sz = Top().m_Float;
		Pop();
		float sy = Top().m_Float;
		Pop();
		float sx = Top().m_Float;
		Pop();
		float z = Top().m_Float;
		Pop();
		float y = Top().m_Float;
		Pop();
		float x = Top().m_Float;
		Pop();
		int id = Top().m_Integer;
		Pop();

		MAPIL::Matrix4x4 < float > mat;
		MAPIL::Matrix4x4 < float > scaleMat;
		MAPIL::CreateScalingMat( &scaleMat, sx, sy, sz );
		MAPIL::CreateTranslationMat( &mat, x, y, z );
		mat = scaleMat * mat;
		MAPIL::DrawModel( m_pStageBGData->m_pStageData->m_ResourceMap.m_pStageResourceMap->m_ModelMap[ id ], mat );
	}

	void StageBackgroundVCPU::SysSetDirLightDir()
	{
		Pop();
		float z = Top().m_Float;
		Pop();
		float y = Top().m_Float;
		Pop();
		float x = Top().m_Float;
		Pop();
		int id = Top().m_Integer;
		Pop();

		MAPIL::SetDirLightDirection( id, x, y, z );
	}

	void StageBackgroundVCPU::SysSetDirLightAmbColor()
	{
		Pop();
		int b = Top().m_Integer;
		Pop();
		int g = Top().m_Integer;
		Pop();
		int r = Top().m_Integer;
		Pop();
		int a = Top().m_Integer;
		Pop();
		int id = Top().m_Integer;
		Pop();

		MAPIL::SetDirLightAmbColor( id, ( a & 0xFF ) << 24 | ( r & 0xFF ) << 16 |
										( g & 0xFF ) << 8 | ( b & 0xFF ) );
	}

	void StageBackgroundVCPU::SysSetDirLightDifColor()
	{
		Pop();
		int b = Top().m_Integer;
		Pop();
		int g = Top().m_Integer;
		Pop();
		int r = Top().m_Integer;
		Pop();
		int a = Top().m_Integer;
		Pop();
		int id = Top().m_Integer;
		Pop();

		MAPIL::SetDirLightDifColor( id, ( a & 0xFF ) << 24 | ( r & 0xFF ) << 16 |
										( g & 0xFF ) << 8 | ( b & 0xFF ) );
	}

	void StageBackgroundVCPU::SysSetDirLightSpcColor()
	{
		Pop();
		int b = Top().m_Integer;
		Pop();
		int g = Top().m_Integer;
		Pop();
		int r = Top().m_Integer;
		Pop();
		int a = Top().m_Integer;
		Pop();
		int id = Top().m_Integer;
		Pop();

		MAPIL::SetDirLightSpcColor( id, ( a & 0xFF ) << 24 | ( r & 0xFF ) << 16 |
										( g & 0xFF ) << 8 | ( b & 0xFF ) );
	}

	void StageBackgroundVCPU::SysSetDirLightAtten()
	{
		Pop();
		float atten2 = Top().m_Float;
		Pop();
		float atten1 = Top().m_Float;
		Pop();
		float atten0 = Top().m_Float;
		Pop();
		int id = Top().m_Integer;
		Pop();

		MAPIL::SetDirLightAttenuation( id, atten0, atten1, atten2 );
	}

	void StageBackgroundVCPU::SysEnableDirLight()
	{
		Pop();
		int id = Top().m_Integer;
		Pop();

		MAPIL::EnableDirLight( id );
	}

	void StageBackgroundVCPU::SysStageGetFrame()
	{
		Pop();
		Push( m_pStageBGData->m_pStageData->m_Frame );
	}

	void StageBackgroundVCPU::Init( VM::Data* pData, StageBackgroundData* pStageBGData )
	{
		m_pData = pData;
		m_pStageBGData = pStageBGData;
		VM::VCPU::Init();
	}

	void StageBackgroundVCPU::SysEnableAlphaBlending()
	{
		Pop();
		MAPIL::EnableBlending();
	}

	void StageBackgroundVCPU::SysDisableAlphaBlending()
	{
		Pop();
		MAPIL::DisableBlending();
	}

	void StageBackgroundVCPU::SysBegin2DGraphics()
	{
		Pop();
		MAPIL::BeginRendering2DGraphics();
	}

	void StageBackgroundVCPU::SysEnd2DGraphics()
	{
		Pop();
		MAPIL::EndRendering2DGraphics();
	}

	void StageBackgroundVCPU::SysDrawTexturePS()
	{
		Pop();
		int color = Top().m_Integer;
		Pop();
		bool centerize = Top().m_Integer == 0 ? false : true;
		Pop();
		float sy = Top().m_Float;
		Pop();
		float sx = Top().m_Float;
		Pop();
		float y = Top().m_Float;
		Pop();
		float x = Top().m_Float;
		Pop();
		int index = Top().m_Integer;
		Pop();

		AddToSpriteBatch(	MAPIL::ALPHA_BLEND_MODE_SEMI_TRANSPARENT,
							m_pStageBGData->m_pStageData->m_ResourceMap.m_pStageResourceMap->m_TextureMap[ index ],
							x, y , sx, sy, 0.0f, centerize, color );
		
		//MAPIL::DrawTexture( , x, y, sx, sy, centerize, color );
	}

	void StageBackgroundVCPU::SysColorARGB()
	{
		Pop();
		int b = Top().m_Integer;
		Pop();
		int g = Top().m_Integer;
		Pop();
		int r = Top().m_Integer;
		Pop();
		int a = Top().m_Integer;
		Pop();

		Push( a << 24 | r << 16 | g << 8 | b );
	}

	void StageBackgroundVCPU::SysStageGetBossFlag()
	{
		Pop();
		int flag = m_pStageBGData->m_pStageData->m_BossMode ? 1 : 0;
		Push( flag );
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
			case VM::SYS_SET_FOG_PARAM:
				SysSetFogParam();
				break;
			case VM::SYS_DRAW_MODEL_FIXED_SCALE_ROT:
				SysDrawModelP();
				break;
			case VM::SYS_SET_DIRLIGHT_DIR:
				SysSetDirLightDir();
				break;
			case VM::SYS_SET_DIRLIGHT_DIF_COLOR:
				SysSetDirLightDifColor();
				break;
			case VM::SYS_SET_DIRLIGHT_AMB_COLOR:
				SysSetDirLightAmbColor();
				break;
			case VM::SYS_SET_DIRLIGHT_SPC_COLOR:
				SysSetDirLightSpcColor();
				break;
			case VM::SYS_SET_DIRLIGHT_ATTEN:
				SysSetDirLightAtten();
				break;
			case VM::SYS_ENABLE_DIRLIGHT:
				SysEnableDirLight();
				break;
			case VM::SYS_ENABLE_ALPHA_BLENDING:
				SysEnableAlphaBlending();
				break;
			case VM::SYS_DISABLE_ALPHA_BLENDING:
				SysDisableAlphaBlending();
				break;
			case VM::SYS_DRAW_MODEL_FIXED_ROT:
				SysDrawModelPS();
				break;
			case VM::SYS_BEGIN_2D_GRAPHICS:
				SysBegin2DGraphics();
				break;
			case VM::SYS_END_2D_GRAPHICS:
				SysEnd2DGraphics();
				break;
			case VM::SYS_DRAW_TEXTURE_FIXED_ROT:
				SysDrawTexturePS();
				break;
			case VM::SYS_COLOR_ARGB:
				SysColorARGB();
				break;

			case VM::SYS_STAGE_GET_BOSS_FLAG:
				SysStageGetBossFlag();
				break;

			case VM::SYS_STAGE_GET_FRAME:
				SysStageGetFrame();
				break;

			default:
				VM::VCPU::OpSysCall( val );
				break;
		}
	}
}