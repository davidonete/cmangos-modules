#ifndef MANGOS_MODULE_CONFIG_H
#define MANGOS_MODULE_CONFIG_H

#include "Config/Config.h"

class CmangosModuleConfig
{
public:
    CmangosModuleConfig(const std::string& filename);
    virtual ~CmangosModuleConfig() {}

    bool Load();

protected:
    virtual bool OnLoad() = 0;

protected:
    std::string filename;
    Config config;
};

#endif