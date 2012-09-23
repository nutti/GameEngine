#include "ReplayDataBuilder.h"

#include <vector>

namespace GameEngine
{

	class ReplayDataBuilder::Impl
	{
	private:
		std::vector < ButtonPushedStatus >		m_ButtonList;			// �ߋ��̃{�^���̗���
	public:
		Impl();
		~Impl(){}
		void AddButtonState( ButtonPushedStatus status );
		void Save( const std::string& fileName );
		void Cleanup();
	};

	ReplayDataBuilder::Impl::Impl()
	{
	}

	void ReplayDataBuilder::Impl::AddButtonState( ButtonPushedStatus status )
	{
	}
	
	void ReplayDataBuilder::Impl::Save( const std::string& fileName )
	{
	}

	void ReplayDataBuilder::Impl::Cleanup()
	{
	}

	// ----------------------------------
	// �����N���X�̌Ăяo��
	// ----------------------------------

	ReplayDataBuilder::ReplayDataBuilder() : m_pImpl( new ReplayDataBuilder::Impl )
	{
	}

	ReplayDataBuilder::~ReplayDataBuilder()
	{
	}

	void ReplayDataBuilder::AddButtonState( ButtonPushedStatus status )
	{
		m_pImpl->AddButtonState( status );
	}

	void ReplayDataBuilder::Save( const std::string& fileName )
	{
		m_pImpl->Save( fileName );
	}

	void ReplayDataBuilder::Cleanup()
	{
		m_pImpl->Cleanup();
	}

}