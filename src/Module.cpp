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
            return player && IsAlliance(player->getRace());
        }

        bool InPvpMap(Player* player)
        {
            if (player)
            {
                if (player->InBattleGround())
                    return true;

#if EXPANSION > 0
                if (player->InArena())
                    return true;
#endif
            }

            return false;
        }

        bool InDungeon(Player* player)
        {
            if (player && player->IsInWorld())
            {
                if (!player->IsBeingTeleported())
                {
                    if (const Map* map = player->GetMap())
                    {
                        return map->IsDungeon();
                    }
                }
            }

            return false;
        }

        bool InRaid(Player* player)
        {
            if (player && player->IsInWorld())
            {
                if (!player->IsBeingTeleported())
                {
                    if (const Map* map = player->GetMap())
                    {
                        return map->IsRaid();
                    }
                }
            }

            return false;
        }

        void ForEachItemInternal(const Player* player, uint8 start, uint8 end, std::function<void(Item*)> callback)
        {
            if (player)
            {
                for (uint8 i = start; i < end; ++i)
                {
                    if (start == INVENTORY_SLOT_BAG_START || start == BANK_SLOT_BAG_START)
                    {
                        if (Bag* bag = (Bag*)player->GetItemByPos(INVENTORY_SLOT_BAG_0, i))
                        {
                            for (uint32 j = 0; j < bag->GetBagSize(); ++j)
                            {
                                if (Item* item = bag->GetItemByPos(j))
                                {
                                    callback(item);
                                }
                            }
                        }
                    }
                    else
                    {
                        if (Item* item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, i))
                        {
                            callback(item);
                        }
                    }
                }
            }
        }

        void ForEachEquippedItem(const Player* player, std::function<void(Item*)> callback)
        {
            ForEachItemInternal(player, EQUIPMENT_SLOT_START, EQUIPMENT_SLOT_END, callback);
        }

        void ForEachInventoryItem(const Player* player, std::function<void(Item*)> callback)
        {
            ForEachItemInternal(player, INVENTORY_SLOT_BAG_START, INVENTORY_SLOT_BAG_END, callback);
        }

        void ForEachKeyItem(const Player* player, std::function<void(Item*)> callback)
        {
            ForEachItemInternal(player, KEYRING_SLOT_START, KEYRING_SLOT_END, callback);
        }

        void ForEachBankItem(const Player* player, std::function<void(Item*)> callback)
        {
            ForEachItemInternal(player, BANK_SLOT_ITEM_START, BANK_SLOT_ITEM_END, callback);
            ForEachItemInternal(player, BANK_SLOT_BAG_START, BANK_SLOT_BAG_END, callback);
        }

        void ForEachItem(const Player* player, std::function<void(Item*)> callback)
        {
            ForEachEquippedItem(player, callback);
            ForEachInventoryItem(player, callback);
            ForEachKeyItem(player, callback);
            ForEachBankItem(player, callback);
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