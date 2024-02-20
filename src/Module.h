#ifndef MANGOS_MODULE_H
#define MANGOS_MODULE_H

#include "Config/Config.h"

class ModuleConfig
{
public:
    ModuleConfig(const std::string& filename);
    virtual ~ModuleConfig() {}

    bool Load();

protected:
    virtual bool OnLoad() { return true; }

private:
    std::string filename;
    Config config;
};

template <typename T>
class Module
{
public:
    Module(T moduleConfig);
    virtual ~Module() {}

    // Hooks
    // World
    virtual void OnWorldInitialized() {}

protected:
    T moduleConfig;
};

#endif