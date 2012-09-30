#include "ScoreManager.h"

namespace GameEngine
{
	class ScoreManager::Impl
	{
	private:
		int			m_Score;			// スコア
		int			m_PendingScore;		// 追加予定のスコア
		int			m_AddScore;			// 次に追加されるべきスコア
		bool		m_NeedUpdate;		// 次に追加されるべきスコアが更新されるべき場合true
		int			m_RestFrame;		// 完全に追加されるまでのフレーム数
		int			m_FrameInterval;	// 追加までのフレーム数
	public:
		Impl();
		~Impl();
		void Add( int score );
		void Update();
		int GetScore() const;
		void Flush();
	};

	ScoreManager::Impl::Impl() :	m_Score( 0 ),
									m_PendingScore( 0 ),
									m_AddScore( 0 ),
									m_NeedUpdate( false ),
									m_RestFrame( 60 ),
									m_FrameInterval( 20 )
	{
		m_RestFrame = m_FrameInterval;
	}

	ScoreManager::Impl::~Impl()
	{
	}

	void ScoreManager::Impl::Add( int score )
	{
		if( !m_NeedUpdate && score != 0 ){
			m_NeedUpdate = true;
		}
		m_PendingScore += score;
	}

	void ScoreManager::Impl::Update()
	{
		if( m_NeedUpdate ){
			m_AddScore = m_PendingScore / m_FrameInterval;
			m_Score += m_PendingScore % m_FrameInterval;
			m_PendingScore -= m_PendingScore % m_FrameInterval;
			m_RestFrame = m_FrameInterval;
			m_NeedUpdate = false;
		}
		if( m_RestFrame > 0 && m_PendingScore != 0 ){
			m_Score += m_AddScore;
			m_PendingScore -= m_AddScore;
			--m_RestFrame;
		}
	}

	int ScoreManager::Impl::GetScore() const
	{
		return m_Score;
	}

	void ScoreManager::Impl::Flush()
	{
		m_Score += m_PendingScore;
		m_NeedUpdate = false;
		m_PendingScore = 0;
	}

	// ----------------------------------
	// 実装クラスの呼び出し
	// ----------------------------------

	ScoreManager::ScoreManager() : m_pImpl( new ScoreManager::Impl )
	{
	}

	ScoreManager::~ScoreManager()
	{
	}

	void ScoreManager::Add( int score )
	{
		m_pImpl->Add( score );
	}

	void ScoreManager::Update()
	{
		m_pImpl->Update();
	}

	int ScoreManager::GetScore() const
	{
		return m_pImpl->GetScore();
	}

	void ScoreManager::Flush()
	{
		m_pImpl->Flush();
	}
}