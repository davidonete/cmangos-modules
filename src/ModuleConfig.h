#ifndef CMANGOS_MODULE_CONFIG_H
#define CMANGOS_MODULE_CONFIG_H

#include "Config/Config.h"

namespace cmangos_modules
{
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
}

#endif