#pragma once

#include <json/json.h>

#define Json_CheckAndGetMember(root, key)\
        if (root.isMember(#key)){\
            Json::Value _##key = root[#key];

#define Json_CheckFalse }else{\

#define Json_CheckEnd }

#define Json_BaseValueLoad(root,ParamText,asType,falseValue) \
            Json_CheckAndGetMember(root, ParamText)\
                ParamText = _##ParamText.asType();\
            Json_CheckFalse\
                ParamText = falseValue;\
            Json_CheckEnd



//使用中文写入json文件
namespace Json
{
    class BaseWriter : public Writer
    {
    public:
        BaseWriter() {}
        virtual ~BaseWriter() {}

    protected:
        static bool isControlCharacter(char ch);
        static bool containsControlCharacter(const char* str);
        static void uintToString(unsigned int value, char *&current);
        std::string valueToString(Int value);
        std::string valueToString(UInt value);
        std::string valueToString(double value);
        std::string valueToString(bool value);
        std::string valueToQuotedString(const char *value);

    };

    class StringWriter : public BaseWriter
    {
    public:
        StringWriter() {}
        virtual ~StringWriter() {}

    public: 
        // overridden from Writer
        /** \brief Serialize a Value in <a HREF="http://www.json.org">JSON</a> format.
        * \param root Value to serialize.
        * \return String containing the JSON document that represents the root value.
        */
        virtual std::string write(const Value &root);
    private:
        void writeValue(const Value &value);
        void writeArrayValue(const Value &value);
        bool isMultineArray(const Value &value);
        void pushValue(const std::string &value);
        void writeIndent();
        void writeWithIndent(const std::string &value);
        void indent();
        void unindent();
        void writeCommentBeforeValue(const Value &root);
        void writeCommentAfterValueOnSameLine(const Value &root);
        bool hasCommentForValue(const Value &value);
        static std::string normalizeEOL(const std::string &text);
        typedef std::vector<std::string> ChildValues;

        ChildValues childValues_;
        std::string document_;
        std::string indentString_;
        int rightMargin_ = 74;
        int indentSize_ = 3;
        bool addChildValues_;
    };
}
