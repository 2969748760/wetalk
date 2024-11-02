//
// Created by 29697 on 24-10-26.
//

#ifndef MYSQLMANAGER_H
#define MYSQLMANAGER_H
#include "MysqlDao.h"
#include "Singleton.h"


class MysqlManager : public Singleton<MysqlManager> {
    friend class Singleton<MysqlManager>;

public:
    ~MysqlManager();

    int RegUser(const std::string name, const std::string email, const std::string pwd);

    bool CheckEmail(const std::string &user, const std::string &email);

    bool UpdatePwd(const std::string &user, const std::string &pwd);

    bool CheckEmail(const std::string &email);

    bool CheckPwd(const std::string &email, const std::string &pwd, UserInfo &userInfo);

private:
    MysqlManager();

    MysqlDao _dao;
};


#endif //MYSQLMANAGER_H
