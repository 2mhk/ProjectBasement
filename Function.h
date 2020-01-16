#ifndef FUNCTION_H
#define FUNCTION_H

#include <QObject>
#include <QImage>

class Function : public QObject
{
public:
	Function(QObject *parent = 0);
	~Function();

	static bool SpliteResolution(const QString& resolution_str, int* width, int* height);

	static QString ProcessFileName(const QString& origin_file_name);

	static bool EndsWith(const std::string& src_string, const std::string& tail_string);

	static bool StartWith(const std::string& src_string, const std::string& head_string);

    static bool CopyFileToPath(const QString& sourceDir, QString outputDir, bool coverFileIfExist, bool isPath = true);

    static bool CopyDirctoryToPath(const QString& sourceDir, const QString& outputDir, bool coverFileIfExist);

    static bool DeleteDir(const QString& path);

    static bool replace_first(std::string& str, const std::string& old_value, const std::string& new_value);
    static bool replace_all_distinct(std::string& str, const std::string& old_value, const std::string& new_value);
    static std::string to_string_nozero(float input);

    static std::vector<std::string> StringSplit(const std::string& str, std::string sep);

    template<typename _MapType>
    static auto get_map_key_value(const _MapType& input_map, const decltype(input_map.begin()->second)& mapped_value) -> decltype(input_map.begin()->first);        //Not Test yet

    static std::string GetSubDir(std::string dir);      //�����޶����򣬲����ƣ����忴����
    static std::string GetDirFileName(std::string dir);     //ͬ��

    template <class T>
    static void DeletePointer(T*& _pointer)
    {
        if (_pointer != NULL)
        {
            delete _pointer;
            _pointer = NULL;
        }
    }

#ifdef _WIN32
    static std::string StrToUtf(const std::string& str);
    static std::string UtfToStr(const std::string& str);

    static bool DeleteFolderA(std::string path);
    static bool CopyFolderA(std::string lpszFromPath, std::string lpszToPath);

    static bool CreatePath(const std::string& path, bool bReplaceIfExist);
#elif __linux__
#else
#   error "Unknown compiler"
#endif


};

#endif // FUNCTION_H
