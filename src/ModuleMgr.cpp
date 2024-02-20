#include "ModuleMgr.h"
#include "Modules.h"

ModuleMgr::~ModuleMgr()
{
    for (Module* module : modules)
    {
        delete module;
        module = nullptr;
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
        module->Initialize();
        module->OnWorldInitialized();
    }
}
