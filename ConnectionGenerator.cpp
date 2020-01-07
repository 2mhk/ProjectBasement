#include "ConnectionGenerator.h"

#include "GlobalControl.h"
#include "QHollowImageWidget.h"
#include "QMainMenu.h"

#include <mutex>

ConnectionGenerator* ConnectionGenerator::m_Instance = NULL;

ConnectionGenerator* ConnectionGenerator::GetInstance()
{
    if (m_Instance == NULL)
    {
        static std::mutex m_mutex;
        MutexGuard(m_mutex);
        if (m_Instance == NULL)
        {
            ConnectionGenerator* ptmp = new ConnectionGenerator();
            m_Instance = ptmp;
        }
    }
    return m_Instance;
}

ConnectionGenerator::ConnectionGenerator()
{
}

ConnectionGenerator::~ConnectionGenerator()
{
}

void ConnectionGenerator::InitializeConnection()
{
    //TODO
    //Add connection in this function



}


