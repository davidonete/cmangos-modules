#include "Module.h"
#include "ModuleMgr.h"
#include "ModuleConfig.h"

Module::Module()
: config(nullptr)
{
    sModuleMgr.RegisterModule(this);
}

Module::~Module()
{
    delete config;
    config = nullptr;
}

void Module::Initialize()
{
    config = CreateConfig();
    config->Load();
    OnInitialize();
}