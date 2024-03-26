#include "ModuleMgr.h"
#include "Modules.h"
#include "Module.h"

#include "Entities/ObjectGuid.h"
#include "Entities/Player.h"
#include "Entities/Unit.h"

ModuleMgr::~ModuleMgr()
{
    for (Module* module : modules)
    {
        delete module;
        module = nullptr;
    }

    modules.clear();
}

void ModuleMgr::RegisterModule(Module* module)
{
    modules.push_back(module);
}

void ModuleMgr::OnWorldPreInitialized()
{
    AddModules();

    for (Module* module : modules)
    {
        module->LoadConfig();
        module->OnWorldPreInitialized();
    }
}

void ModuleMgr::OnWorldInitialized()
{
    for (Module* module : modules)
    {
        module->Initialize();
        module->OnWorldInitialized();
    }
}

void ModuleMgr::OnWorldUpdated(uint32 elapsed)
{
    for (Module* module : modules)
    {
        module->OnUpdate(elapsed);
        module->OnWorldUpdated(elapsed);
    }
}

bool ModuleMgr::OnUseItem(Player* player, Item* item)
{
    bool overriden = false;
    for (Module* module : modules)
    {
        if (module->OnUseItem(player, item))
        {
            overriden = true;
        }
    }

    return overriden;
}

bool ModuleMgr::OnPreGossipHello(Player* player, const ObjectGuid& guid)
{
    bool overriden = false;
    if (player)
    {
        if (guid.IsAnyTypeCreature())
        {
            Creature* creature = player->GetNPCIfCanInteractWith(guid, UNIT_NPC_FLAG_NONE);
            if (creature)
            {
                for (Module* module : modules)
                {
                    if (module->OnPreGossipHello(player, creature))
                    {
                        overriden = true;
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
                    if (module->OnPreGossipHello(player, gameObject))
                    {
                        overriden = true;
                    }
                }
            }
        }
    }

    return overriden;
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
                for (Module* module : modules)
                {
                    module->OnGossipHello(player, creature);
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
                    module->OnGossipHello(player, gameObject);
                }
            }
        }
    }
}

bool ModuleMgr::OnGossipSelect(Player* player, const ObjectGuid& guid, uint32 sender, uint32 action, const std::string& code, uint32 gossipListId)
{
    bool overriden = false;
    if (player)
    {
        if (guid.IsAnyTypeCreature())
        {
            Creature* creature = player->GetNPCIfCanInteractWith(guid, UNIT_NPC_FLAG_NONE);
            if (creature)
            {
                for (Module* module : modules)
                {
                    if (module->OnGossipSelect(player, creature, sender, action, code, gossipListId))
                    {
                        overriden = true;
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
                    if (module->OnGossipSelect(player, gameObject, sender, action, code, gossipListId))
                    {
                        overriden = true;
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
                    if (module->OnGossipSelect(player, item, sender, action, code, gossipListId))
                    {
                        overriden = true;
                    }
                }
            }
        }
    }

    return overriden;
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

void ModuleMgr::OnPreLoadFromDB(Player* player)
{
    if (player)
    {
        const uint32 playerId = player->GetObjectGuid().GetCounter();
        for (Module* module : modules)
        {
            module->OnPreLoadFromDB(player);
            module->OnPreLoadFromDB(playerId);
        }
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

void ModuleMgr::OnDeleteFromDB(uint32 playerId)
{
    for (Module* module : modules)
    {
        module->OnDeleteFromDB(playerId);
    }
}

void ModuleMgr::OnLogOut(Player* player)
{
    for (Module* module : modules)
    {
        module->OnLogOut(player);
    }
}

void ModuleMgr::OnPreCharacterCreated(Player* player)
{
    for (Module* module : modules)
    {
        module->OnPreCharacterCreated(player);
    }
}

void ModuleMgr::OnCharacterCreated(Player* player)
{
    for (Module* module : modules)
    {
        module->OnCharacterCreated(player);
    }
}

bool ModuleMgr::OnLoadActionButtons(Player* player, ActionButtonList& actionButtons)
{
    bool overriden = false;
    for (Module* module : modules)
    {
        if (module->OnLoadActionButtons(player, actionButtons))
        {
            overriden = true;
        }
    }

    return overriden;
}

bool ModuleMgr::OnLoadActionButtons(Player* player, ActionButtonList(&actionButtons)[2])
{
    bool overriden = false;
    for (Module* module : modules)
    {
        for (auto& actionButton : actionButtons)
        {
            if (module->OnLoadActionButtons(player, actionButton))
            {
                overriden = true;
            }
        }

        if (overriden)
        {
            break;
        }
    }

    return overriden;
}

bool ModuleMgr::OnSaveActionButtons(Player* player, ActionButtonList& actionButtons)
{
    bool overriden = false;
    for (Module* module : modules)
    {
        if (module->OnSaveActionButtons(player, actionButtons))
        {
            overriden = true;
        }
    }

    return overriden;
}

bool ModuleMgr::OnSaveActionButtons(Player* player, ActionButtonList(&actionButtons)[2])
{
    bool overriden = false;
    for (Module* module : modules)
    {
        for (auto& actionButton : actionButtons)
        {
            if (module->OnSaveActionButtons(player, actionButton))
            {
                overriden = true;
            }
        }

        if (overriden)
        {
            break;
        }
    }

    return overriden;
}

bool ModuleMgr::OnPreHandleFall(Player* player, const MovementInfo& movementInfo, float lastFallZ, uint32& outDamage)
{
    bool overriden = false;
    for (Module* module : modules)
    {
        if (module->OnPreHandleFall(player, movementInfo, lastFallZ, outDamage))
        {
            overriden = true;
        }
    }

    return overriden;
}

void ModuleMgr::OnHandleFall(Player* player, const MovementInfo& movementInfo, float lastFallZ, uint32 damage)
{
    for (Module* module : modules)
    {
        module->OnHandleFall(player, movementInfo, lastFallZ, damage);
    }
}

bool ModuleMgr::OnPreResurrect(Player* player)
{
    bool overriden = false;
    for (Module* module : modules)
    {
        if (module->OnPreResurrect(player))
        {
            overriden = true;
        }
    }

    return overriden;
}

void ModuleMgr::OnResurrect(Player* player)
{
    for (Module* module : modules)
    {
        module->OnResurrect(player);
    }
}

void ModuleMgr::OnReleaseSpirit(Player* player, const WorldSafeLocsEntry* closestGrave)
{
    for (Module* module : modules)
    {
        module->OnReleaseSpirit(player, closestGrave);
    }
}

void ModuleMgr::OnDeath(Player* player, Unit* killer)
{
    for (Module* module : modules)
    {
        module->OnDeath(player, killer);
    }
}

void ModuleMgr::OnDeath(Player* player, uint8 environmentalDamageType)
{
    for (Module* module : modules)
    {
        module->OnDeath(player, environmentalDamageType);
    }
}

void ModuleMgr::OnGiveXP(Player* player, uint32 xp, Creature* victim)
{
    for (Module* module : modules)
    {
        module->OnGiveXP(player, xp, victim);
    }
}

void ModuleMgr::OnGiveLevel(Player* player, uint32 level)
{
    for (Module* module : modules)
    {
        module->OnGiveLevel(player, level);
    }
}

void ModuleMgr::OnModifyMoney(Player* player, int32 diff)
{
    for (Module* module : modules)
    {
        module->OnModifyMoney(player, diff);
    }
}

void ModuleMgr::OnSetReputation(Player* player, const FactionEntry* factionEntry, int32 standing, bool incremental)
{
    for (Module* module : modules)
    {
        module->OnSetReputation(player, factionEntry, standing, incremental);
    }
}

void ModuleMgr::OnRewardQuest(Player* player, const Quest* quest)
{
    for (Module* module : modules)
    {
        module->OnRewardQuest(player, quest);
    }
}

void ModuleMgr::OnGetPlayerLevelInfo(Player* player, PlayerLevelInfo& info)
{
    for (Module* module : modules)
    {
        module->OnGetPlayerLevelInfo(player, info);
    }
}

void ModuleMgr::OnSetVisibleItemSlot(Player* player, uint8 slot, Item* item)
{
    for (Module* module : modules)
    {
        module->OnSetVisibleItemSlot(player, slot, item);
    }
}

void ModuleMgr::OnMoveItemFromInventory(Player* player, Item* item)
{
    for (Module* module : modules)
    {
        module->OnMoveItemFromInventory(player, item);
    }
}

void ModuleMgr::OnMoveItemToInventory(Player* player, Item* item)
{
    for (Module* module : modules)
    {
        module->OnMoveItemToInventory(player, item);
    }
}

void ModuleMgr::OnStoreItem(Player* player, Loot* loot, Item* item)
{
    for (Module* module : modules)
    {
        module->OnStoreItem(player, loot, item);
    }
}

void ModuleMgr::OnStoreItem(Player* player, Item* item)
{
    for (Module* module : modules)
    {
        module->OnStoreItem(player, item);
    }
}

void ModuleMgr::OnAddSpell(Player* player, uint32 spellId)
{
    for (Module* module : modules)
    {
        module->OnAddSpell(player, spellId);
    }
}

void ModuleMgr::OnDuelComplete(Player* player, Player* opponent, uint8 duelCompleteType)
{
    for (Module* module : modules)
    {
        module->OnDuelComplete(player, opponent, duelCompleteType);
    }
}

void ModuleMgr::OnKilledMonsterCredit(Player* player, uint32 entry, ObjectGuid& guid)
{
    for (Module* module : modules)
    {
        module->OnKilledMonsterCredit(player, entry, guid);
    }
}

bool ModuleMgr::OnPreRewardPlayerAtKill(Player* player, Unit* victim)
{
    bool overriden = false;
    for (Module* module : modules)
    {
        if (module->OnPreRewardPlayerAtKill(player, victim))
        {
            overriden = true;
        }
    }

    return overriden;
}

void ModuleMgr::OnRewardPlayerAtKill(Player* player, Unit* victim)
{
    for (Module* module : modules)
    {
        module->OnRewardPlayerAtKill(player, victim);
    }
}

bool ModuleMgr::OnHandlePageTextQuery(Player* player, const WorldPacket& packet)
{
    bool overriden = false;
    for (Module* module : modules)
    {
        if (module->OnHandlePageTextQuery(player, packet))
        {
            overriden = true;
        }
    }

    return overriden;
}

void ModuleMgr::OnUpdateSkill(Player* player, uint16 skillId)
{
    for (Module* module : modules)
    {
        module->OnUpdateSkill(player, skillId);
    }
}

void ModuleMgr::OnRewardHonor(Player* player, Unit* victim)
{
    for (Module* module : modules)
    {
        module->OnRewardHonor(player, victim);
    }
}

void ModuleMgr::OnEquipItem(Player* player, Item* item)
{
    for (Module* module : modules)
    {
        module->OnEquipItem(player, item);
    }
}

void ModuleMgr::OnTaxiFlightRouteStart(Player* player, const Taxi::Tracker& taxiTracker, bool initial)
{
    for (Module* module : modules)
    {
        module->OnTaxiFlightRouteStart(player, taxiTracker, initial);
    }
}

void ModuleMgr::OnTaxiFlightRouteEnd(Player* player, const Taxi::Tracker& taxiTracker, bool final)
{
    for (Module* module : modules)
    {
        module->OnTaxiFlightRouteEnd(player, taxiTracker, final);
    }
}

void ModuleMgr::OnEmote(Player* player, Unit* target, uint32 emote)
{
    for (Module* module : modules)
    {
        module->OnEmote(player, target, emote);
    }
}

void ModuleMgr::OnBuyBankSlot(Player* player, uint32 slot, uint32 price)
{
    for (Module* module : modules)
    {
        module->OnBuyBankSlot(player, slot, price);
    }
}

bool ModuleMgr::OnRespawn(Creature* creature, time_t& respawnTime)
{
    bool overriden = false;
    for (Module* module : modules)
    {
        if (module->OnRespawn(creature, respawnTime))
        {
            overriden = true;
        }
    }

    return overriden;
}

void ModuleMgr::OnRespawnRequest(Creature* creature)
{
    for (Module* module : modules)
    {
        module->OnRespawnRequest(creature);
    }
}

bool ModuleMgr::OnUse(GameObject* gameObject, Unit* user)
{
    bool overriden = false;
    for (Module* module : modules)
    {
        if (module->OnUse(gameObject, user))
        {
            overriden = true;
        }
    }

    return overriden;
}

bool ModuleMgr::OnCalculateEffectiveDodgeChance(const Unit* unit, const Unit* attacker, uint8 attType, const SpellEntry* ability, float& outChance)
{
    bool overriden = false;
    for (Module* module : modules)
    {
        if (module->OnCalculateEffectiveDodgeChance(unit, attacker, attType, ability, outChance))
        {
            overriden = true;
        }
    }

    return overriden;
}

bool ModuleMgr::OnCalculateEffectiveBlockChance(const Unit* unit, const Unit* attacker, uint8 attType, const SpellEntry* ability, float& outChance)
{
    bool overriden = false;
    for (Module* module : modules)
    {
        if (module->OnCalculateEffectiveBlockChance(unit, attacker, attType, ability, outChance))
        {
            overriden = true;
        }
    }

    return overriden;
}

bool ModuleMgr::OnCalculateEffectiveParryChance(const Unit* unit, const Unit* attacker, uint8 attType, const SpellEntry* ability, float& outChance)
{
    bool overriden = false;
    for (Module* module : modules)
    {
        if (module->OnCalculateEffectiveParryChance(unit, attacker, attType, ability, outChance))
        {
            overriden = true;
        }
    }

    return overriden;
}

bool ModuleMgr::OnCalculateEffectiveCritChance(const Unit* unit, const Unit* victim, uint8 attType, const SpellEntry* ability, float& outChance)
{
    bool overriden = false;
    for (Module* module : modules)
    {
        if (module->OnCalculateEffectiveCritChance(unit, victim, attType, ability, outChance))
        {
            overriden = true;
        }
    }

    return overriden;
}

bool ModuleMgr::OnCalculateEffectiveMissChance(const Unit* unit, const Unit* victim, uint8 attType, const SpellEntry* ability, const Spell* const* currentSpells, const SpellPartialResistDistribution& spellPartialResistDistribution, float& outChance)
{
    bool overriden = false;
    for (Module* module : modules)
    {
        if (module->OnCalculateEffectiveMissChance(unit, victim, attType, ability, currentSpells, spellPartialResistDistribution, outChance))
        {
            overriden = true;
        }
    }

    return overriden;
}

bool ModuleMgr::OnCalculateSpellMissChance(const Unit* unit, const Unit* victim, uint32 schoolMask, const SpellEntry* spell, float& outChance)
{
    bool overriden = false;
    for (Module* module : modules)
    {
        if (module->OnCalculateSpellMissChance(unit, victim, schoolMask, spell, outChance))
        {
            overriden = true;
        }
    }

    return overriden;
}

bool ModuleMgr::OnGetAttackDistance(const Unit* unit, const Unit* target, float& outDistance)
{
    bool overriden = false;
    for (Module* module : modules)
    {
        if (module->OnGetAttackDistance(unit, target, outDistance))
        {
            overriden = true;
        }
    }

    return overriden;
}

void ModuleMgr::OnDealDamage(Unit* unit, Unit* victim, uint32 health, uint32 damage)
{
    for (Module* module : modules)
    {
        module->OnDealDamage(unit, victim, health, damage);
    }
}

void ModuleMgr::OnKill(Unit* unit, Unit* victim)
{
    for (Module* module : modules)
    {
        module->OnKill(unit, victim);
    }
}

void ModuleMgr::OnDealHeal(Unit* unit, Unit* victim, int32 gain, uint32 addHealth)
{
    for (Module* module : modules)
    {
        module->OnDealHeal(unit, victim, gain, addHealth);
    }
}

void ModuleMgr::OnHit(Spell* spell, Unit* caster, Unit* victim)
{
    for (Module* module : modules)
    {
        module->OnHit(spell, caster, victim);
    }
}

void ModuleMgr::OnCast(Spell* spell, Unit* caster, Unit* victim)
{
    for (Module* module : modules)
    {
        module->OnCast(spell, caster, victim);
    }
}

bool ModuleMgr::OnFillLoot(Loot* loot, Player* owner)
{
    bool overriden = false;
    for (Module* module : modules)
    {
        if (module->OnFillLoot(loot, owner))
        {
            overriden = true;
        }
    }

    return overriden;
}

bool ModuleMgr::OnGenerateMoneyLoot(Loot* loot, uint32& outMoney)
{
    bool overriden = false;
    for (Module* module : modules)
    {
        if (module->OnGenerateMoneyLoot(loot, outMoney))
        {
            overriden = true;
        }
    }

    return overriden;
}

void ModuleMgr::OnAddItem(Loot* loot, LootItem* lootItem)
{
    for (Module* module : modules)
    {
        module->OnAddItem(loot, lootItem);
    }
}

void ModuleMgr::OnSendGold(Loot* loot, Player* player, uint32 gold, uint8 lootMethod)
{
    for (Module* module : modules)
    {
        module->OnSendGold(loot, player, gold, lootMethod);
    }
}

void ModuleMgr::OnHandleLootMasterGive(Loot* loot, Player* target, LootItem* lootItem)
{
    for (Module* module : modules)
    {
        module->OnHandleLootMasterGive(loot, target, lootItem);
    }
}

void ModuleMgr::OnPlayerRoll(Loot* loot, Player* player, uint32 itemSlot, uint8 rollType)
{
    for (Module* module : modules)
    {
        module->OnPlayerRoll(loot, player, itemSlot, rollType);
    }
}

void ModuleMgr::OnPlayerWinRoll(Loot* loot, Player* player, uint8 rollType, uint8 rollAmount, uint32 itemSlot, uint8 inventoryResult)
{
    for (Module* module : modules)
    {
        module->OnPlayerWinRoll(loot, player, rollType, rollAmount, itemSlot, inventoryResult);
    }
}

void ModuleMgr::OnStartBattleGround(BattleGround* battleground)
{
    for (Module* module : modules)
    {
        module->OnStartBattleGround(battleground);
    }
}

void ModuleMgr::OnEndBattleGround(BattleGround* battleground, uint32 winnerTeam)
{
    for (Module* module : modules)
    {
        module->OnEndBattleGround(battleground, winnerTeam);
    }
}

void ModuleMgr::OnUpdatePlayerScore(BattleGround* battleground, Player* player, uint8 scoreType, uint32 value)
{
    for (Module* module : modules)
    {
        module->OnUpdatePlayerScore(battleground, player, scoreType, value);
    }
}

void ModuleMgr::OnLeaveBattleGround(BattleGround* battleground, Player* player)
{
    for (Module* module : modules)
    {
        module->OnLeaveBattleGround(battleground, player);
    }
}

void ModuleMgr::OnJoinBattleGround(BattleGround* battleground, Player* player)
{
    for (Module* module : modules)
    {
        module->OnJoinBattleGround(battleground, player);
    }
}

void ModuleMgr::OnPickUpFlag(BattleGroundWS* battleground, Player* player, uint32 team)
{
    for (Module* module : modules)
    {
        module->OnPickUpFlag(battleground, player, team);
    }
}

void ModuleMgr::OnSellItem(AuctionEntry* auctionEntry, Player* player)
{
    for (Module* module : modules)
    {
        module->OnSellItem(auctionEntry, player);
    }
}

void ModuleMgr::OnSellItem(Player* player, Item* item, uint32 money)
{
    for (Module* module : modules)
    {
        module->OnSellItem(player, item, money);
    }
}

void ModuleMgr::OnBuyBackItem(Player* player, Item* item, uint32 money)
{
    for (Module* module : modules)
    {
        module->OnBuyBackItem(player, item, money);
    }
}

void ModuleMgr::OnSummoned(Player* player, const ObjectGuid& summoner)
{
    for (Module* module : modules)
    {
        module->OnSummoned(player, summoner);
    }
}

void ModuleMgr::OnAreaExplored(Player* player, uint32 areaId)
{
    for (Module* module : modules)
    {
        module->OnAreaExplored(player, areaId);
    }
}

void ModuleMgr::OnUpdateHonor(Player* player)
{
    for (Module* module : modules)
    {
        module->OnUpdateHonor(player);
    }
}

void ModuleMgr::OnSendMail(Player* player, const ObjectGuid& receiver, uint32 cost)
{
    for (Module* module : modules)
    {
        module->OnSendMail(player, receiver, cost);
    }
}

void ModuleMgr::OnAbandonQuest(Player* player, uint32 questId)
{
    for (Module* module : modules)
    {
        module->OnAbandonQuest(player, questId);
    }
}

void ModuleMgr::OnUpdateBid(AuctionEntry* auctionEntry, Player* player, uint32 newBid)
{
    for (Module* module : modules)
    {
        module->OnUpdateBid(auctionEntry, player, newBid);
    }
}

void ModuleMgr::OnWriteDump(uint32 playerId, std::string& dump)
{
    for (Module* module : modules)
    {
        module->OnWriteDump(playerId, dump);
    }
}

bool ModuleMgr::IsModuleDumpTable(const std::string& dbTableName)
{
    for (Module* module : modules)
    {
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
            if (spacePos != std::string::npos)
            {
                cmdArgs = cmdSuffix.substr(spacePos + 1);
                cmdSuffix = cmdSuffix.substr(0, spacePos);
            }
        }

        if (!cmdPrefix.empty() && !cmdSuffix.empty())
        {
            WorldSession* session = chatHandler->GetSession();
            for (Module* module : modules)
            {
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