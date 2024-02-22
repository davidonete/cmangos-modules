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

void ModuleMgr::OnWorldUpdated(uint32 elapsed)
{
    for (const auto& pair : modules)
    {
        Module* module = pair.second;
        module->OnUpdate(elapsed);
        module->OnWorldUpdated(elapsed);
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

bool ModuleMgr::OnPreGossipHello(Player* player, const ObjectGuid& guid)
{
    bool override = false;
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
                    if (module->OnPreGossipHello(player, creature))
                    {
                        override = true;
                        break;
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
                    if (module->OnPreGossipHello(player, gameObject))
                    {
                        override = true;
                        break;
                    }
                }
            }
        }
    }

    if (override)
    {
        OnGossipHello(player, guid);
    }

    return override;
}

bool ModuleMgr::OnPreGossipSelect(Player* player, const ObjectGuid& guid, uint32 sender, uint32 action, const std::string& code, uint32 gossipListId)
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
                    if (module->OnPreGossipSelect(player, creature, sender, action, code, gossipListId))
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
                    if (module->OnPreGossipSelect(player, gameObject, sender, action, code, gossipListId))
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
                    if (module->OnPreGossipSelect(player, item, sender, action, code, gossipListId))
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

bool ModuleMgr::OnHandleFall(Player* player, const MovementInfo& movementInfo, float lastFallZ)
{
    for (const auto& pair : modules)
    {
        Module* module = pair.second;
        if (module->OnHandleFall(player, movementInfo, lastFallZ))
        {
            return true;
        }
    }

    return false;
}

void ModuleMgr::OnResurrect(Player* player)
{
    for (const auto& pair : modules)
    {
        Module* module = pair.second;
        module->OnResurrect(player);
    }
}

void ModuleMgr::OnGiveXP(Player* player, uint32 xp, Creature* victim)
{
    for (const auto& pair : modules)
    {
        Module* module = pair.second;
        module->OnGiveXP(player, xp, victim);
    }
}

void ModuleMgr::OnGiveLevel(Player* player, uint32 level)
{
    for (const auto& pair : modules)
    {
        Module* module = pair.second;
        module->OnGiveLevel(player, level);
    }
}

void ModuleMgr::OnModifyMoney(Player* player, int32 diff)
{
    for (const auto& pair : modules)
    {
        Module* module = pair.second;
        module->OnModifyMoney(player, diff);
    }
}

void ModuleMgr::OnSetReputation(Player* player, const FactionEntry* factionEntry, int32 standing, bool incremental)
{
    for (const auto& pair : modules)
    {
        Module* module = pair.second;
        module->OnSetReputation(player, factionEntry, standing, incremental);
    }
}

void ModuleMgr::OnRewardQuest(Player* player, const Quest* quest)
{
    for (const auto& pair : modules)
    {
        Module* module = pair.second;
        module->OnRewardQuest(player, quest);
    }
}

void ModuleMgr::OnGetPlayerLevelInfo(Player* player, PlayerLevelInfo& info)
{
    for (const auto& pair : modules)
    {
        Module* module = pair.second;
        module->OnGetPlayerLevelInfo(player, info);
    }
}

bool ModuleMgr::OnRespawn(Creature* creature, time_t& respawnTime)
{
    for (const auto& pair : modules)
    {
        Module* module = pair.second;
        if (module->OnRespawn(creature, respawnTime))
        {
            return true;
        }
    }

    return false;
}

void ModuleMgr::OnRespawnRequest(Creature* creature)
{
    for (const auto& pair : modules)
    {
        Module* module = pair.second;
        module->OnRespawnRequest(creature);
    }
}

bool ModuleMgr::OnUseFishingNode(GameObject* gameObject, Player* player)
{
    for (const auto& pair : modules)
    {
        Module* module = pair.second;
        if (module->OnUseFishingNode(gameObject, player))
        {
            return true;
        }
    }

    return false;
}

bool ModuleMgr::OnCalculateEffectiveDodgeChance(const Unit* unit, const Unit* attacker, uint8 attType, const SpellEntry* ability, float& outChance)
{
    for (const auto& pair : modules)
    {
        Module* module = pair.second;
        if (module->OnCalculateEffectiveDodgeChance(unit, attacker, attType, ability, outChance))
        {
            return true;
        }
    }

    return false;
}

bool ModuleMgr::OnCalculateEffectiveBlockChance(const Unit* unit, const Unit* attacker, uint8 attType, const SpellEntry* ability, float& outChance)
{
    for (const auto& pair : modules)
    {
        Module* module = pair.second;
        if (module->OnCalculateEffectiveBlockChance(unit, attacker, attType, ability, outChance))
        {
            return true;
        }
    }

    return false;
}

bool ModuleMgr::OnCalculateEffectiveParryChance(const Unit* unit, const Unit* attacker, uint8 attType, const SpellEntry* ability, float& outChance)
{
    for (const auto& pair : modules)
    {
        Module* module = pair.second;
        if (module->OnCalculateEffectiveParryChance(unit, attacker, attType, ability, outChance))
        {
            return true;
        }
    }

    return false;
}

bool ModuleMgr::OnCalculateEffectiveCritChance(const Unit* unit, const Unit* victim, uint8 attType, const SpellEntry* ability, float& outChance)
{
    for (const auto& pair : modules)
    {
        Module* module = pair.second;
        if (module->OnCalculateEffectiveCritChance(unit, victim, attType, ability, outChance))
        {
            return true;
        }
    }

    return false;
}

bool ModuleMgr::OnCalculateEffectiveMissChance(const Unit* unit, const Unit* victim, uint8 attType, const SpellEntry* ability, const Spell* const* currentSpells, const SpellPartialResistDistribution& spellPartialResistDistribution, float& outChance)
{
    for (const auto& pair : modules)
    {
        Module* module = pair.second;
        if (module->OnCalculateEffectiveMissChance(unit, victim, attType, ability, currentSpells, spellPartialResistDistribution, outChance))
        {
            return true;
        }
    }

    return false;
}

bool ModuleMgr::OnCalculateSpellMissChance(const Unit* unit, const Unit* victim, uint32 schoolMask, const SpellEntry* spell, float& outChance)
{
    for (const auto& pair : modules)
    {
        Module* module = pair.second;
        if (module->OnCalculateSpellMissChance(unit, victim, schoolMask, spell, outChance))
        {
            return true;
        }
    }

    return false;
}

bool ModuleMgr::OnGetAttackDistance(const Unit* unit, const Unit* target, float& outDistance)
{
    for (const auto& pair : modules)
    {
        Module* module = pair.second;
        if (module->OnGetAttackDistance(unit, target, outDistance))
        {
            return true;
        }
    }

    return false;
}
