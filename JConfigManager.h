#ifndef JCONFIGMANAGER_H
#define JCONFIGMANAGER_H

#include <QString>
#include <string>
#include <mutex>
#include <functional>
#include <map>
#include "Template.h"
#include "Jsoncpp_Modify.h"
#include "Function.h"


struct CameraConfig
{
    int ImageOffSet_x;
    int ImageOffSet_y;

    void Clear()
    {
        //cameraParam_AutoFoucs.Clear();
        ImageOffSet_x = 0;
        ImageOffSet_y = 0;
    }

    bool LoadFromJson(const Json::Value& root)
    {
        try
        {
            //Json_CheckAndGetMember(root, cameraParam_AutoFoucs)
            //    cameraParam_AutoFoucs.LoadFromJson(_cameraParam_AutoFoucs);
            //Json_CheckFalse
            //    cameraParam_AutoFoucs.Clear();
            //Json_CheckEnd;

            Json_BaseValueLoad(root, ImageOffSet_x, asInt, 0);
            Json_BaseValueLoad(root, ImageOffSet_y, asInt, 0);
        }
        catch (...)
        {
            Clear();
            return false;
        }

        return true;
    }

    bool WriteToJson(Json::Value& root)
    {
        try
        {
            root["ImageOffSet_x"] = ImageOffSet_x;
            root["ImageOffSet_y"] = ImageOffSet_y;
        }
        catch (...)
        {
            return false;
        }

        return true;
    }

    friend bool operator==(const CameraConfig &_Compare, const CameraConfig &toCompare)
    {
        if (
            _Compare.ImageOffSet_x          == toCompare.ImageOffSet_x          &&
            _Compare.ImageOffSet_y          == toCompare.ImageOffSet_y
            )
            return true;
        else
            return false;
    }
};


class JConfigManager
{
    typedef std::function<void(CameraConfig&)> CallGeneral;
    typedef std::function<void(void*)> CallPointer;
    typedef std::function<void()> CallEmpty;

    template <class T>
    class JsonFileElement_LC : public ElementInterface_Locked<T>
    {
        friend class JConfigManager;
    protected:
        CallPointer m_callback;

        void SetWriteProcess(CallPointer _CallbackFunction)
        {
            m_callback = move(_CallbackFunction);
        }
    public:
        JsonFileElement_LC(T& input, std::mutex& mutex) : ElementInterface_Locked<T>(input, mutex){}
        JsonFileElement_LC(T& input) : ElementInterface_Locked<T>(input){}

        //涉及文件IO操作，勿频繁操作。
        virtual void Set(T input)
        {
            lock_guard<std::mutex> guard(*m_mutex);
            if (m_callback._Empty())
                *m_Element = input;
            else
                m_callback(&input);
        }
    };

public:
    JConfigManager();
    ~JConfigManager();

    bool Read(const std::string& fileName);
    bool Parse();
    bool Write();

    void ClearResult();

private:
    void CheckCameraConfig(void* input);

private:
    CameraConfig m_CameraConfig;

public:
    JsonFileElement_LC<CameraConfig>* m_ITF_CameraConfig = NULL;

private:
    std::string m_jsonbuffer;	//用于暂存与写入多余的内容
    std::string m_fileName;     //文件名

    std::mutex m_mutex;

};



#endif
