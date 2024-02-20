#include "ModuleMgr.h"
#include "Modules.h"

#include "Entities/ObjectGuid.h"
#include "Entities/Player.h"
#include "Entities/Unit.h"

ModuleMgr::~ModuleMgr()
{
    modules.clear();
}

void ModuleMgr::RegisterModule(Module* module)
{
    modules.push_back(module);
}

void ModuleMgr::OnWorldInitialized()
{
    for (Module* module : modules)
    {
        module->Initialize();
        module->OnWorldInitialized();
    }
}

bool ModuleMgr::OnUseItem(Player* player, Item* item)
{
    for (Module* module : modules)
    {
        if (module->OnUseItem(player, item))
        {
            return true;
        }
    }

    return false;
}

bool ModuleMgr::OnGossipHello(Player* player, Creature* creature)
{
    for (Module* module : modules)
    {
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
                for (Module* module : modules)
                {
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
                for (Module* module : modules)
                {
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
                for (Module* module : modules)
                {
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
    for (Module* module : modules)
    {
        module->OnLearnTalent(player, spellId);
    }
}

void ModuleMgr::OnResetTalents(Player* player, uint32 cost)
{
    for (Module* module : modules)
    {
        module->OnResetTalents(player, cost);
    }
}

void ModuleMgr::OnPreLoadFromDB(uint32 playerId)
{
    for (Module* module : modules)
    {
        module->OnPreLoadFromDB(playerId);
    }
}

void ModuleMgr::OnLoadFromDB(Player* player)
{
    for (Module* module : modules)
    {
        module->OnLoadFromDB(player);
    }
}

void ModuleMgr::OnSaveToDB(Player* player)
{
    for (Module* module : modules)
    {
        module->OnSaveToDB(player);
    }
}

void ModuleMgr::OnLogOut(Player* player)
{
    for (Module* module : modules)
    {
        module->OnLogOut(player);
    }
}

void ModuleMgr::OnCharacterCreated(Player* player)
{
    for (Module* module : modules)
    {
        module->OnCharacterCreated(player);
    }
}

void ModuleMgr::OnCharacterDeleted(uint32 playerId)
{
    for (Module* module : modules)
    {
        module->OnCharacterDeleted(playerId);
    }
}

bool ModuleMgr::OnLoadActionButtons(Player* player, ActionButtonList& actionButtons)
{
    for (Module* module : modules)
    {
        if (module->OnLoadActionButtons(player, actionButtons))
        {
            return true;
        }
    }

    return false;
}

bool ModuleMgr::OnSaveActionButtons(Player* player, ActionButtonList& actionButtons)
{
    for (Module* module : modules)
    {
        if (module->OnSaveActionButtons(player, actionButtons))
        {
            return true;
        }
    }

    return false;
}
