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

void ModuleMgr::OnWorldPreInitialized()
{
    for (const auto& pair : modules)
    {
        Module* module = pair.second;
        module->LoadConfig();
        module->OnWorldPreInitialized();
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
                    if (module->OnPreGossipHello(player, gameObject))
                    {
                        return true;
                    }
                }
            }
        }
    }

    return false;
}

void ModuleMgr::OnGossipHello(Player* player, const ObjectGuid& guid)
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
                    module->OnGossipHello(player, creature);
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
                    module->OnGossipHello(player, gameObject);
                }
            }
        }
    }
}

bool ModuleMgr::OnGossipSelect(Player* player, const ObjectGuid& guid, uint32 sender, uint32 action, const std::string& code, uint32 gossipListId)
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
                    if (module->OnGossipSelect(player, creature, sender, action, code, gossipListId))
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
                    if (module->OnGossipSelect(player, gameObject, sender, action, code, gossipListId))
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
                    if (module->OnGossipSelect(player, item, sender, action, code, gossipListId))
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

void ModuleMgr::OnPreLoadFromDB(Player* player)
{
    if (player)
    {
        const uint32 playerId = player->GetObjectGuid().GetCounter();
        for (const auto& pair : modules)
        {
            Module* module = pair.second;
            module->OnPreLoadFromDB(player);
            module->OnPreLoadFromDB(playerId);
        }
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

void ModuleMgr::OnPreCharacterCreated(Player* player)
{
    for (const auto& pair : modules)
    {
        Module* module = pair.second;
        module->OnPreCharacterCreated(player);
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

bool ModuleMgr::OnPreResurrect(Player* player)
{
    for (const auto& pair : modules)
    {
        Module* module = pair.second;
        if (module->OnPreResurrect(player))
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

void ModuleMgr::OnReleaseSpirit(Player* player, const WorldSafeLocsEntry* closestGrave)
{
    for (const auto& pair : modules)
    {
        Module* module = pair.second;
        module->OnReleaseSpirit(player, closestGrave);
    }
}

void ModuleMgr::OnDeath(Player* player, Unit* killer)
{
    for (const auto& pair : modules)
    {
        Module* module = pair.second;
        module->OnDeath(player, killer);
    }
}

void ModuleMgr::OnDeath(Player* player, uint8 environmentalDamageType)
{
    for (const auto& pair : modules)
    {
        Module* module = pair.second;
        module->OnDeath(player, environmentalDamageType);
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

void ModuleMgr::OnSetVisibleItemSlot(Player* player, uint8 slot, Item* item)
{
    for (const auto& pair : modules)
    {
        Module* module = pair.second;
        module->OnSetVisibleItemSlot(player, slot, item);
    }
}

void ModuleMgr::OnMoveItemFromInventory(Player* player, Item* item)
{
    for (const auto& pair : modules)
    {
        Module* module = pair.second;
        module->OnMoveItemFromInventory(player, item);
    }
}

void ModuleMgr::OnMoveItemToInventory(Player* player, Item* item)
{
    for (const auto& pair : modules)
    {
        Module* module = pair.second;
        module->OnMoveItemToInventory(player, item);
    }
}

void ModuleMgr::OnStoreNewItem(Player* player, Loot* loot, Item* item)
{
    for (const auto& pair : modules)
    {
        Module* module = pair.second;
        module->OnStoreNewItem(player, loot, item);
    }
}

void ModuleMgr::OnStoreNewItem(Player* player, Item* item)
{
    for (const auto& pair : modules)
    {
        Module* module = pair.second;
        module->OnStoreNewItem(player, item);
    }
}

void ModuleMgr::OnAddSpell(Player* player, uint32 spellId)
{
    for (const auto& pair : modules)
    {
        Module* module = pair.second;
        module->OnAddSpell(player, spellId);
    }
}

void ModuleMgr::OnDuelComplete(Player* player, Player* opponent, uint8 duelCompleteType)
{
    for (const auto& pair : modules)
    {
        Module* module = pair.second;
        module->OnDuelComplete(player, opponent, duelCompleteType);
    }
}

void ModuleMgr::OnKilledMonsterCredit(Player* player, uint32 entry, ObjectGuid& guid)
{
    for (const auto& pair : modules)
    {
        Module* module = pair.second;
        module->OnKilledMonsterCredit(player, entry, guid);
    }
}

void ModuleMgr::OnRewardSinglePlayerAtKill(Player* player, Unit* victim)
{
    for (const auto& pair : modules)
    {
        Module* module = pair.second;
        module->OnRewardSinglePlayerAtKill(player, victim);
    }
}

bool ModuleMgr::OnHandlePageTextQuery(Player* player, const WorldPacket& packet)
{
    for (const auto& pair : modules)
    {
        Module* module = pair.second;
        if (module->OnHandlePageTextQuery(player, packet))
        {
            return true;
        }
    }

    return false;
}

void ModuleMgr::OnUpdateSkill(Player* player, uint16 skillId)
{
    for (const auto& pair : modules)
    {
        Module* module = pair.second;
        module->OnUpdateSkill(player, skillId);
    }
}

void ModuleMgr::OnRewardHonor(Player* player, Unit* victim)
{
    for (const auto& pair : modules)
    {
        Module* module = pair.second;
        module->OnRewardHonor(player, victim);
    }
}

void ModuleMgr::OnEquipItem(Player* player, Item* item)
{
    for (const auto& pair : modules)
    {
        Module* module = pair.second;
        module->OnEquipItem(player, item);
    }
}

void ModuleMgr::OnTaxiFlightRouteStart(Player* player, const Taxi::Tracker& taxiTracker, bool initial)
{
    for (const auto& pair : modules)
    {
        Module* module = pair.second;
        module->OnTaxiFlightRouteStart(player, taxiTracker, initial);
    }
}

void ModuleMgr::OnTaxiFlightRouteEnd(Player* player, const Taxi::Tracker& taxiTracker, bool final)
{
    for (const auto& pair : modules)
    {
        Module* module = pair.second;
        module->OnTaxiFlightRouteEnd(player, taxiTracker, final);
    }
}

void ModuleMgr::OnEmote(Player* player, Unit* target, uint32 emote)
{
    for (const auto& pair : modules)
    {
        Module* module = pair.second;
        module->OnEmote(player, target, emote);
    }
}

void ModuleMgr::OnBuyBankSlot(Player* player, uint32 slot, uint32 price)
{
    for (const auto& pair : modules)
    {
        Module* module = pair.second;
        module->OnBuyBankSlot(player, slot, price);
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

bool ModuleMgr::OnUse(GameObject* gameObject, Unit* user)
{
    for (const auto& pair : modules)
    {
        Module* module = pair.second;
        if (module->OnUse(gameObject, user))
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

void ModuleMgr::OnDealDamage(Unit* unit, Unit* victim, uint32 health, uint32 damage)
{
    for (const auto& pair : modules)
    {
        Module* module = pair.second;
        module->OnDealDamage(unit, victim, health, damage);
    }
}

void ModuleMgr::OnKill(Unit* unit, Unit* victim)
{
    for (const auto& pair : modules)
    {
        Module* module = pair.second;
        module->OnKill(unit, victim);
    }
}

void ModuleMgr::OnDealHeal(Unit* unit, Unit* victim, int32 gain, uint32 addHealth)
{
    for (const auto& pair : modules)
    {
        Module* module = pair.second;
        module->OnDealHeal(unit, victim, gain, addHealth);
    }
}

void ModuleMgr::OnHit(Spell* spell, Unit* caster, Unit* victim)
{
    for (const auto& pair : modules)
    {
        Module* module = pair.second;
        module->OnHit(spell, caster, victim);
    }
}

void ModuleMgr::OnCast(Spell* spell, Unit* caster, Unit* victim)
{
    for (const auto& pair : modules)
    {
        Module* module = pair.second;
        module->OnCast(spell, caster, victim);
    }
}

bool ModuleMgr::OnFillLoot(Loot* loot, Player* owner)
{
    for (const auto& pair : modules)
    {
        Module* module = pair.second;
        if (module->OnFillLoot(loot, owner))
        {
            return true;
        }
    }

    return false;
}

bool ModuleMgr::OnGenerateMoneyLoot(Loot* loot, uint32& outMoney)
{
    for (const auto& pair : modules)
    {
        Module* module = pair.second;
        if (module->OnGenerateMoneyLoot(loot, outMoney))
        {
            return true;
        }
    }

    return false;
}

void ModuleMgr::OnAddItem(Loot* loot, LootItem* lootItem)
{
    for (const auto& pair : modules)
    {
        Module* module = pair.second;
        module->OnAddItem(loot, lootItem);
    }
}

void ModuleMgr::OnSendGold(Loot* loot, Player* player, uint32 gold, uint8 lootMethod)
{
    for (const auto& pair : modules)
    {
        Module* module = pair.second;
        module->OnSendGold(loot, player, gold, lootMethod);
    }
}

void ModuleMgr::OnHandleLootMasterGive(Loot* loot, Player* target, LootItem* lootItem)
{
    for (const auto& pair : modules)
    {
        Module* module = pair.second;
        module->OnHandleLootMasterGive(loot, target, lootItem);
    }
}

void ModuleMgr::OnPlayerRoll(Loot* loot, Player* player, uint32 itemSlot, uint8 rollType)
{
    for (const auto& pair : modules)
    {
        Module* module = pair.second;
        module->OnPlayerRoll(loot, player, itemSlot, rollType);
    }
}

void ModuleMgr::OnPlayerWinRoll(Loot* loot, Player* player, uint8 rollType, uint8 rollAmount, uint32 itemSlot, uint8 inventoryResult)
{
    for (const auto& pair : modules)
    {
        Module* module = pair.second;
        module->OnPlayerWinRoll(loot, player, rollType, rollAmount, itemSlot, inventoryResult);
    }
}

void ModuleMgr::OnStartBattleGround(BattleGround* battleground)
{
    for (const auto& pair : modules)
    {
        Module* module = pair.second;
        module->OnStartBattleGround(battleground);
    }
}

void ModuleMgr::OnEndBattleGround(BattleGround* battleground, uint32 winnerTeam)
{
    for (const auto& pair : modules)
    {
        Module* module = pair.second;
        module->OnEndBattleGround(battleground, winnerTeam);
    }
}

void ModuleMgr::OnUpdatePlayerScore(BattleGround* battleground, Player* player, uint8 scoreType, uint32 value)
{
    for (const auto& pair : modules)
    {
        Module* module = pair.second;
        module->OnUpdatePlayerScore(battleground, player, scoreType, value);
    }
}

void ModuleMgr::OnLeaveBattleGround(BattleGround* battleground, Player* player)
{
    for (const auto& pair : modules)
    {
        Module* module = pair.second;
        module->OnLeaveBattleGround(battleground, player);
    }
}

void ModuleMgr::OnJoinBattleGround(BattleGround* battleground, Player* player)
{
    for (const auto& pair : modules)
    {
        Module* module = pair.second;
        module->OnJoinBattleGround(battleground, player);
    }
}

void ModuleMgr::OnPickUpFlag(BattleGroundWS* battleground, Player* player, uint32 team)
{
    for (const auto& pair : modules)
    {
        Module* module = pair.second;
        module->OnPickUpFlag(battleground, player, team);
    }
}

void ModuleMgr::OnSellItem(AuctionEntry* auctionEntry, Player* player)
{
    for (const auto& pair : modules)
    {
        Module* module = pair.second;
        module->OnSellItem(auctionEntry, player);
    }
}

void ModuleMgr::OnSellItem(Player* player, Item* item, uint32 money)
{
    for (const auto& pair : modules)
    {
        Module* module = pair.second;
        module->OnSellItem(player, item, money);
    }
}

void ModuleMgr::OnBuyBackItem(Player* player, Item* item, uint32 money)
{
    for (const auto& pair : modules)
    {
        Module* module = pair.second;
        module->OnBuyBackItem(player, item, money);
    }
}

void ModuleMgr::OnSummoned(Player* player, const ObjectGuid& summoner)
{
    for (const auto& pair : modules)
    {
        Module* module = pair.second;
        module->OnSummoned(player, summoner);
    }
}

void ModuleMgr::OnAreaExplored(Player* player, uint32 areaId)
{
    for (const auto& pair : modules)
    {
        Module* module = pair.second;
        module->OnAreaExplored(player, areaId);
    }
}

void ModuleMgr::OnUpdateHonor(Player* player)
{
    for (const auto& pair : modules)
    {
        Module* module = pair.second;
        module->OnUpdateHonor(player);
    }
}

void ModuleMgr::OnSendMail(Player* player, const ObjectGuid& receiver, uint32 cost)
{
    for (const auto& pair : modules)
    {
        Module* module = pair.second;
        module->OnSendMail(player, receiver, cost);
    }
}

void ModuleMgr::OnAbandonQuest(Player* player, uint32 questId)
{
    for (const auto& pair : modules)
    {
        Module* module = pair.second;
        module->OnAbandonQuest(player, questId);
    }
}

void ModuleMgr::OnUpdateBid(AuctionEntry* auctionEntry, Player* player, uint32 newBid)
{
    for (const auto& pair : modules)
    {
        Module* module = pair.second;
        module->OnUpdateBid(auctionEntry, player, newBid);
    }
}

void ModuleMgr::OnWriteDump(uint32 playerId, std::string& dump)
{
    for (const auto& pair : modules)
    {
        Module* module = pair.second;
        module->OnWriteDump(playerId, dump);
    }
}

bool ModuleMgr::IsModuleDumpTable(const std::string& dbTableName)
{
    for (const auto& pair : modules)
    {
        Module* module = pair.second;
        if (module->IsModuleDumpTable(dbTableName))
        {
            return true;
        }
    }

    return false;
}

bool ModuleMgr::OnExecuteCommand(ChatHandler* chatHandler, const std::string& cmd)
{
    if (!cmd.empty())
    {
        // Extract the prefix and suffix of the cmd
        std::string cmdSuffix, cmdArgs;
        std::string cmdPrefix = cmd;
        size_t spacePos = cmd.find(' ');
        if (spacePos != std::string::npos)
        {
            cmdPrefix = cmd.substr(0, spacePos);
            cmdSuffix = cmd.substr(spacePos + 1);

            // Extract possible extra args from suffix
            spacePos = cmdSuffix.find(' ');
            if (spacePos != std::string::npos);
            {
                cmdArgs = cmdSuffix.substr(spacePos + 1);
                cmdSuffix = cmdSuffix.substr(0, spacePos);
            }
        }

        if (!cmdPrefix.empty() && !cmdSuffix.empty())
        {
            WorldSession* session = chatHandler->GetSession();
            for (const auto& pair : modules)
            {
                Module* module = pair.second;
                const char* moduleCommandPrefix = module->GetChatCommandPrefix();
                if (moduleCommandPrefix && moduleCommandPrefix == cmdPrefix)
                {
                    std::vector<ModuleChatCommand>* commandTable = module->GetCommandTable();
                    if (commandTable)
                    {
                        for (auto chatCommand : *commandTable)
                        {
                            if (chatCommand.name == cmdSuffix && session->GetSecurity() >= chatCommand.securityLevel)
                            {
                                return chatCommand.callback(session, cmdArgs);
                            }
                        }
                    }
                }
            }
        }
    }

    return false;
}