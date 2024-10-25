// GateServer.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <RedisManager.h>
#include <json/json.h>
#include <json/value.h>
#include <json/reader.h>

#include "Headers/ConfigManager.h"
#include "Headers/const.h"
#include "Headers/CServer.h"

void TestRedisMgr() {
    assert(RedisManager::GetInstance()->Set("blogwebsite","llfc.club"));
    std::string value = "";
    assert(RedisManager::GetInstance()->Get("blogwebsite", value));
    assert(RedisManager::GetInstance()->Get("nonekey", value) == false);
    assert(RedisManager::GetInstance()->HSet("bloginfo","blogwebsite", "llfc.club"));
    assert(RedisManager::GetInstance()->HGet("bloginfo","blogwebsite") != "");
    assert(RedisManager::GetInstance()->ExistsKey("bloginfo"));
    assert(RedisManager::GetInstance()->Del("bloginfo"));
    assert(RedisManager::GetInstance()->Del("bloginfo"));
    assert(RedisManager::GetInstance()->ExistsKey("bloginfo") == false);
    assert(RedisManager::GetInstance()->LPush("lpushkey1", "lpushvalue1"));
    assert(RedisManager::GetInstance()->LPush("lpushkey1", "lpushvalue2"));
    assert(RedisManager::GetInstance()->LPush("lpushkey1", "lpushvalue3"));
    assert(RedisManager::GetInstance()->RPop("lpushkey1", value));
    assert(RedisManager::GetInstance()->RPop("lpushkey1", value));
    assert(RedisManager::GetInstance()->LPop("lpushkey1", value));
    assert(RedisManager::GetInstance()->LPop("lpushkey2", value)==false);
}

void TestRedis() {
    //连接redis 需要启动才可以进行连接
    //redis默认监听端口为6387 可以再配置文件中修改
    redisContext *c = redisConnect("122.51.188.96", 6380);
    if (c->err) {
        printf("Connect to redisServer faile:%s\n", c->errstr);
        redisFree(c);
        return;
    }
    printf("Connect to redisServer Success\n");

    std::string redis_password = "123456";
    redisReply *r = (redisReply *) redisCommand(c, ("auth " + redis_password).c_str());
    if (r->type == REDIS_REPLY_ERROR) {
        printf("Redis认证失败！\n");
    } else {
        printf("Redis认证成功！\n");
    }

    //为redis设置key
    const char *command1 = "set stest1 value1";

    //执行redis命令行
    r = (redisReply *) redisCommand(c, command1);

    //如果返回NULL则说明执行失败
    if (NULL == r) {
        printf("Execut command1 failure\n");
        redisFree(c);
        return;
    }

    //如果执行失败则释放连接
    if (!(r->type == REDIS_REPLY_STATUS && (strcmp(r->str, "OK") == 0 || strcmp(r->str, "ok") == 0))) {
        printf("Failed to execute command[%s]\n", command1);
        freeReplyObject(r);
        redisFree(c);
        return;
    }

    //执行成功 释放redisCommand执行后返回的redisReply所占用的内存
    freeReplyObject(r);
    printf("Succeed to execute command[%s]\n", command1);

    const char *command2 = "strlen stest1";
    r = (redisReply *) redisCommand(c, command2);

    //如果返回类型不是整形 则释放连接
    if (r->type != REDIS_REPLY_INTEGER) {
        printf("Failed to execute command[%s]\n", command2);
        freeReplyObject(r);
        redisFree(c);
        return;
    }

    //获取字符串长度
    int length = r->integer;
    freeReplyObject(r);
    printf("The length of 'stest1' is %d.\n", length);
    printf("Succeed to execute command[%s]\n", command2);

    //获取redis键值对信息
    const char *command3 = "get stest1";
    r = (redisReply *) redisCommand(c, command3);
    if (r->type != REDIS_REPLY_STRING) {
        printf("Failed to execute command[%s]\n", command3);
        freeReplyObject(r);
        redisFree(c);
        return;
    }
    printf("The value of 'stest1' is %s\n", r->str);
    freeReplyObject(r);
    printf("Succeed to execute command[%s]\n", command3);

    const char *command4 = "get stest2";
    r = (redisReply *) redisCommand(c, command4);
    if (r->type != REDIS_REPLY_NIL) {
        printf("Failed to execute command[%s]\n", command4);
        freeReplyObject(r);
        redisFree(c);
        return;
    }
    freeReplyObject(r);
    printf("Succeed to execute command[%s]\n", command4);

    //释放连接资源
    redisFree(c);
}

int main() {
    // TestRedis();
    // TestRedisMgr();
    auto &g_config_manager = ConfigManager::GetInstance();
    std::string gate_port_str = g_config_manager["GateServer"]["port"];
    unsigned short gate_port = atoi(gate_port_str.c_str());
    try {
        unsigned short port = static_cast<unsigned short>(8080);
        net::io_context ioc{1};
        boost::asio::signal_set signals(ioc, SIGINT, SIGTERM);
        signals.async_wait([&ioc](const boost::system::error_code &error, int signal_number) {
            if (error) {
                return;
            }
            ioc.stop();
        });
        std::make_shared<CServer>(ioc, port)->Start();
        std::cout << "Gate Server listen on port: " << port << std::endl;
        ioc.run();
    } catch (std::exception const &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << "Gate Server stopped" << std::endl;
    return 0;
}
