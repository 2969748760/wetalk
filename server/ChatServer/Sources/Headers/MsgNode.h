//
// Created by 29697 on 24-11-26.
//

#ifndef MSGNODE_H
#define MSGNODE_H
#include <boost/asio.hpp>

using namespace std;
using boost::asio::ip::tcp;
class LogicSystem;

class MsgNode {
public:
    explicit MsgNode(short max_len);

    ~MsgNode();

    void Clear();

    short _cur_len;
    short _total_len;
    char *_data;
};

class RecvNode : public MsgNode {
    friend class LogicSystem;

public:
    RecvNode(short max_len, short msg_id);

private:
    short _msg_id{};
};

class SendNode : public MsgNode {
    friend class LogicSystem;

public:
    SendNode(const char *msg, short max_len, short msg_id);

private:
    short _msg_id;
};


#endif //MSGNODE_H
