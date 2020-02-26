#include "Function.h"

#include <windows.h>

#include <assert.h>
#include <iostream>
#include <fstream>

#include <QFile>
#include <QDir>
#include <QDebug>

Function::Function(QObject *parent)
	: QObject(parent)
{

}

Function::~Function()
{

}


bool Function::SpliteResolution(const QString& resolution_str, int* width, int* height)
{
	QStringList width_height_list = resolution_str.split("x");
	if (2 != width_height_list.count())
	{
		return false;
	}
	bool is_width_ok = false;
	bool is_height_ok = false;
	*width = width_height_list.at(0).toInt(&is_width_ok);
	*height = width_height_list.at(1).toInt(&is_height_ok);
	if (!is_width_ok || !is_height_ok)
	{
		return false;
	}
	return true;
}

QString Function::ProcessFileName(const QString& origin_file_name)
{
	QStringList str_list = origin_file_name.split(".");
	if (2 != str_list.count())
	{
		return origin_file_name;
	}
	QString new_file_name = QString("%1_processed.%2").arg(str_list.at(0)).arg(str_list.at(1));
	return new_file_name;
}

bool Function::EndsWith(const std::string& src_string, const std::string& tail_string)
{
	return src_string.compare(src_string.size() - tail_string.size(), tail_string.size(), tail_string) == 0;
}

bool Function::StartWith(const std::string& src_string, const std::string& head_string)
{
	return src_string.compare(0, head_string.size(), head_string) == 0;
}

bool Function::CopyFileToPath(const QString& sourceDir, QString toDir, bool coverFileIfExist, bool isPath)
{
    toDir.replace("\\", "/");
    if (sourceDir == toDir){
        return true;
    }

    QString pathDir = toDir;
    if (!isPath)
    {
        int index = toDir.lastIndexOf("/");
        pathDir = toDir.mid(0, index);
    }
    QDir targetDir(pathDir);


    if (!targetDir.exists()){    /**< 如果目标目录不存在，则进行创建 */
        if (!targetDir.mkdir(pathDir))
            return false;
    }

    if (!QFile::exists(sourceDir)){
        return false;
    }

    QDir createfile;
    bool exist = createfile.exists(toDir);
    if (exist){
        if (coverFileIfExist){
            createfile.remove(toDir);
        }
    }

    if (!QFile::copy(sourceDir, toDir))
    {
        return false;
    }
    return true;
}

bool Function::CopyDirctoryToPath(const QString &fromDir, const QString &toDir, bool coverFileIfExist)
{
    QDir sourceDir(fromDir);
    QDir targetDir(toDir);
    if (!targetDir.exists()){    /**< 如果目标目录不存在，则进行创建 */
        if (!targetDir.mkdir(toDir))
            return false;
    }

    QFileInfoList fileInfoList = sourceDir.entryInfoList();
    foreach(QFileInfo fileInfo, fileInfoList){
        if (fileInfo.fileName() == "." || fileInfo.fileName() == "..")
            continue;

        if (fileInfo.isDir()){    /**< 当为目录时，递归的进行copy */
            if (!CopyFileToPath(fileInfo.filePath(),
                targetDir.filePath(fileInfo.fileName()),
                coverFileIfExist))
                return false;
        }
        else{            /**< 当允许覆盖操作时，将旧文件进行删除操作 */
            if (coverFileIfExist && targetDir.exists(fileInfo.fileName())){
                targetDir.remove(fileInfo.fileName());
            }

            /// 进行文件copy
            if (!QFile::copy(fileInfo.filePath(),
                targetDir.filePath(fileInfo.fileName()))){
                return false;
            }
        }
    }
    return true;
}

bool Function::DeleteDir(const QString& path)
{
    if (path.isEmpty()){
        return false;
    }
    QDir dir(path);
    if (!dir.exists()){
        return true;
    }
    dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot); //设置过滤
    QFileInfoList fileList = dir.entryInfoList(); // 获取所有的文件信息
    foreach(QFileInfo file, fileList){ //遍历文件信息
        if (file.isFile()){ // 是文件，删除
            file.dir().remove(file.fileName());
        }
        else{ // 递归删除
            DeleteDir(file.absoluteFilePath());
        }
    }
    return dir.rmpath(dir.absolutePath()); // 删除文件夹
}

bool Function::replace_first(std::string& str, const std::string& old_value, const std::string& new_value)
{
    std::string::size_type pos(0);
    if ((pos = str.find(old_value)) != std::string::npos)
    {
        str.replace(pos, old_value.length(), new_value);
        return true;
    }
    else return false;
}

bool Function::replace_all_distinct(std::string& str, const std::string& old_value, const std::string& new_value)
{
    bool bfind = false;
    for (std::string::size_type pos(0); pos != std::string::npos; pos += new_value.length())
    {
        if ((pos = str.find(old_value, pos)) != std::string::npos)
        {
            str.replace(pos, old_value.length(), new_value);
            bfind = true;
        }
        else
            break;
    }
    return bfind;
}

std::string Function::to_string_nozero(float input)
{
    char _output[20];
    memset(_output, 0, 20);

    sprintf_s(_output, "%g", input);

    return _output;
}

template<typename _MapType>
auto Function::get_map_key_value(const _MapType& input_map, const decltype(input_map.begin()->second)& mapped_value) -> decltype(input_map.begin()->first)
{
    auto iter = std::find_if(input_map.begin(), input_map.end(), [mapped_value](const auto& item) {
        return (item.second == mapped_value);
    });

    if (iter == input_map.end())
    {
        return decltype(input_map.begin()->first)();
    }
    return iter->first;
}


std::vector<std::string> Function::StringSplit(const std::string& str, std::string sep)
{
    std::vector<std::string> ret_;

    std::string tmp;
    tmp.clear();
    std::string::size_type pos_begin = str.find_first_not_of(sep);
    std::string::size_type sep_pos_end = str.find_last_of(sep);
    std::string::size_type comma_pos = 0;

    while (pos_begin != std::string::npos)
    {
        comma_pos = str.find(sep, pos_begin);
        if (comma_pos != std::string::npos)
        {
            tmp += str.substr(pos_begin, comma_pos - pos_begin);
            pos_begin = comma_pos + sep.length();
            if (pos_begin < sep_pos_end) continue;
        }
        else
        {
            tmp = str.substr(pos_begin);
            pos_begin = comma_pos;
        }
        if (!tmp.empty())
        {
            ret_.push_back(tmp);
            tmp.clear();
        }
    }
    return ret_;
}

std::string Function::GetSubDir(std::string dir)
{
    int pos = dir.find_last_of("/");
    if (pos != std::string::npos)
    {
        return dir.substr(0, pos);
    }
    else
    {
#ifdef _DEBUG
        assert(0);
#endif
        return "";
    }
}

std::string Function::GetDirFileName(std::string dir)
{
    int pos = dir.find_last_of("/");
    if (pos != std::string::npos)
    {
        return dir.substr(pos + 1, dir.length() - pos - 1);
    }
    else
    {
#ifdef _DEBUG
        assert(0);
#endif
        return "";
    }
}

bool Function::ReadFile(const std::string& filename, std::string& content)
{
    std::ifstream ifile;
    ifile.open(filename, std::ifstream::binary);
    if (ifile)
    {
        std::string str_file((std::istreambuf_iterator<char>(ifile)), std::istreambuf_iterator<char>());
        content = str_file;
        ifile.close();
        return true;
//  another way to read . 
//         std::ifstream in("some.file");
//         std::ostringstream tmp;
//         tmp << in.rdbuf();
//         std::string str = tmp.str();
    }
    else
    {
        ifile.close();
        return false;
    }
}

bool Function::WriteFile(const std::string& filename, const std::string& content)
{
    std::ofstream fout(filename, std::ifstream::binary);
    if (fout)
    {
        fout << content;
        fout.close();
        return true;
    }
    else
    {
        fout.close();
        return false;
    }
}


#ifdef _WIN32

std::string Function::StrToUtf(const std::string& str)
{
    int nwLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
    wchar_t* pwBuf = new wchar_t[nwLen + 1];
    ZeroMemory(pwBuf, nwLen * 2 + 2);
    ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), (int)str.length(), pwBuf, nwLen);
    int nLen = ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL);
    char* pBuf = new char[nLen + 1];
    ZeroMemory(pBuf, nLen + 1);
    ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);
    std::string retStr(pBuf);

    delete[]pwBuf;	delete[]pBuf;
    pwBuf = NULL;	pBuf = NULL;
    return retStr;
}

std::string Function::UtfToStr(const std::string& str)
{
    int nwLen = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
    wchar_t * pwBuf = new wchar_t[nwLen + 1];
    memset(pwBuf, 0, nwLen * 2 + 2);
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.length(), pwBuf, nwLen);
    int nLen = WideCharToMultiByte(CP_ACP, 0, pwBuf, -1, NULL, NULL, NULL, NULL);
    char * pBuf = new char[nLen + 1];
    memset(pBuf, 0, nLen + 1);
    WideCharToMultiByte(CP_ACP, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);
    std::string retStr = pBuf;
    delete[]pBuf;	delete[]pwBuf;

    pBuf = NULL;	pwBuf = NULL;
    return retStr;
}

bool Function::DeleteFolderA(std::string lpszPath)
{
    SHFILEOPSTRUCTA FileOp;
    ZeroMemory((void*)&FileOp, sizeof(SHFILEOPSTRUCTA));
    char path[259];
    ZeroMemory(path, 259);

    FileOp.fFlags = FOF_NOCONFIRMATION;
    FileOp.hNameMappings = NULL;
    FileOp.hwnd = NULL;
    FileOp.lpszProgressTitle = NULL;
    lpszPath.copy(path, lpszPath.length(), 0);
    FileOp.pFrom = path;
    FileOp.pTo = NULL;
    FileOp.wFunc = FO_DELETE;

    bool isRemoved = !SHFileOperationA(&FileOp);
    return isRemoved;
}

bool Function::CopyFolderA(std::string lpszFromPath, std::string lpszToPath)
{
    SHFILEOPSTRUCTA FileOp;
    ZeroMemory((void*)&FileOp, sizeof(SHFILEOPSTRUCTA));
    char path_from[259];
    ZeroMemory(path_from, 259);
    char path_to[259];
    ZeroMemory(path_to, 259);

    FileOp.fFlags = FOF_NOCONFIRMATION;
    FileOp.hNameMappings = NULL;
    FileOp.hwnd = NULL;
    FileOp.lpszProgressTitle = NULL;
    lpszFromPath.copy(path_from, lpszFromPath.length(), 0);
    FileOp.pFrom = path_from;
    lpszToPath.copy(path_to, lpszToPath.length(), 0);
    FileOp.pTo = path_to;
    FileOp.wFunc = FO_COPY;

    return SHFileOperationA(&FileOp) == 0;
}

#include <ShlObj.h>
#include <Shlwapi.h>
#pragma comment(lib,"Shlwapi.lib")
bool Function::CreatePath(const std::string& path, bool bReplaceIfExist)
{
    bool exist = PathFileExistsA(path.c_str());
    if (!exist || bReplaceIfExist)
    {
        if (ERROR_SUCCESS != SHCreateDirectoryExA(NULL, path.c_str(), NULL))
        {
            return false;
        }
    }

    return true;
}

#elif __linux__
#else
#   error "Unknown compiler"
#endif
