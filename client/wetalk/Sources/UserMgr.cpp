//
// Created by 29697 on 24-12-2.
//

#include "UserMgr.h"

UserMgr::~UserMgr() {
}

void UserMgr::SetName(QString name) {
    _name = name;
}

void UserMgr::SetUid(int uid) {
    _uid = uid;
}

void UserMgr::SetToken(QString token) {
    _token = token;
}

UserMgr::UserMgr() {

}




