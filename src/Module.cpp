#include "Module.h"
#include "ModuleMgr.h"
#include "ModuleConfig.h"

#include "Entities/Player.h"
#include "World/World.h"

namespace cmangos_module
{
    namespace helper
    {
        std::string FormatString(const char* format, ...)
        {
            va_list ap;
            char out[2048];
            va_start(ap, format);
            vsnprintf(out, 2048, format, ap);
            va_end(ap);
            return std::string(out);
        }

        bool IsMaxLevel(Player* player)
        {
            return player && player->GetLevel() >= sWorld.getConfig(CONFIG_UINT32_MAX_PLAYER_LEVEL);
        }

        bool IsAlliance(uint8 race)
        {
            return race == RACE_HUMAN ||
                   race == RACE_DWARF ||
                   race == RACE_NIGHTELF ||
#if EXPANSION > 0
                   race == RACE_DRAENEI ||
#endif
                   race == RACE_GNOME;
        }

        bool IsAlliance(Player* player)
        {
            return IsAlliance(player->getRace());
        }
    }

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