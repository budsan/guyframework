#ifndef _CONTENT_INCLUDED_
#define _CONTENT_INCLUDED_

#include <set>
#include <map>
#include <boost/type_traits.hpp>

//---------------------------------------------------------------------------//

class Content
{
	//Inherit class MUST have an empty constructor
	Content() {}
	virtual ~Content() {}

	virtual int load(const char *filename) = 0;
	virtual int unload() = 0;

	virtual bool isNull() = 0;
};

//---------------------------------------------------------------------------//

class ContentManagerBase
{
public:
	ContentManagerBase();
	virtual ~ContentManagerBase();

	virtual void unloadAll() = 0;
	virtual unsigned int contentCount() = 0;

	static void unloadManagers();

protected:

	static std::set<ContentManagerBase*> s_instances;
};

//---------------------------------------------------------------------------//

template <typename T>
class ContentManager : boost::is_base_and_derived<Content, T>, public ContentManagerBase
{
public:
	static ContentManager& instance();
	static ContentManager* ptrInstance();
	~ContentManager(void) { unloadAll(); }

	int      load(const char *filename);
	int      unload(const char *filename);
	const T& get(const char *filename);

	void unloadAll();
	unsigned int contentCount();

protected:
	ContentManager() {}
	static ContentManager* s_instance;

	bool contains(const std::string &key) {
		return m_content.find(key) != m_content.end();
	}

	struct Attributes
	{
		//Empty struct. Specialize to add vars.
	};

	Attributes m_attr;
	std::map<std::string, T> m_content;
};

//---------------------------------------------------------------------------//

template <typename T> ContentManager<T>* ContentManager<T>::s_instance = 0;

template <typename T>
ContentManager<T>& ContentManager<T>::instance()
{
	return *ptrInstance();
}

template <typename T>
ContentManager<T>* ContentManager<T>::ptrInstance()
{
	if (s_instance == NULL) {
		s_instance = new ContentManager<T>();
	}

	return s_instance;
}

template <typename T>
int ContentManager<T>::load(const char *filename)
{
	std::string key = std::string(filename);
	if(contains(key)) return 0;

	T value;
	int res = value.load(filename);
	if (res >= 0) m_content.insert(key, value);
	return res;
}

template <typename T>
int ContentManager<T>::unload(const char *filename)
{
	return -1;
}

template <typename T>
const T& ContentManager<T>::get(const char *filename)
{
	std::string file = std::string(filename);
	typename std::map<std::string, T>::iterator iter = m_content.find(file);
	if(iter != m_content.end()) return iter->second;

	return T();
}

template <typename T>
void ContentManager<T>::unloadAll()
{
	while (!m_content.empty())
	{
		m_content.begin()->unload();
		m_content.erase(m_content.begin());
	}
}

template <typename T>
unsigned int ContentManager<T>::contentCount()
{
	return m_content.size();
}

#endif // _CONTENT_INCLUDED_
