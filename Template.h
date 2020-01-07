#ifndef JGTEMPLATE_H
#define JGTEMPLATE_H

template <class T>
class ElementInterface
{
private:
    T* m_Element;
public:
    ElementInterface(T& input)
    {
        m_Element = &input;
    }

    virtual T Get()
    {
        return *m_Element;
    }

    virtual void Set(T input)
    {
        *m_Element = input;
    }
};

template <class T>
class ElementInterface_Locked
{
private:
    std::mutex m_LocalMutex;
protected:
    T* m_Element;
    std::mutex* m_mutex;
public:
    ElementInterface_Locked(T& input, std::mutex& mutex)
    {
        m_mutex = mutex;
        m_Element = &input;
    }

    ElementInterface_Locked(T& input)
    {
        m_mutex = &m_LocalMutex;
        m_Element = &input;
    }

    virtual T Get()
    {
        lock_guard<std::mutex> guard(*m_mutex);
        return *m_Element;
    }

    virtual void Set(T input)
    {
        lock_guard<std::mutex> guard(*m_mutex);
        *m_Element = input;
    }
};

//Abandon
template <class T, class FuncType>
class ElementInterface_Callback
{
public:
    typedef std::function<void()> CallEmpty;
    typedef std::function<void(T*)> CallElement;

protected:
    T* m_Element;
    FuncType m_GetProcess;
    FuncType m_SetProcess;

    bool m_isConfirmedType;     //回调函数的使能

    void CheckCallback()        //初始化时调用
    {
        if (std::is_same<CallEmpty, FuncType>::value
            || std::is_same<CallElement, FuncType>::value)
            m_isConfirmedType = true;
        else
            m_isConfirmedType = false;
    }

    void CallFunction(FuncType _function)
    {
        if (_function == NULL)
            return;

        if (std::is_same<CallEmpty, FuncType>::value)
            _function();
        else if (std::is_same<CallElement, FuncType>::value)
            _function(m_Element);
        else
            return;
    }

public:
    ElementInterface_Callback(T& input, FuncType GetProcess, FuncType SetProcess)
    {
        CheckCallback();
        m_GetProcess = GetProcess;
        m_SetProcess = SetProcess;
        m_Element = &input;
    }

    ElementInterface_Callback(T& input)
    {
        m_isConfirmedType = false;
        m_GetProcess = NULL;
        m_SetProcess = NULL;
        m_Element = &input;
    }

    virtual T Get()
    {
        if (m_isConfirmedType)
            CallFunction(m_GetProcess);     //回调 - 仅动作 
        return *m_Element;
    }

    virtual void Set(T input)
    {
        if (m_isConfirmedType && m_SetProcess != NULL)
        {
            if (std::is_same<CallEmpty, FuncType>::value)
                m_SetProcess();                  //回调 - 仅动作 
            else if (std::is_same<CallElement, FuncType>::value)
                m_SetProcess(&input);            //回调 - 传递即将赋入的参数的引用
        }
        *m_Element = input;
    }
};

//Abandon
template <class T, class FuncType>
class ElementInterface_Callback_locked : public ElementInterface_Callback<T,FuncType>
{
protected:
    std::mutex* m_mutex;
    std::mutex m_LocalMutex;
public:
    ElementInterface_Callback_locked(T& input, FuncType GetProcess, FuncType SetProcess, std::mutex& mutex)
    {
        m_mutex = mutex;
        m_Element = &input;
    }

    ElementInterface_Callback_locked(T& input, FuncType GetProcess, FuncType SetProcess)
    {
        m_mutex = &m_LocalMutex;
        m_Element = &input;
    }

    virtual T Get()
    {
        std::lock_guard<std::mutex> guard(*m_mutex);
        if (m_isConfirmedType)
            CallFunction(m_GetProcess);
        return *m_Element;
    }

    virtual void Set(T input)
    {
        std::lock_guard<std::mutex> guard(*m_mutex);
        if (m_isConfirmedType && m_SetProcess != NULL)
        {
            if (std::is_same<CallEmpty, FuncType>::value)
                m_SetProcess();
            else if (std::is_same<CallElement, FuncType>::value)
                m_SetProcess(input);
        }
        *m_Element = input;
    }
};






#endif 

