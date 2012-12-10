#include "controller.h"

namespace Guy {

Controller::Controller()
{
    m_isEnabled = true;
    m_isConnected = false;
}

Controller::~Controller()
{
}

void Controller::setConnected(bool connected) 
{ 
    m_isConnected = connected;
}

bool Controller::isConnected() 
{ 
    return m_isConnected;
}

void Controller::setEnabled(bool enabled) 
{ 
    m_isEnabled = enabled;
}

bool Controller::isEnabled() 
{ 
    return m_isEnabled && m_isConnected;
}

} // namespace Guy
