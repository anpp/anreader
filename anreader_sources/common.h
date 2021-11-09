#ifndef COMMON_H
#define COMMON_H

#include <map>
#include <unordered_map>
#include <QString>

#ifdef Q_OS_MACOS
    typedef std::map<QString, QString> map_DataList;
#else
    #ifdef Q_OS_WIN64
        typedef std::unordered_map<QString, QString> map_DataList;
    #else
        typedef std::map<QString, QString> map_DataList;
    #endif
#endif


#endif // COMMON_H
