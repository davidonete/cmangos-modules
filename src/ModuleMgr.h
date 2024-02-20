#ifndef MANGOS_MODULES_MGR_H
#define MANGOS_MODULES_MGR_H

#include <vector>

class Module
{
public:
    Module();
    virtual ~Module();

    virtual void OnWorldInitialized() {}
};

class ModuleMgr
{
public:
    ModuleMgr() {}
    ~ModuleMgr();

    void RegisterModule(Module* module);

    // Callbacks
    // World
    void OnWorldInitialized();

private:
    std::vector<Module*> modules;
};

#define sModuleMgr MaNGOS::Singleton<ModuleMgr>::Instance()

#include "Modules.h"
#endif