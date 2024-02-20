#include "Module.h"
#include "ModuleMgr.h"
#include "ModuleConfig.h"

Module::Module()
: config(nullptr)
{
    sModuleMgr.RegisterModule(this);
}

void Module::Initialize()
{
    config = CreateConfig();
    config->Load();
    OnInitialize();
}