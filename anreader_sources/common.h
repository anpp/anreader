#ifndef COMMON_H
#define COMMON_H

#include <map>
#include <QString>

typedef std::map<QString, QString> map_DataList;

#ifdef Q_OS_WIN64
#else
    #ifdef Q_OS_MACOS
    #else
        template <typename T>
        typename std::add_const<T>::type &qAsConst(T &t) {return t;} //для XP
    #endif
#endif


#endif // COMMON_H
