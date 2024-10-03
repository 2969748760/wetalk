/**
  ******************************************************************************
  * @File           : singleton.h
  * @Author         : 钟先生
  * @Brief          : XXX Function
  * @Date           : 2024/09/27
  ******************************************************************************
 */

#ifndef WETALK_SINGLETON_H
#define WETALK_SINGLETON_H

#include <global.h>

template<typename T>
class Singleton {
protected:
    Singleton() = default;

    Singleton(const Singleton<T> &) = delete;

    Singleton<T> &operator=(const Singleton<T> &) = delete;

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

#endif //WETALK_SINGLETON_H
