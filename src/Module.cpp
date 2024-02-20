#include "Module.h"
#include "ModuleConfig.h"

Module::Module(ModuleConfig* config)
: config(nullptr)
{
    sModuleMgr.RegisterModule(this);
}

void Module::Initialize()
{
    config = CreateConfig();
    config->Load();
}