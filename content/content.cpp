#include "content.h"

namespace content
{
	std::set<ContentLoader*> s_contentLoaders;

	void unloadAll()
	{
		std::set<ContentLoader*>::iterator it = s_contentLoaders.begin();
		for (; it != s_contentLoaders.end(); ++it) {
			(*it)->unloadAll();
		}
	}

} //namespace Content
