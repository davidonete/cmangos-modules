#include "Module.h"
#include "ModuleMgr.h"
#include "ModuleConfig.h"

Module::Module(const std::string& name)
: config(nullptr)
{
    sModuleMgr.RegisterModule(this, name);
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