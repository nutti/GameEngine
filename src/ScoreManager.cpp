#include "ScoreManager.h"

namespace GameEngine
{
	class ScoreManager::Impl
	{
	private:
		int			m_Score;			// �X�R�A
		int			m_PendingScore;		// �ǉ��\��̃X�R�A
		int			m_AddScore;			// ���ɒǉ������ׂ��X�R�A
		bool		m_NeedUpdate;		// ���ɒǉ������ׂ��X�R�A���X�V�����ׂ��ꍇtrue
		int			m_RestFrame;		// ���S�ɒǉ������܂ł̃t���[����
		int			m_FrameInterval;	// �ǉ��܂ł̃t���[����
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
	// �����N���X�̌Ăяo��
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