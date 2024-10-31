//
// Created by 29697 on 24-10-26.
//

#include "Headers/MysqlManager.h"

MysqlManager::~MysqlManager() {
}

int MysqlManager::RegUser(const std::string name, const std::string email, const std::string pwd) {
    return _dao.RegUser(name, email, pwd);
}

bool MysqlManager::CheckEmail(const std::string &user, const std::string &email) {
    return _dao.CheckEmail(user, email);
}

bool MysqlManager::UpdatePwd(const std::string &user, const std::string &pwd) {
    return _dao.UpdatePwd(user, pwd);
}

MysqlManager::MysqlManager() {

}
