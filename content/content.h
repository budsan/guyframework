#ifndef _CONTENT_INCLUDED_
#define _CONTENT_INCLUDED_

#include <set>
#include "boost/static_assert.hpp"

namespace content {

class ContentLoader
{
public:
	virtual ~ContentLoader() {}
	virtual void unloadAll() = 0;
};

extern std::set<ContentLoader*> s_contentLoaders;

//---------------------------------------------------------------------------//

template <typename T>
class Loader : ContentLoader
{
public:
	static Loader<T>* ptrInstance();
	static Loader<T>& instance();
	~Loader();

	int load(const char *filename);
	int unload(const char *filename);
	void unloadAll();

	const T &get(const char *filename);
private:
	Loader();

	static Loader<T>* m_instance;
	static bool m_isInstanced;
};

template <typename T> Loader<T>* Loader<T>::m_instance = 0;
template <typename T> bool Loader<T>::m_isInstanced = false;

template <typename T>
Loader<T>::Loader()
{
	s_contentLoaders.insert(this);
}

template <typename T>
Loader<T>::~Loader()
{
	this->unloadAll();
	s_contentLoaders.erase(this);
	m_instance = NULL;
}

template <typename T>
Loader<T>* Loader<T>::ptrInstance()
{
	if (Loader<T>::m_instance == 0)
	{
		m_instance = new Loader<T>();
	}

	return m_instance;
}

template <typename T>
Loader<T>& Loader<T>::instance()
{
	return *ptrInstance();
}

template <typename T>
int Loader<T>::load(const char *filename)
{
	BOOST_STATIC_ASSERT_MSG(sizeof(T) == sizeof(T), "Declare an explicit specialization to avoid this warning");
	return 0;
}

template <typename T>
int Loader<T>::unload(const char *filename)
{
	BOOST_STATIC_ASSERT_MSG(sizeof(T) == sizeof(T), "Declare an explicit specialization to avoid this warning");
	return 0;
}

template <typename T>
void Loader<T>::unloadAll()
{
	BOOST_STATIC_ASSERT_MSG(sizeof(T) == sizeof(T), "Declare an explicit specialization to avoid this warning");
}

template <typename T>
const T &Loader<T>::get(const char *filename)
{
	BOOST_STATIC_ASSERT_MSG(sizeof(T) == 0, "Declare an explicit specialization to avoid this warning");
	return *(const T*)0;
}

//---------------------------------------------------------------------------//

template <typename T>
int load(const char *filename)
{
	Loader<T> &loader = Loader<T>::instance();
	return loader.load(filename);
}

template <typename T>
const T &get(const char *filename)
{
	Loader<T> &loader = Loader<T>::instance();
	return loader.get(filename);
}

template <typename T>
int unload(const char *filename)
{
	Loader<T> &loader = Loader<T>::instance();
	return loader.unload(filename);
}

void unloadAll();

}// namespace content

#endif // _CONTENT_INCLUDED_
