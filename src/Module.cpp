#include "Module.h"
#include "ModuleMgr.h"
#include "ModuleConfig.h"

namespace cmangos_module
{
    Module::Module(const std::string& name, ModuleConfig* config)
    : config(config)
    , name(name)
    {
        sModuleMgr.RegisterModule(this);
    }

    Module::~Module()
    {
        if (config)
        {
            delete config;
            config = nullptr;
        }
    }

    void Module::LoadConfig()
    {
        if (config)
        {
            config->Load();
        }
    }

    void Module::Initialize()
    {
        sLog.outString("Initializing %s module", name.c_str());
        OnInitialize();
    }
}