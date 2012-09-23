#ifndef INCLUDED_GAMEENGINE_GAMEOBJECTLIST_HPP
#define INCLUDED_GAMEENGINE_GAMEOBJECTLIST_HPP

#include <MAPIL/MAPIL.h>

#include <list>

namespace GameEngine
{
	template < typename T >
	class GameObjectList
	{
	private:
		std::list < T >							m_List;
		typename std::list < T > ::iterator		m_Iter;
	public:
		GameObjectList();
		~GameObjectList();
		void Add( T p );		// 追加
		void Delete( T p );		// 削除
		void Erase( T p );		// delete演算子付削除
		void EraseAll();		// 全消去

		void IterStart();		// 走査開始
		bool IterNext();		// 次の操作を取得
		bool IsEnd();			// 走査完了か？
		T Get();				// 現在挿しているオブジェクトを取得
	};

	template < typename T >
	GameObjectList < T > ::GameObjectList()
	{
		m_List.clear();
	}

	template < typename T >
	GameObjectList < T > ::~GameObjectList()
	{
		m_List.clear();
	}

	template < typename T >
	void GameObjectList < T > ::Add( T p )
	{
		m_List.push_back( p );
	}

	template < typename T >
	void GameObjectList < T > ::Delete( T p )
	{
		m_List.remove( p );
	}

	template < typename T >
	void GameObjectList < T > ::Erase( T p )
	{
		m_List.remove( p );
		MAPIL::SafeDelete( p );
	}

	template < typename T >
	void GameObjectList < T > ::EraseAll()
	{
		for( std::list < T > ::iterator it = m_List.begin(); it != m_List.end(); ++it ){
			MAPIL::SafeDelete( *it );
		}
		m_List.clear();
	}

	template < typename T >
	void GameObjectList < T > ::IterStart()
	{
		m_Iter = m_List.begin();
	}

	template < typename T >
	bool GameObjectList < T > ::IterNext()
	{
		++m_Iter;
	}

	template < typename T >
	bool GameObjectList < T > ::IsEnd()
	{
		return m_Iter == m_List.end();
	}

	template < typename T >
	T GameObjectList < T > ::Get()
	{
		return *m_Iter;
	}
}

#endif