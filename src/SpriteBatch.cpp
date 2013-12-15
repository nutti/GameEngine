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
			bool	m_Global;
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
			bool	m_Global;
		};
		struct WorkCliped
		{
			int		m_TexID;
			float	m_PosX;
			float	m_PosY;
			float	m_ScaleX;
			float	m_ScaleY;
			float	m_Angle;
			bool	m_Centerized;
			int		m_Color;
			float	m_ClipedX1;
			float	m_ClipedY1;
			float	m_ClipedX2;
			float	m_ClipedY2;
			bool	m_Global;
		};

		typedef std::map < int, std::vector < Work > >	WorkList;
		typedef std::map < int, std::vector < WorkScale > > WorkScaleList;
		typedef std::map < int, std::vector < Work > >	AtlasWorkList;
		typedef std::map < int, std::vector < WorkScale > > AtlasWorkScaleList;
		typedef std::map < int, std::vector < WorkCliped > > AtlasWorkClipedList;

		WorkList m_WorkList;
		WorkScaleList m_WorkScaleList;
		AtlasWorkList m_AtlasWorkList;
		AtlasWorkScaleList m_AtlasWorkScaleList;
		AtlasWorkClipedList m_AtlasWorkClipedList;
	public:
		SpriteBatch();
		~SpriteBatch();
		void Add(	int blendingMode, int texID, float posX, float posY, float angle,
					bool centerized = true, int color = 0xFFFFFFFF );
		void Add(	int blendingMode, int texID, float posX, float posY, float scaleX, float scaleY, float angle,
					bool centerized = true, int color = 0xFFFFFFFF );
		void AddAtlas(	bool global, int blendingMode, int atlasID, float posX, float posY, float angle,
						bool centerized = true, int color = 0xFFFFFFFF );
		void AddAtlas(	bool global, int blendingMode, int atlasID, float posX, float posY, float scaleX, float scaleY, float angle,
						bool centerized = true, int color = 0xFFFFFFFF );
		void AddAtlas(	bool global, int blendingMode, int atlasID,
						float cx1, float cy1, float cx2, float cy2,
						float posX, float posY, float scaleX, float scaleY, float angle,
						bool centerized = true, int color = 0xFFFFFFFF );
		void ProcAllWorks( const ResourceMap& map );
		void Cleanup();
		void AtlasCleanup();
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

	void SpriteBatch::AddAtlas(	bool global, int blendingMode, int atlasID, float posX, float posY, float angle,
								bool centerized, int color )
	{
		SpriteBatch::Work work;
		work.m_TexID = atlasID;
		work.m_PosX = posX;
		work.m_PosY = posY;
		work.m_Angle = angle;
		work.m_Centerized = centerized;
		work.m_Color = color;
		work.m_Global = global;

		m_AtlasWorkList[ blendingMode ].push_back( work );
	}

	void SpriteBatch::AddAtlas(	bool global, int blendingMode, int atlasID, float posX, float posY, float scaleX, float scaleY, float angle,
								bool centerized, int color )
	{
		SpriteBatch::WorkScale work;
		work.m_TexID = atlasID;
		work.m_PosX = posX;
		work.m_PosY = posY;
		work.m_ScaleX = scaleX;
		work.m_ScaleY = scaleY;
		work.m_Angle = angle;
		work.m_Centerized = centerized;
		work.m_Color = color;
		work.m_Global = global;

		m_AtlasWorkScaleList[ blendingMode ].push_back( work );
	}

	void SpriteBatch::AddAtlas(	bool global, int blendingMode, int atlasID,
								float cx1, float cy1, float cx2, float cy2,
								float posX, float posY, float scaleX, float scaleY, float angle,
								bool centerized, int color )
	{
		SpriteBatch::WorkCliped work;
		work.m_TexID = atlasID;
		work.m_PosX = posX;
		work.m_PosY = posY;
		work.m_ScaleX = scaleX;
		work.m_ScaleY = scaleY;
		work.m_Angle = angle;
		work.m_Centerized = centerized;
		work.m_Color = color;
		work.m_ClipedX1 = cx1;
		work.m_ClipedY1 = cy1;
		work.m_ClipedX2 = cx2;
		work.m_ClipedY2 = cy2;
		work.m_Global = global;
		
		m_AtlasWorkClipedList[ blendingMode ].push_back( work );
	}

	void SpriteBatch::ProcAllWorks( const ResourceMap& map )
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

		std::for_each(	m_AtlasWorkList.begin(), m_AtlasWorkList.end(),
						[this,map]( const std::pair < int, std::vector < SpriteBatch::Work > >& pair ){
			const std::vector < SpriteBatch::Work >& works = pair.second;
			MAPIL::Set2DAlphaBlendingMode( pair.first );
			for( int i = 0; i < works.size(); ++i ){
				const SpriteBatch::Work& w = works[ i ];
				ResourceMap::TextureAtlas atlas;
				if( w.m_Global ){
					atlas = map.m_pGlobalResourceMap->m_TexAtlasMap[ w.m_TexID ];
				}
				else{
					atlas = map.m_pStageResourceMap->m_TexAtlasMap[ w.m_TexID ];
				}
				MAPIL::DrawClipedTexture(	map.m_pStageResourceMap->m_TextureMap[ atlas.m_TexID ],
											w.m_PosX, w.m_PosY, 1.0f, 1.0f, w.m_Angle,
											atlas.m_X, atlas.m_Y,
											atlas.m_X + atlas.m_Width, atlas.m_Y + atlas.m_Height,
											w.m_Centerized, w.m_Color );
			}
		} );

		std::for_each(	m_AtlasWorkScaleList.begin(), m_AtlasWorkScaleList.end(),
						[this,map]( const std::pair < int, std::vector < SpriteBatch::WorkScale > >& pair ){
			const std::vector < SpriteBatch::WorkScale >& works = pair.second;
			MAPIL::Set2DAlphaBlendingMode( pair.first );
			for( int i = 0; i < works.size(); ++i ){
				const SpriteBatch::WorkScale& w = works[ i ];
				ResourceMap::TextureAtlas atlas;
				if( w.m_Global ){
					atlas = map.m_pGlobalResourceMap->m_TexAtlasMap[ w.m_TexID ];
				}
				else{
					atlas = map.m_pStageResourceMap->m_TexAtlasMap[ w.m_TexID ];
				}
				MAPIL::DrawClipedTexture(	map.m_pStageResourceMap->m_TextureMap[ atlas.m_TexID ],
											w.m_PosX, w.m_PosY, w.m_ScaleX, w.m_ScaleY, w.m_Angle,
											atlas.m_X, atlas.m_Y,
											atlas.m_X + atlas.m_Width, atlas.m_Y + atlas.m_Height,
											w.m_Centerized, w.m_Color );
			}
		} );

		std::for_each(	m_AtlasWorkClipedList.begin(), m_AtlasWorkClipedList.end(),
						[this,map]( const std::pair < int, std::vector < SpriteBatch::WorkCliped > >& pair ){
			const std::vector < SpriteBatch::WorkCliped >& works = pair.second;
			MAPIL::Set2DAlphaBlendingMode( pair.first );
			for( int i = 0; i < works.size(); ++i ){
				const SpriteBatch::WorkCliped& w = works[ i ];
				ResourceMap::TextureAtlas atlas;
				if( w.m_Global ){
					atlas = map.m_pGlobalResourceMap->m_TexAtlasMap[ w.m_TexID ];
				}
				else{
					atlas = map.m_pStageResourceMap->m_TexAtlasMap[ w.m_TexID ];
				}
				MAPIL::DrawClipedTexture(	map.m_pStageResourceMap->m_TextureMap[ atlas.m_TexID ],
											w.m_PosX, w.m_PosY, w.m_ScaleX, w.m_ScaleY, w.m_Angle,
											atlas.m_X + w.m_ClipedX1, atlas.m_Y + w.m_ClipedY1,
											atlas.m_X + w.m_ClipedX2, atlas.m_Y + w.m_ClipedY2,
											w.m_Centerized, w.m_Color );
			}
		} );

		Cleanup();
		MAPIL::Set2DAlphaBlendingMode( MAPIL::ALPHA_BLEND_MODE_SEMI_TRANSPARENT );
	}

	void SpriteBatch::Cleanup()
	{
		m_WorkList.clear();
		m_WorkScaleList.clear();
		m_AtlasWorkList.clear();
		m_AtlasWorkScaleList.clear();
		m_AtlasWorkClipedList.clear();
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

	void AddToAtlasSpriteBatch(	bool global, int blendingMode, int atlasID, float posX, float posY, float angle,
								bool centerized, int color )
	{
		g_Batch.AddAtlas( global, blendingMode, atlasID, posX, posY, angle, centerized, color );
	}

	void AddToAtlasSpriteBatch(	bool global, int blendingMode, int atlasID, float posX, float posY, float scaleX, float scaleY, float angle,
								bool centerized, int color )
	{
		g_Batch.AddAtlas( global, blendingMode, atlasID, posX, posY, scaleX, scaleY, angle, centerized, color );
	}

	void AddToAtlasSpriteBatch(	bool global, int blendingMode, int atlasID,
								float cx1, float cy1, float cx2, float cy2,
								float posX, float posY, float scaleX, float scaleY, float angle,
								bool centerized, int color )
	{
		g_Batch.AddAtlas(	global, blendingMode, atlasID,
							cx1, cy1, cx2, cy2,
							posX, posY, scaleX, scaleY, angle, centerized, color );
	}

	void ProcAllBatchWorks( const ResourceMap& map )
	{
		g_Batch.ProcAllWorks( map );
	}
}