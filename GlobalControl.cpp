#include "GlobalControl.h"
#include "ConnectionGenerator.h"
#include <QtWidgets/QApplication>
#include <QDebug>
#include <windows.h>

GlobalControl* GlobalControl::m_Instance = NULL;

GlobalControl::GlobalControl()
{
    Initialize();

    pConnectionGenerator.Set_GlobalControl(this);
}

GlobalControl::~GlobalControl()
{
}



GlobalControl* GlobalControl::GetInstance()
{
    if (m_Instance == NULL)
    {
        static std::mutex m_mutex;
        MutexGuard(m_mutex);
        if (m_Instance == NULL)
        {
            GlobalControl* ptmp = new GlobalControl();
            m_Instance = ptmp;
        }
    }
    return m_Instance;
}

void GlobalControl::Initialize()
{

}


