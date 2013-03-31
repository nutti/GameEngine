#include "SpriteBatch.h"

#include <MAPIL/MAPIL.h>

#include <vector>
#include <map>

namespace GameEngine
{
	class SpriteBatch
	{
	private:
		struct Work
		{
			int		m_TexID;
			float	m_PosX;
			float	m_PosY;
			float	m_Angle;
			bool	m_Centerized;
			int		m_Color;
		};
		struct WorkScale
		{
			int		m_TexID;
			float	m_PosX;
			float	m_PosY;
			float	m_ScaleX;
			float	m_ScaleY;
			float	m_Angle;
			bool	m_Centerized;
			int		m_Color;
		};
		typedef std::map < int, std::vector < Work > >	WorkList;
		typedef std::map < int, std::vector < WorkScale > > WorkScaleList;
		WorkList m_WorkList;
		WorkScaleList m_WorkScaleList;
	public:
		SpriteBatch();
		~SpriteBatch();
		void Add(	int blendingMode, int texID, float posX, float posY, float angle,
					bool centerized = true, int color = 0xFFFFFFFF );
		void Add(	int blendingMode, int texID, float posX, float posY, float scaleX, float scaleY, float angle,
					bool centerized = true, int color = 0xFFFFFFFF );
		void ProcAllWorks();
		void Cleanup();
	};

	SpriteBatch::SpriteBatch()
	{
		Cleanup();
	}
	
	SpriteBatch::~SpriteBatch()
	{
		Cleanup();
	}

	void SpriteBatch::Add(	int blendingMode, int texID, float posX, float posY, float angle,
							bool centerized, int color )
	{
		SpriteBatch::Work work;
		work.m_TexID = texID;
		work.m_PosX = posX;
		work.m_PosY = posY;
		work.m_Angle = angle;
		work.m_Centerized = centerized;
		work.m_Color = color;

		m_WorkList[ blendingMode ].push_back( work );
	}

	void SpriteBatch::Add(	int blendingMode, int texID, float posX, float posY, float scaleX, float scaleY, float angle,
							bool centerized, int color )
	{
		SpriteBatch::WorkScale work;
		work.m_TexID = texID;
		work.m_PosX = posX;
		work.m_PosY = posY;
		work.m_ScaleX = scaleX;
		work.m_ScaleY = scaleY;
		work.m_Angle = angle;
		work.m_Centerized = centerized;
		work.m_Color = color;

		m_WorkScaleList[ blendingMode ].push_back( work );
	}

	void SpriteBatch::ProcAllWorks()
	{
		typedef WorkList::iterator workIt;
		typedef WorkScaleList::iterator workScaleIt;

		for( workIt it = m_WorkList.begin(); it != m_WorkList.end(); ++it ){
			const std::vector < SpriteBatch::Work >& works = it->second;
			MAPIL::Set2DAlphaBlendingMode( it->first );
			for( int i = 0; i < works.size(); ++i ){
				const SpriteBatch::Work& workItem = works[ i ];
				MAPIL::DrawTexture(	workItem.m_TexID,
									workItem.m_PosX, workItem.m_PosY, workItem.m_Angle,
									workItem.m_Centerized, workItem.m_Color );
			}
		}

		for( workScaleIt it = m_WorkScaleList.begin(); it != m_WorkScaleList.end(); ++it ){
			const std::vector < SpriteBatch::WorkScale >& works = it->second;
			MAPIL::Set2DAlphaBlendingMode( it->first );
			for( int i = 0; i < works.size(); ++i ){
				const SpriteBatch::WorkScale& workItem = works[ i ];
				MAPIL::DrawTexture(	workItem.m_TexID,
									workItem.m_PosX, workItem.m_PosY, workItem.m_ScaleX, workItem.m_ScaleY, workItem.m_Angle,
									workItem.m_Centerized, workItem.m_Color );
			}
		}

		Cleanup();
		MAPIL::Set2DAlphaBlendingMode( MAPIL::ALPHA_BLEND_MODE_SEMI_TRANSPARENT );
	}

	void SpriteBatch::Cleanup()
	{
		m_WorkList.clear();
		m_WorkScaleList.clear();
	}

	static SpriteBatch g_Batch;

	void AddToSpriteBatch(	int blendingMode, int texID, float posX, float posY, float angle,
							bool centerized, int color )
	{
		g_Batch.Add( blendingMode, texID, posX, posY, angle, centerized, color );
	}

	void AddToSpriteBatch(	int blendingMode, int texID, float posX, float posY, float scaleX, float scaleY, float angle,
							bool centerized, int color )
	{
		g_Batch.Add( blendingMode, texID, posX, posY, scaleX, scaleY, angle, centerized, color );
	}

	void ProcAllBatchWorks()
	{
		g_Batch.ProcAllWorks();
	}
}