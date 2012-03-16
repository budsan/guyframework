#include "content.h"

std::set<ContentManagerBase*> ContentManagerBase::s_instances;

ContentManagerBase::ContentManagerBase()
{
	s_instances.insert(this);
}

ContentManagerBase::~ContentManagerBase()
{
	s_instances.erase(this);
}

void ContentManagerBase::unloadManagers()
{
	std::set<ContentManagerBase*>::iterator it = s_instances.begin();
	for (; it != s_instances.end(); ++it) (*it)->unloadAll();
}
