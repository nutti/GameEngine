#include <MAPIL/MAPIL.h>

#include "GameDataHolder.h"

namespace GameEngine
{

	class GameDataHolder::Impl
	{
	private:
		GameDataMsg			m_GameData;				// 現在のゲームデータ
		GameDataMsg			m_TempGameData;			// 一時敵なゲームデータ
	public:
		Impl();
		~Impl(){}
		void StartRecording();
		void EndRecording();
		void Update();
		GameDataMsg GetScoreData() const;
		void Add( const GameDataMsg& data );
	};

	GameDataHolder::Impl::Impl()
	{
		MAPIL::ZeroObject( &m_GameData, sizeof( m_GameData ) );
	}

	void GameDataHolder::Impl::StartRecording()
	{
		MAPIL::ZeroObject( &m_GameData, sizeof( m_GameData ) );
	}

	void GameDataHolder::Impl::EndRecording()
	{
	}

	void GameDataHolder::Impl::Update()
	{
	}

	GameDataMsg GameDataHolder::Impl::GetScoreData() const
	{
		return m_GameData;
	}

	void GameDataHolder::Impl::Add( const GameDataMsg& data )
	{
		m_GameData.m_Score = data.m_Score;
		m_GameData.m_HIScore = data.m_HIScore;
		m_GameData.m_Killed = data.m_Killed;
		m_GameData.m_CrystalTotal = data.m_CrystalTotal;
	}


	// ----------------------------------
	// 実装クラスの呼び出し
	// ----------------------------------

	GameDataHolder::GameDataHolder() : m_pImpl( new GameDataHolder::Impl )
	{
	}

	GameDataHolder::~GameDataHolder()
	{
	}

	void GameDataHolder::StartRecording()
	{
		m_pImpl->StartRecording();
	}

	void GameDataHolder::EndRecording()
	{
		m_pImpl->EndRecording();
	}

	void GameDataHolder::Update()
	{
		m_pImpl->Update();
	}

	GameDataMsg GameDataHolder::GetScoreData() const
	{
		return m_pImpl->GetScoreData();
	}

	void GameDataHolder::Add( const GameDataMsg& data )
	{
		m_pImpl->Add( data );
	}

}