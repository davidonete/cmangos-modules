#include "ModuleMgr.h"
#include "Modules.h"

Module::Module()
{
    sModuleMgr.RegisterModule(this);
}

Module::~Module()
{
    // Unregister module??
    int a = 0;
    a++;
}

ModuleMgr::~ModuleMgr()
{
    for (Module* module : modules)
    {
        delete module;
    }

    modules.clear();
}

void ModuleMgr::RegisterModule(Module* module)
{
    modules.push_back(module);
}

void ModuleMgr::OnWorldInitialized()
{
    for (Module* module : modules)
    {
        module->OnWorldInitialized();
    }
}
