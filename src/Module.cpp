#include "Module.h"

ModuleConfig::ModuleConfig(const std::string& filename)
, filename(filename)
{

}

bool ModuleConfig::Load()
{
    if (config.SetSource(SYSCONFDIR+filename))
    {
        return OnLoad();
    }
    else
    {
        sLog.outError("Failed to open configuration file dualspec.conf");
        return false;
    }
}

Module::Module(T moduleConfig)
: moduleConfig(moduleConfig)
{
    sModuleMgr.RegisterModule(this);
}

