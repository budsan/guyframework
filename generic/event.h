#pragma once

#include <vector>
#include <functional>

template<typename T>
class Event
{
public:
	typedef std::function<void (T)> Slot;

public:
	void call( T arg )
	{
		for( auto i = m_handlers.begin(); i != m_handlers.end(); i++ )
		{
			(*i)( arg );
		}
	}

	void operator () (T arg)
	{
		call( arg );
	}

	Event& operator += (Slot s)
	{
		m_handlers.push_back(s);
		return *this;
	}

	Event& operator -= (Slot s)
	{
		for (auto i = m_handlers.begin(); i != m_handlers.end(); i++)
		{
			if ( (*i).target() == s.target() )
			{
				m_handlers.erase(i);
				break;
			}
		}

		return *this;
	}

private:
	std::vector<Slot> m_handlers;
};
