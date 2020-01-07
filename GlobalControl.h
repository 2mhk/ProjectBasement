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

//һ����������õĻ��ܹ��ܵĵ�����
//��������ֱ�Ӳ�������������װ�õ�managerָ�봫��������
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
