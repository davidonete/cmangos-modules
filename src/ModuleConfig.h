#ifndef MANGOS_MODULE_CONFIG_H
#define MANGOS_MODULE_CONFIG_H

#include "Config/Config.h"

class ModuleConfig
{
public:
    ModuleConfig(const std::string& filename);
    virtual ~ModuleConfig() {}

    bool Load();

protected:
    virtual bool OnLoad() = 0;

protected:
    std::string filename;
    Config config;
};

#endif