//
// Created by 29697 on 24-10-24.
//

#include "Headers/RedisManager.h"

#include "Headers/ConfigManager.h"


RedisManager::RedisManager() {
    auto &g_config_manager = ConfigManager::GetInstance();
    auto host = g_config_manager["Redis"]["Host"];
    auto port = g_config_manager["Redis"]["Port"];
    auto pwd = g_config_manager["Redis"]["Passwd"];
    _connection_pool.reset(new RedisConnectionPool(5, host.c_str(), std::atoi(port.c_str()), pwd.c_str()));
}

RedisManager::~RedisManager() {
    Close();
}

bool RedisManager::Get(const std::string &key, std::string &value) {
    auto connection = _connection_pool->getConnection();
    if (connection == nullptr) {
        return false;
    }
    auto reply = (redisReply *) redisCommand(connection, "GET %s", key.c_str());
    if (reply == nullptr) {
        std::cout << "[ GET  " << key << " ] failed" << std::endl;
        freeReplyObject(reply);
        return false;
    }

    if (reply->type != REDIS_REPLY_STRING) {
        std::cout << "[ GET  " << key << " ] failed" << std::endl;
        freeReplyObject(reply);
        return false;
    }

    value = reply->str;
    freeReplyObject(reply);

    std::cout << "Succeed to execute command [ GET " << key << "  ]" << std::endl;
    return true;
}

bool RedisManager::Set(const std::string &key, const std::string &value) {
    //执行redis命令行
    auto connection = _connection_pool->getConnection();
    if (connection == nullptr) {
        return false;
    }
    auto reply = (redisReply *) redisCommand(connection, "SET %s %s", key.c_str(), value.c_str());

    //如果返回NULL则说明执行失败
    if (nullptr == reply) {
        std::cout << "Execute command [ SET " << key << "  " << value << " ] failure ! " << std::endl;
        freeReplyObject(reply);
        return false;
    }

    //如果执行失败则释放连接
    if (!(reply->type == REDIS_REPLY_STATUS && (
              strcmp(reply->str, "OK") == 0 || strcmp(reply->str, "ok") == 0))) {
        std::cout << "Execute command [ SET " << key << "  " << value << " ] failure ! " << std::endl;
        freeReplyObject(reply);
        return false;
    }

    //执行成功 释放redisCommand执行后返回的redisReply所占用的内存
    freeReplyObject(reply);
    std::cout << "Execute command [ SET " << key << "  " << value << " ] success ! " << std::endl;
    return true;
}

bool RedisManager::Auth(const std::string &password) {
    auto connection = _connection_pool->getConnection();
    if (connection == nullptr) {
        return false;
    }
    auto reply = (redisReply *) redisCommand(connection, "AUTH %s", password.c_str());
    if (reply->type == REDIS_REPLY_ERROR) {
        std::cout << "认证失败" << std::endl;
        //执行成功 释放redisCommand执行后返回的redisReply所占用的内存
        freeReplyObject(reply);
        return false;
    } else {
        //执行成功 释放redisCommand执行后返回的redisReply所占用的内存
        freeReplyObject(reply);
        std::cout << "认证成功" << std::endl;
        return true;
    }
}

bool RedisManager::LPush(const std::string &key, const std::string &value) {
    auto connection = _connection_pool->getConnection();
    if (connection == nullptr) {
        return false;
    }
    auto reply = (redisReply *) redisCommand(connection, "LPUSH %s %s", key.c_str(), value.c_str());
    if (nullptr == reply) {
        std::cout << "Execute command [ LPUSH " << key << "  " << value << " ] failure ! " << std::endl;
        freeReplyObject(reply);
        return false;
    }

    if (reply->type != REDIS_REPLY_INTEGER || reply->integer <= 0) {
        std::cout << "Execute command [ LPUSH " << key << "  " << value << " ] failure ! " << std::endl;
        freeReplyObject(reply);
        return false;
    }

    std::cout << "Execute command [ LPUSH " << key << "  " << value << " ] success ! " << std::endl;
    freeReplyObject(reply);
    return true;
}

bool RedisManager::LPop(const std::string &key, std::string &value) {
    auto connection = _connection_pool->getConnection();
    if (connection == nullptr) {
        return false;
    }
    auto reply = (redisReply *) redisCommand(connection, "LPOP %s ", key.c_str());
    if (reply == nullptr || reply->type == REDIS_REPLY_NIL) {
        std::cout << "Execute command [ LPOP " << key << " ] failure ! " << std::endl;
        freeReplyObject(reply);
        return false;
    }
    value = reply->str;
    std::cout << "Execute command [ LPOP " << key << " ] success ! " << std::endl;
    freeReplyObject(reply);
    return true;
}

bool RedisManager::RPush(const std::string &key, const std::string &value) {
    auto connection = _connection_pool->getConnection();
    if (connection == nullptr) {
        return false;
    }
    auto reply = (redisReply *) redisCommand(connection, "RPUSH %s %s", key.c_str(), value.c_str());
    if (nullptr == reply) {
        std::cout << "Execute command [ RPUSH " << key << "  " << value << " ] failure ! " << std::endl;
        freeReplyObject(reply);
        return false;
    }

    if (reply->type != REDIS_REPLY_INTEGER || reply->integer <= 0) {
        std::cout << "Execute command [ RPUSH " << key << "  " << value << " ] failure ! " << std::endl;
        freeReplyObject(reply);
        return false;
    }

    std::cout << "Execute command [ RPUSH " << key << "  " << value << " ] success ! " << std::endl;
    freeReplyObject(reply);
    return true;
}

bool RedisManager::RPop(const std::string &key, std::string &value) {
    auto connection = _connection_pool->getConnection();
    if (connection == nullptr) {
        return false;
    }
    auto reply = (redisReply *) redisCommand(connection, "RPOP %s ", key.c_str());
    if (reply == nullptr || reply->type == REDIS_REPLY_NIL) {
        std::cout << "Execute command [ RPOP " << key << " ] failure ! " << std::endl;
        freeReplyObject(reply);
        return false;
    }
    value = reply->str;
    std::cout << "Execute command [ RPOP " << key << " ] success ! " << std::endl;
    freeReplyObject(reply);
    return true;
}

bool RedisManager::HSet(const std::string &key, const std::string &hkey, const std::string &value) {
    auto connection = _connection_pool->getConnection();
    if (connection == nullptr) {
        return false;
    }
    auto reply = (redisReply *) redisCommand(connection, "HSET %s %s %s", key.c_str(), hkey.c_str(),
                                        value.c_str());
    if (reply == nullptr || reply->type != REDIS_REPLY_INTEGER) {
        std::cout << "Execute command [ HSet " << key << "  " << hkey << "  " << value << " ] failure ! " << std::endl;
        freeReplyObject(reply);
        return false;
    }
    std::cout << "Execute command [ HSet " << key << "  " << hkey << "  " << value << " ] success ! " << std::endl;
    freeReplyObject(reply);
    return true;
}


bool RedisManager::HSet(const char *key, const char *hkey, const char *hvalue, size_t hvaluelen) {
    auto connection = _connection_pool->getConnection();
    if (connection == nullptr) {
        return false;
    }
    const char *argv[4];
    size_t argvlen[4];
    argv[0] = "HSET";
    argvlen[0] = 4;
    argv[1] = key;
    argvlen[1] = strlen(key);
    argv[2] = hkey;
    argvlen[2] = strlen(hkey);
    argv[3] = hvalue;
    argvlen[3] = hvaluelen;
    auto reply = (redisReply *) redisCommandArgv(connection, 4, argv, argvlen);
    if (reply == nullptr || reply->type != REDIS_REPLY_INTEGER) {
        std::cout << "Execute command [ HSet " << key << "  " << hkey << "  " << hvalue << " ] failure ! " << std::endl;
        freeReplyObject(reply);
        return false;
    }
    std::cout << "Execute command [ HSet " << key << "  " << hkey << "  " << hvalue << " ] success ! " << std::endl;
    freeReplyObject(reply);
    return true;
}

std::string RedisManager::HGet(const std::string &key, const std::string &hkey) {
    auto connection = _connection_pool->getConnection();
    if (connection == nullptr) {
        return "";
    }
    const char *argv[3];
    size_t argvlen[3];
    argv[0] = "HGET";
    argvlen[0] = 4;
    argv[1] = key.c_str();
    argvlen[1] = key.length();
    argv[2] = hkey.c_str();
    argvlen[2] = hkey.length();
    auto reply = (redisReply *) redisCommandArgv(connection, 3, argv, argvlen);
    if (reply == nullptr || reply->type == REDIS_REPLY_NIL) {
        freeReplyObject(reply);
        std::cout << "Execute command [ HGet " << key << " " << hkey << "  ] failure ! " << std::endl;
        return "";
    }

    std::string value = reply->str;
    freeReplyObject(reply);
    std::cout << "Execute command [ HGet " << key << " " << hkey << " ] success ! " << std::endl;
    return value;
}

bool RedisManager::Del(const std::string &key) {
    auto connection = _connection_pool->getConnection();
    if (connection == nullptr) {
        return false;
    }
    auto reply = (redisReply *) redisCommand(connection, "DEL %s", key.c_str());
    if (reply == nullptr || reply->type != REDIS_REPLY_INTEGER) {
        std::cout << "Execute command [ Del " << key << " ] failure ! " << std::endl;
        freeReplyObject(reply);
        return false;
    }
    std::cout << "Execute command [ Del " << key << " ] success ! " << std::endl;
    freeReplyObject(reply);
    return true;
}

bool RedisManager::ExistsKey(const std::string &key) {
    auto connection = _connection_pool->getConnection();
    if (connection == nullptr) {
        return false;
    }
    auto reply = (redisReply *) redisCommand(connection, "exists %s", key.c_str());
    if (reply == nullptr || reply->type != REDIS_REPLY_INTEGER || reply->integer == 0) {
        std::cout << "Not Found [ Key " << key << " ]  ! " << std::endl;
        freeReplyObject(reply);
        return false;
    }
    std::cout << " Found [ Key " << key << " ] exists ! " << std::endl;
    freeReplyObject(reply);
    return true;
}

void RedisManager::Close() {
    _connection_pool->Close();
}
