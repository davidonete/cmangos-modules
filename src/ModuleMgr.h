#ifndef MANGOS_MODULE_MGR_H
#define MANGOS_MODULE_MGR_H

#include <vector>

class Module;

class ModuleMgr
{
public:
    ModuleMgr() {}
    ~ModuleMgr();

    void RegisterModule(Module* module);

    // Hooks
    // World
    void OnWorldInitialized();

private:
    std::vector<Module*> modules;
};

#define sModuleMgr MaNGOS::Singleton<ModuleMgr>::Instance()
#endif