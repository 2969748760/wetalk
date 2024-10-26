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

private:
    MysqlManager();

    MysqlDao _dao;
};


#endif //MYSQLMANAGER_H
