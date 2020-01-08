#ifndef CONNECTIONGENERATOR_H
#define CONNECTIONGENERATOR_H

#include <QObject>
#include <mutex>
#include "Define.h"

#define ConnectionParamPointer(type,name,checkProcess) \
    type* name = NULL;\
    void Set##name(type* pointer){name = pointer;checkProcess;}

#define ConnectionParamPointer_2(type,name) \
    ConnectionParamPointer(type, name, CheckConnectionParam())

#define ConnectionParamPointer_1(type) \
    type* _##type = NULL;\
    void Set_##type(type* pointer){_##type = pointer;CheckConnectionParam();}


#define pConnectionGenerator (*ConnectionGenerator::GetInstance())

class GlobalControl;
class QHollowImageWidget;
class QMainMenu;

//简单的统一信号槽管理
class ConnectionGenerator : public QObject
{
    Q_OBJECT

public:
    static ConnectionGenerator* GetInstance();

    ConnectionParamPointer_1(GlobalControl);
    ConnectionParamPointer_1(QHollowImageWidget);
    ConnectionParamPointer_1(QMainMenu);

private:
    ConnectionGenerator();
    ~ConnectionGenerator();
    static ConnectionGenerator* m_Instance;
    std::mutex m_CheckMutex;

    void InitializeConnection();
    bool CheckConnectionParam()
    {
        MutexGuard(m_CheckMutex);
        if (
              _GlobalControl      == NULL
            ||_QHollowImageWidget == NULL
            ||_QMainMenu          == NULL
            )
        {
            return false;
        }

        InitializeConnection();
        return true;
    }

};

#endif // CONNECTIONGENERATOR_H
