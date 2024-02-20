#include "ModuleMgr.h"
#include "Modules.h"

ModuleMgr::~ModuleMgr()
{
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
