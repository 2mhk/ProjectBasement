#ifndef _DEFINE_H_
#define _DEFINE_H_

#include <QDate>
#include <QTime>
#include <QLocale>
#include <QMetaType>
#include <QTextCodec>

//ʹ��
#define ENABLE_LOG_HEARTBREAK   0       //����Log

static const std::string VERSION = "0.0.1";

static const QDate BUILD_DATE = QLocale(QLocale::English).toDate(QString(__DATE__).replace("  ", " 0"), "MMM dd yyyy");
static const QTime BUILD_TIME = QTime::fromString(__TIME__, "hh:mm:ss");

static const QString PROJECT_NAME = QTextCodec::codecForLocale()->toUnicode("Project Name Space");

#define FILENAME_SETTINGS_JSON "Config/Setting.json"

//Q_DECLARE_METATYPE(cv::Mat);

//Codeֻ�ڵ�һ�ε�������Ч
#define ProcessOnlyFirstTime(Code)\
    {\
        static bool isFirst = true;\
        if (isFirst)\
                {\
            isFirst = false;\
            Code\
                }\
    }


#define ENABLE_PRINT_TIME_STAMP 1

#if ENABLE_PRINT_TIME_STAMP
#define PrintCurrentTime(TimeStamp)\
    {\
        time_t TimeStamp = clock();\
        std::cout << #TimeStamp << " : "<< TimeStamp << " ms " << __FUNCDNAME__ << endl;\
    }
#else
#define PrintCurrentTime(TimeStamp)
#endif

#define SetMutex \
    static std::mutex _TmpMutex_SetMutex;\
    std::lock_guard<std::mutex> guard(_TmpMutex_SetMutex);

#define MutexGuard(_mutex) std::lock_guard<std::mutex> _LockGuard_MutexGuard(_mutex);

#if ENABLE_LOG_HEARTBREAK
#define LogHeartBreak qInfo() << "function: " << __FILE__ << " line:" << __LINE__;
#else
#define LogHeartBreak
#endif


#endif	_DEFINE_H_
