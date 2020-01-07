#ifndef GLOBALCONTROLERCP_H
#define GLOBALCONTROLERCP_H

#include <mutex>
#include <atomic>

#include <QObject>

#include "util.h"

#include "ConnectionGenerator.h"

#define pGlobalController (*GlobalControl::GetInstance())


class QConnectionGlobal : public QObject
{
    Q_OBJECT

signals:


};

//一个供界面调用的汇总功能的单例类
//尽量减少直接操作函数，将封装好的manager指针传给调用者
class GlobalControl
{
public:
    static GlobalControl* GetInstance();

    QConnectionGlobal m_QInterface;

    //CommonUseful

private:
    void Initialize();

private:
    GlobalControl();
    ~GlobalControl();
    static GlobalControl* m_Instance;

    std::string m_Log;


};




#endif GLOBALCONTROLERCP_H
