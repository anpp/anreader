#ifndef COMMON_H
#define COMMON_H

#include <map>
#include <QString>
#include <QtGlobal>

typedef std::map<QString, QString> map_DataList;

#if QT_VERSION <= QT_VERSION_CHECK(5, 6, 3)
    //для XP
template <typename T>
typename std::add_const<T>::type &qAsConst(T &t) {return t;}

template <typename... Args>
struct QNonConstOverload
{
    template <typename R, typename T>
    Q_DECL_CONSTEXPR auto operator()(R (T::*ptr)(Args...)) const noexcept -> decltype(ptr)
    { return ptr; }

    template <typename R, typename T>
    static Q_DECL_CONSTEXPR auto of(R (T::*ptr)(Args...)) noexcept -> decltype(ptr)
    { return ptr; }
};

template <typename... Args>
struct QConstOverload
{
    template <typename R, typename T>
    Q_DECL_CONSTEXPR auto operator()(R (T::*ptr)(Args...) const) const noexcept -> decltype(ptr)
    { return ptr; }

    template <typename R, typename T>
    static Q_DECL_CONSTEXPR auto of(R (T::*ptr)(Args...) const) noexcept -> decltype(ptr)
    { return ptr; }
};

template <typename... Args>
struct QOverload : QConstOverload<Args...>, QNonConstOverload<Args...>
{
    using QConstOverload<Args...>::of;
    using QConstOverload<Args...>::operator();
    using QNonConstOverload<Args...>::of;
    using QNonConstOverload<Args...>::operator();

    template <typename R>
    Q_DECL_CONSTEXPR auto operator()(R (*ptr)(Args...)) const noexcept -> decltype(ptr)
    { return ptr; }

    template <typename R>
    static Q_DECL_CONSTEXPR auto of(R (*ptr)(Args...)) noexcept -> decltype(ptr)
    { return ptr; }
};


#endif


#endif // COMMON_H
