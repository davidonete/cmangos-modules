#include "ModuleMgr.h"
#include "Modules.h"

#include "Entities/ObjectGuid.h"
#include "Entities/Player.h"
#include "Entities/Unit.h"

ModuleMgr::~ModuleMgr()
{
    modules.clear();
}

void ModuleMgr::RegisterModule(Module* module, const std::string& name)
{
    auto it = modules.find(name);
    if (it == modules.end())
    {
        modules.insert(std::make_pair(name, module));
    }
}

void ModuleMgr::OnWorldInitialized()
{
    for (const auto& pair : modules)
    {
        Module* module = pair.second;
        module->Initialize();
        module->OnWorldInitialized();
    }
}

bool ModuleMgr::OnUseItem(Player* player, Item* item)
{
    for (const auto& pair : modules)
    {
        Module* module = pair.second;
        if (module->OnUseItem(player, item))
        {
            return true;
        }
    }

    return false;
}

bool ModuleMgr::OnGossipHello(Player* player, Creature* creature)
{
    for (const auto& pair : modules)
    {
        Module* module = pair.second;
        if (module->OnGossipHello(player, creature))
        {
            return true;
        }
    }

    return false;
}

bool ModuleMgr::OnGossipSelect(Player* player, const ObjectGuid& guid, uint32 sender, uint32 action, const std::string& code)
{
    if (player)
    {
        if (guid.IsAnyTypeCreature())
        {
            Creature* creature = player->GetNPCIfCanInteractWith(guid, UNIT_NPC_FLAG_NONE);
            if (creature)
            {
                for (const auto& pair : modules)
                {
                    Module* module = pair.second;
                    if (module->OnGossipSelect(player, creature, sender, action, code))
                    {
                        return true;
                    }
                }
            }
        }
        else if (guid.IsGameObject())
        {
            GameObject* gameObject = player->GetGameObjectIfCanInteractWith(guid);
            if (gameObject)
            {
                for (const auto& pair : modules)
                {
                    Module* module = pair.second;
                    if (module->OnGossipSelect(player, gameObject, sender, action, code))
                    {
                        return true;
                    }
                }
            }
        }
        else if (guid.IsItem())
        {
            Item* item = player->GetItemByGuid(guid);
            if (item)
            {
                for (const auto& pair : modules)
                {
                    Module* module = pair.second;
                    if (module->OnGossipSelect(player, item, sender, action, code))
                    {
                        return true;
                    }
                }
            }
        }
    }

    return false;
}

void ModuleMgr::OnLearnTalent(Player* player, uint32 spellId)
{
    for (const auto& pair : modules)
    {
        Module* module = pair.second;
        module->OnLearnTalent(player, spellId);
    }
}

void ModuleMgr::OnResetTalents(Player* player, uint32 cost)
{
    for (const auto& pair : modules)
    {
        Module* module = pair.second;
        module->OnResetTalents(player, cost);
    }
}

void ModuleMgr::OnPreLoadFromDB(uint32 playerId)
{
    for (const auto& pair : modules)
    {
        Module* module = pair.second;
        module->OnPreLoadFromDB(playerId);
    }
}

void ModuleMgr::OnLoadFromDB(Player* player)
{
    for (const auto& pair : modules)
    {
        Module* module = pair.second;
        module->OnLoadFromDB(player);
    }
}

void ModuleMgr::OnSaveToDB(Player* player)
{
    for (const auto& pair : modules)
    {
        Module* module = pair.second;
        module->OnSaveToDB(player);
    }
}

void ModuleMgr::OnDeleteFromDB(uint32 playerId)
{
    for (const auto& pair : modules)
    {
        Module* module = pair.second;
        module->OnDeleteFromDB(playerId);
    }
}

void ModuleMgr::OnLogOut(Player* player)
{
    for (const auto& pair : modules)
    {
        Module* module = pair.second;
        module->OnLogOut(player);
    }
}

void ModuleMgr::OnCharacterCreated(Player* player)
{
    for (const auto& pair : modules)
    {
        Module* module = pair.second;
        module->OnCharacterCreated(player);
    }
}

bool ModuleMgr::OnLoadActionButtons(Player* player, ActionButtonList& actionButtons)
{
    for (const auto& pair : modules)
    {
        Module* module = pair.second;
        if (module->OnLoadActionButtons(player, actionButtons))
        {
            return true;
        }
    }

    return false;
}

bool ModuleMgr::OnSaveActionButtons(Player* player, ActionButtonList& actionButtons)
{
    for (const auto& pair : modules)
    {
        Module* module = pair.second;
        if (module->OnSaveActionButtons(player, actionButtons))
        {
            return true;
        }
    }

    return false;
}
