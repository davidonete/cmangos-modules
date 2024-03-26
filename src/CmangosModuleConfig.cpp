#include "CmangosModuleConfig.h"

#include "Log/Log.h"
#include "SystemConfig.h"

CmangosModuleConfig::CmangosModuleConfig(const std::string& filename)
: filename(filename)
{

}

bool CmangosModuleConfig::Load()
{
    if (config.SetSource(SYSCONFDIR + filename, ""))
    {
        return OnLoad();
    }
    else
    {
        sLog.outError(std::string("Failed to open configuration file " + filename).c_str());
        return false;
    }
}