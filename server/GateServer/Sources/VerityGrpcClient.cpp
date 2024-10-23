//
// Created by 29697 on 24-10-19.
//

#include "Headers/VerityGrpcClient.h"
#include "Headers/ConfigManager.h"

VerifyGrpcClient::VerifyGrpcClient() {
    auto &g_config_manager = ConfigManager::GetInstance();
    std::string host = g_config_manager["VerifyServer"]["Host"];
    std::string port = g_config_manager["VerifyServer"]["Port"];
    pool_.reset(new RPCConnectPool(5, host, port));
}
