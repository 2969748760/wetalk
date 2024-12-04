/**
  ******************************************************************************
  * @File           : Singleton.h
  * @Author         : 钟先生
  * @Brief          : XXX Function
  * @Date           : 2024/10/03
  ******************************************************************************
 */

#ifndef GATESERVER_SINGLETON_H
#define GATESERVER_SINGLETON_H

#include <memory>
#include <mutex>
#include <iostream>

template<typename T>
class Singleton {
protected:
    Singleton() = default;

    Singleton(const Singleton<T> &) = delete;

    Singleton &operator=(const Singleton<T> &st) = delete;

    static std::shared_ptr<T> _instance;
public:
    static std::shared_ptr<T> GetInstance() {
        static std::once_flag s_flag;
        std::call_once(s_flag, [&]() {
            _instance = std::shared_ptr<T>(new T);
        });

        return _instance;
    }

    void PrintAddress() {
        std::cout << _instance.get() << std::endl;
    }

    ~Singleton() {
        std::cout << "this is singleton destruct" << std::endl;
    }
};

template<typename T>
std::shared_ptr<T> Singleton<T>::_instance = nullptr;

#endif //GATESERVER_SINGLETON_H
