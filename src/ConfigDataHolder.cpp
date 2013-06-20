#include <MAPIL/MAPIL.h>

#include <time.h>

#include "ConfigDataHolder.h"
#include "Util.h"

namespace GameEngine
{

	struct ConfigFileData
	{
		GameConfigData		m_ConfigData;
	};

	class ConfigDataHolder::Impl
	{
	private:
		ConfigFileData		m_ConfigFileData;			// セーブファイルに書き出すデータ
	public:
		Impl();
		~Impl(){}
		void Save( const std::string& fileName );
		void Load( const std::string& fileName );
		int GetBGMVolume() const;
		int GetSEVolume() const;
		int GetPlaySpeed() const;
		void SetBGMVolume( int volume );
		void SetSEVolume( int volume );
		void SetPlaySpeed( int speed );
	};

	ConfigDataHolder::Impl::Impl()
	{
		MAPIL::ZeroObject( &m_ConfigFileData, sizeof( m_ConfigFileData ) );
		m_ConfigFileData.m_ConfigData.m_BGMVolume = MAX_BGM_VOLUME;
		m_ConfigFileData.m_ConfigData.m_SEVolume = MAX_SE_VOLUME;
		m_ConfigFileData.m_ConfigData.m_PlaySpeed = GAME_PLAY_SPEED_NORMAL;
	}

	void ConfigDataHolder::Impl::Load( const std::string& fileName )
	{
		std::fstream fIn( fileName, std::ios::in );
		if( !fIn ){
			return;
		}

		while( !fIn.eof() ){
			char str[ 1024 ];
			if( fIn.getline( str, sizeof( str ) ) != NULL ){
				std::string s = str;
				std::string::size_type end = s.find( '=', 0 );
				if( end != std::string::npos ){
					std::string key;
					std::string value;
					key = s.substr( 0, end );
					value = s.substr( end + 1, s.length() - end - 1 );
					if( key == "BGM" ){
						m_ConfigFileData.m_ConfigData.m_BGMVolume = ::atoi( value.c_str() );
						if( m_ConfigFileData.m_ConfigData.m_BGMVolume > MAX_BGM_VOLUME ){
							m_ConfigFileData.m_ConfigData.m_BGMVolume = MAX_BGM_VOLUME;
						}
						else if( m_ConfigFileData.m_ConfigData.m_BGMVolume < MIN_BGM_VOLUME ){
							m_ConfigFileData.m_ConfigData.m_BGMVolume = MIN_BGM_VOLUME;
						}
					}
					else if( key == "SE" ){
						m_ConfigFileData.m_ConfigData.m_SEVolume = ::atoi( value.c_str() );
						if( m_ConfigFileData.m_ConfigData.m_SEVolume > MAX_SE_VOLUME ){
							m_ConfigFileData.m_ConfigData.m_SEVolume = MAX_SE_VOLUME;
						}
						else if( m_ConfigFileData.m_ConfigData.m_SEVolume < MIN_SE_VOLUME ){
							m_ConfigFileData.m_ConfigData.m_SEVolume = MIN_SE_VOLUME;
						}
					}
					else if( key == "PLAY_SPEED" ){
						std::string strings[ 7 ] = { "1/4", "1/3", "1/2", "x1", "x2", "x3", "x4" };
						m_ConfigFileData.m_ConfigData.m_PlaySpeed = GAME_PLAY_SPEED_NORMAL;
						for( int i = 0; i < 7; ++i ){
							if( strings[ i ] == value ){
								m_ConfigFileData.m_ConfigData.m_PlaySpeed = i;
							}
						}
						if( m_ConfigFileData.m_ConfigData.m_PlaySpeed < GAME_PLAY_SPEED_X1_4 ){
							m_ConfigFileData.m_ConfigData.m_PlaySpeed = GAME_PLAY_SPEED_X1_4;
						}
						else if( m_ConfigFileData.m_ConfigData.m_PlaySpeed > GAME_PLAY_SPEED_X4 ){
							m_ConfigFileData.m_ConfigData.m_PlaySpeed = GAME_PLAY_SPEED_X4;
						}
					}
				}
			}
		}

		fIn.close();
	}

	void ConfigDataHolder::Impl::Save( const std::string& fileName )
	{
		if( !FileExist( "config" ) ){
			CreateDirectory( "config" );
		}

		std::fstream fOut( fileName, std::ios::out );
		if( !fOut ){
			return;
		}

		fOut << "BGM=" << m_ConfigFileData.m_ConfigData.m_BGMVolume << std::endl;
		fOut << "SE=" << m_ConfigFileData.m_ConfigData.m_SEVolume << std::endl;
		std::string strings[ 7 ] = { "1/4", "1/3", "1/2", "x1", "x2", "x3", "x4" };
		fOut << "PLAY_SPEED=" << strings[ m_ConfigFileData.m_ConfigData.m_PlaySpeed ] << std::endl;

		fOut.close();
	}

	int ConfigDataHolder::Impl::GetBGMVolume() const
	{
		return m_ConfigFileData.m_ConfigData.m_BGMVolume;
	}

	int ConfigDataHolder::Impl::GetSEVolume() const
	{
		return m_ConfigFileData.m_ConfigData.m_SEVolume;
	}

	int ConfigDataHolder::Impl::GetPlaySpeed() const
	{
		return m_ConfigFileData.m_ConfigData.m_PlaySpeed;
	}

	void ConfigDataHolder::Impl::SetBGMVolume( int volume )
	{
		if( volume > MAX_BGM_VOLUME ){
			m_ConfigFileData.m_ConfigData.m_BGMVolume = MAX_BGM_VOLUME;
		}
		else if( volume < MIN_BGM_VOLUME ){
			m_ConfigFileData.m_ConfigData.m_BGMVolume = MIN_BGM_VOLUME;
		}
		else{
			m_ConfigFileData.m_ConfigData.m_BGMVolume = volume;
		}
	}

	void ConfigDataHolder::Impl::SetSEVolume( int volume )
	{
		if( volume > MAX_SE_VOLUME ){
			m_ConfigFileData.m_ConfigData.m_SEVolume = MAX_SE_VOLUME;
		}
		else if( volume < MIN_SE_VOLUME ){
			m_ConfigFileData.m_ConfigData.m_SEVolume = MIN_SE_VOLUME;
		}
		else{
			m_ConfigFileData.m_ConfigData.m_SEVolume = volume;
		}
	}

	void ConfigDataHolder::Impl::SetPlaySpeed( int speed )
	{
		if( speed > GAME_PLAY_SPEED_X4 ){
			m_ConfigFileData.m_ConfigData.m_PlaySpeed = GAME_PLAY_SPEED_X4;
		}
		else if( speed < GAME_PLAY_SPEED_X1_4 ){
			m_ConfigFileData.m_ConfigData.m_PlaySpeed = GAME_PLAY_SPEED_X1_4;
		}
		else{
			m_ConfigFileData.m_ConfigData.m_PlaySpeed = speed;
		}
	}

	// ----------------------------------
	// 実装クラスの呼び出し
	// ----------------------------------

	ConfigDataHolder::ConfigDataHolder() : m_pImpl( new ConfigDataHolder::Impl )
	{
	}

	ConfigDataHolder::~ConfigDataHolder()
	{
	}

	void ConfigDataHolder::Load( const std::string& fileName )
	{
		m_pImpl->Load( fileName );
	}

	void ConfigDataHolder::Save( const std::string& fileName )
	{
		m_pImpl->Save( fileName );
	}

	int ConfigDataHolder::GetBGMVolume() const
	{
		return m_pImpl->GetBGMVolume();
	}

	int ConfigDataHolder::GetSEVolume() const
	{
		return m_pImpl->GetSEVolume();
	}

	int ConfigDataHolder::GetPlaySpeed() const
	{
		return m_pImpl->GetPlaySpeed();
	}

	void ConfigDataHolder::SetBGMVolume( int volume )
	{
		m_pImpl->SetBGMVolume( volume );
	}

	void ConfigDataHolder::SetSEVolume( int volume )
	{
		m_pImpl->SetSEVolume( volume );
	}

	void ConfigDataHolder::SetPlaySpeed( int speed )
	{
		m_pImpl->SetPlaySpeed( speed );
	}
}