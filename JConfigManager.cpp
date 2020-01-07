#include "JConfigManager.h"

#include "Jsoncpp_Modify.h"
#include "Function.h"
#include "Define.h"

#include <fstream>
#include <iostream>
#include <stdlib.h>

using namespace std;
using namespace std::placeholders;

JConfigManager::JConfigManager()
: m_fileName(""), m_jsonbuffer("")
{
    m_ITF_CameraConfig = new JsonFileElement_LC<CameraConfig>(m_CameraConfig);
    CallPointer callBack_CameraConfig = std::bind(&JConfigManager::CheckCameraConfig, this, _1);
    m_ITF_CameraConfig->SetWriteProcess(callBack_CameraConfig);
}

JConfigManager::~JConfigManager()
{
    delete m_ITF_CameraConfig;
}

void JConfigManager::ClearResult()
{
    m_CameraConfig.Clear();
}

bool JConfigManager::Read(const std::string& fileName)
{
    MutexGuard(m_mutex);
    std::ifstream ifile;
    ifile.open(fileName, std::ifstream::binary);
    if (ifile)
    {
        m_fileName = fileName;
        std::string str_tmp((std::istreambuf_iterator<char>(ifile)), std::istreambuf_iterator<char>());
        m_jsonbuffer = str_tmp;
        ifile.close();
    }
    else
    {
        return false;
    }

    return Parse();
}

bool JConfigManager::Parse()
{
    Json::Value root;
    Json::Reader reader;

    ClearResult();

    if (reader.parse(m_jsonbuffer, root))
    {
        try
        {
            Json_CheckAndGetMember(root, Camera_Config)
                m_CameraConfig.LoadFromJson(_Camera_Config);
            Json_CheckFalse
                m_CameraConfig.Clear();
            Json_CheckEnd;
        }
        catch (...)
        {
            return false;
        }
    }
    else
    {
        return false;
    }

    return true;
}

bool JConfigManager::Write()
{
    MutexGuard(m_mutex);
    Json::Value root;
    Json::Reader reader;

    if (!reader.parse(m_jsonbuffer, root))      //对未知元素的保存
    {
        return false;
    }

    Json::Value& Camera_Config = root["Camera_Config"];
    m_CameraConfig.WriteToJson(Camera_Config);

    string strFileBuffer;
    Json::StringWriter strWriter;
    strFileBuffer = strWriter.write(root);
    m_jsonbuffer = strFileBuffer;

    std::ofstream fout(m_fileName, std::ifstream::binary);
    if (!fout)
    {
        return false;
    }
    fout << strFileBuffer;
    fout.close();

    return true;
}

void JConfigManager::CheckCameraConfig(void* _pointer)
{
    CameraConfig* input = static_cast<CameraConfig*>(_pointer);
    if (!(*input == m_CameraConfig))
    {
        m_CameraConfig = *input;
        Write();
    }
}