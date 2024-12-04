//
// Created by 29697 on 24-11-25.
//

#ifndef LOGICSYSTEM_H
#define LOGICSYSTEM_H
#include <functional>
#include <json/value.h>
#include "data.h"
#include "CSession.h"
#include "Singleton.h"

typedef std::function<void(std::shared_ptr<CSession>, const short &msg_id, const std::string &msg_data)> FunCallBack;

class LogicSystem : public Singleton<LogicSystem> {
    friend class Singleton<LogicSystem>;

public:
    ~LogicSystem();

    void PostMsgToQue(std::shared_ptr<LogicNode> msg);

private:
    LogicSystem();

    void DealMsg();

    void RegisterCallBacks();

    void LoginHandler(std::shared_ptr<CSession> session, const short &msg_id, const std::string &msg_data);

    void SearchInfo(std::shared_ptr<CSession> session, const short &msg_id, const std::string &msg_data);

    void AddFriendApply(std::shared_ptr<CSession> session, const short &msg_id, const std::string &msg_data);

    void AuthFriendApply(std::shared_ptr<CSession> session, const short &msg_id, const std::string &msg_data);

    void DealChatTextMsg(std::shared_ptr<CSession> session, const short &msg_id, const std::string &msg_data);

    bool isPureDigit(const std::string &str);

    void GetUserByUid(std::string uid_str, Json::Value &rtvalue);

    void GetUserByName(std::string name, Json::Value &rtvalue);

    bool GetBaseInfo(std::string base_key, int uid, std::shared_ptr<UserInfo> &userinfo);

    bool GetFriendApplyInfo(int to_uid, std::vector<std::shared_ptr<ApplyInfo> > &list);

    bool GetFriendList(int self_id, std::vector<std::shared_ptr<UserInfo> > &user_list);

    std::thread _worker_thread;
    std::queue<std::shared_ptr<LogicNode> > _msg_que;
    std::mutex _mutex;
    std::condition_variable _consume;
    bool _b_stop;
    std::map<short, FunCallBack> _fun_callbacks;
};


#endif //LOGICSYSTEM_H