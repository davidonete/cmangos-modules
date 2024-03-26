#include "ModuleMgr.h"
#include "Modules.h"
#include "Module.h"

#include "Entities/ObjectGuid.h"
#include "Entities/Player.h"
#include "Entities/Unit.h"

CmangosModuleMgr::~CmangosModuleMgr()
{
    for (Module* module : modules)
    {
        delete module;
        module = nullptr;
    }

    modules.clear();
}

void CmangosModuleMgr::RegisterModule(Module* module)
{
    modules.push_back(module);
}

void CmangosModuleMgr::OnWorldPreInitialized()
{
    AddModules();

    for (Module* module : modules)
    {
        module->LoadConfig();
        module->OnWorldPreInitialized();
    }
}

void CmangosModuleMgr::OnWorldInitialized()
{
    for (Module* module : modules)
    {
        module->Initialize();
        module->OnWorldInitialized();
    }
}

void CmangosModuleMgr::OnWorldUpdated(uint32 elapsed)
{
    for (Module* module : modules)
    {
        module->OnUpdate(elapsed);
        module->OnWorldUpdated(elapsed);
    }
}

bool CmangosModuleMgr::OnUseItem(Player* player, Item* item)
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

bool CmangosModuleMgr::OnPreGossipHello(Player* player, const ObjectGuid& guid)
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

void CmangosModuleMgr::OnGossipHello(Player* player, const ObjectGuid& guid)
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

bool CmangosModuleMgr::OnGossipSelect(Player* player, const ObjectGuid& guid, uint32 sender, uint32 action, const std::string& code, uint32 gossipListId)
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

void CmangosModuleMgr::OnLearnTalent(Player* player, uint32 spellId)
{
    for (Module* module : modules)
    {
        module->OnLearnTalent(player, spellId);
    }
}

void CmangosModuleMgr::OnResetTalents(Player* player, uint32 cost)
{
    for (Module* module : modules)
    {
        module->OnResetTalents(player, cost);
    }
}

void CmangosModuleMgr::OnPreLoadFromDB(Player* player)
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

void CmangosModuleMgr::OnLoadFromDB(Player* player)
{
    for (Module* module : modules)
    {
        module->OnLoadFromDB(player);
    }
}

void CmangosModuleMgr::OnSaveToDB(Player* player)
{
    for (Module* module : modules)
    {
        module->OnSaveToDB(player);
    }
}

void CmangosModuleMgr::OnDeleteFromDB(uint32 playerId)
{
    for (Module* module : modules)
    {
        module->OnDeleteFromDB(playerId);
    }
}

void CmangosModuleMgr::OnLogOut(Player* player)
{
    for (Module* module : modules)
    {
        module->OnLogOut(player);
    }
}

void CmangosModuleMgr::OnPreCharacterCreated(Player* player)
{
    for (Module* module : modules)
    {
        module->OnPreCharacterCreated(player);
    }
}

void CmangosModuleMgr::OnCharacterCreated(Player* player)
{
    for (Module* module : modules)
    {
        module->OnCharacterCreated(player);
    }
}

bool CmangosModuleMgr::OnLoadActionButtons(Player* player, ActionButtonList& actionButtons)
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

bool CmangosModuleMgr::OnLoadActionButtons(Player* player, ActionButtonList(&actionButtons)[2])
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

bool CmangosModuleMgr::OnSaveActionButtons(Player* player, ActionButtonList& actionButtons)
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

bool CmangosModuleMgr::OnSaveActionButtons(Player* player, ActionButtonList(&actionButtons)[2])
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

bool CmangosModuleMgr::OnPreHandleFall(Player* player, const MovementInfo& movementInfo, float lastFallZ, uint32& outDamage)
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

void CmangosModuleMgr::OnHandleFall(Player* player, const MovementInfo& movementInfo, float lastFallZ, uint32 damage)
{
    for (Module* module : modules)
    {
        module->OnHandleFall(player, movementInfo, lastFallZ, damage);
    }
}

bool CmangosModuleMgr::OnPreResurrect(Player* player)
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

void CmangosModuleMgr::OnResurrect(Player* player)
{
    for (Module* module : modules)
    {
        module->OnResurrect(player);
    }
}

void CmangosModuleMgr::OnReleaseSpirit(Player* player, const WorldSafeLocsEntry* closestGrave)
{
    for (Module* module : modules)
    {
        module->OnReleaseSpirit(player, closestGrave);
    }
}

void CmangosModuleMgr::OnDeath(Player* player, Unit* killer)
{
    for (Module* module : modules)
    {
        module->OnDeath(player, killer);
    }
}

void CmangosModuleMgr::OnDeath(Player* player, uint8 environmentalDamageType)
{
    for (Module* module : modules)
    {
        module->OnDeath(player, environmentalDamageType);
    }
}

void CmangosModuleMgr::OnGiveXP(Player* player, uint32 xp, Creature* victim)
{
    for (Module* module : modules)
    {
        module->OnGiveXP(player, xp, victim);
    }
}

void CmangosModuleMgr::OnGiveLevel(Player* player, uint32 level)
{
    for (Module* module : modules)
    {
        module->OnGiveLevel(player, level);
    }
}

void CmangosModuleMgr::OnModifyMoney(Player* player, int32 diff)
{
    for (Module* module : modules)
    {
        module->OnModifyMoney(player, diff);
    }
}

void CmangosModuleMgr::OnSetReputation(Player* player, const FactionEntry* factionEntry, int32 standing, bool incremental)
{
    for (Module* module : modules)
    {
        module->OnSetReputation(player, factionEntry, standing, incremental);
    }
}

void CmangosModuleMgr::OnRewardQuest(Player* player, const Quest* quest)
{
    for (Module* module : modules)
    {
        module->OnRewardQuest(player, quest);
    }
}

void CmangosModuleMgr::OnGetPlayerLevelInfo(Player* player, PlayerLevelInfo& info)
{
    for (Module* module : modules)
    {
        module->OnGetPlayerLevelInfo(player, info);
    }
}

void CmangosModuleMgr::OnSetVisibleItemSlot(Player* player, uint8 slot, Item* item)
{
    for (Module* module : modules)
    {
        module->OnSetVisibleItemSlot(player, slot, item);
    }
}

void CmangosModuleMgr::OnMoveItemFromInventory(Player* player, Item* item)
{
    for (Module* module : modules)
    {
        module->OnMoveItemFromInventory(player, item);
    }
}

void CmangosModuleMgr::OnMoveItemToInventory(Player* player, Item* item)
{
    for (Module* module : modules)
    {
        module->OnMoveItemToInventory(player, item);
    }
}

void CmangosModuleMgr::OnStoreItem(Player* player, Loot* loot, Item* item)
{
    for (Module* module : modules)
    {
        module->OnStoreItem(player, loot, item);
    }
}

void CmangosModuleMgr::OnStoreItem(Player* player, Item* item)
{
    for (Module* module : modules)
    {
        module->OnStoreItem(player, item);
    }
}

void CmangosModuleMgr::OnAddSpell(Player* player, uint32 spellId)
{
    for (Module* module : modules)
    {
        module->OnAddSpell(player, spellId);
    }
}

void CmangosModuleMgr::OnDuelComplete(Player* player, Player* opponent, uint8 duelCompleteType)
{
    for (Module* module : modules)
    {
        module->OnDuelComplete(player, opponent, duelCompleteType);
    }
}

void CmangosModuleMgr::OnKilledMonsterCredit(Player* player, uint32 entry, ObjectGuid& guid)
{
    for (Module* module : modules)
    {
        module->OnKilledMonsterCredit(player, entry, guid);
    }
}

bool CmangosModuleMgr::OnPreRewardPlayerAtKill(Player* player, Unit* victim)
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

void CmangosModuleMgr::OnRewardPlayerAtKill(Player* player, Unit* victim)
{
    for (Module* module : modules)
    {
        module->OnRewardPlayerAtKill(player, victim);
    }
}

bool CmangosModuleMgr::OnHandlePageTextQuery(Player* player, const WorldPacket& packet)
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

void CmangosModuleMgr::OnUpdateSkill(Player* player, uint16 skillId)
{
    for (Module* module : modules)
    {
        module->OnUpdateSkill(player, skillId);
    }
}

void CmangosModuleMgr::OnRewardHonor(Player* player, Unit* victim)
{
    for (Module* module : modules)
    {
        module->OnRewardHonor(player, victim);
    }
}

void CmangosModuleMgr::OnEquipItem(Player* player, Item* item)
{
    for (Module* module : modules)
    {
        module->OnEquipItem(player, item);
    }
}

void CmangosModuleMgr::OnTaxiFlightRouteStart(Player* player, const Taxi::Tracker& taxiTracker, bool initial)
{
    for (Module* module : modules)
    {
        module->OnTaxiFlightRouteStart(player, taxiTracker, initial);
    }
}

void CmangosModuleMgr::OnTaxiFlightRouteEnd(Player* player, const Taxi::Tracker& taxiTracker, bool final)
{
    for (Module* module : modules)
    {
        module->OnTaxiFlightRouteEnd(player, taxiTracker, final);
    }
}

void CmangosModuleMgr::OnEmote(Player* player, Unit* target, uint32 emote)
{
    for (Module* module : modules)
    {
        module->OnEmote(player, target, emote);
    }
}

void CmangosModuleMgr::OnBuyBankSlot(Player* player, uint32 slot, uint32 price)
{
    for (Module* module : modules)
    {
        module->OnBuyBankSlot(player, slot, price);
    }
}

bool CmangosModuleMgr::OnRespawn(Creature* creature, time_t& respawnTime)
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

void CmangosModuleMgr::OnRespawnRequest(Creature* creature)
{
    for (Module* module : modules)
    {
        module->OnRespawnRequest(creature);
    }
}

bool CmangosModuleMgr::OnUse(GameObject* gameObject, Unit* user)
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

bool CmangosModuleMgr::OnCalculateEffectiveDodgeChance(const Unit* unit, const Unit* attacker, uint8 attType, const SpellEntry* ability, float& outChance)
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

bool CmangosModuleMgr::OnCalculateEffectiveBlockChance(const Unit* unit, const Unit* attacker, uint8 attType, const SpellEntry* ability, float& outChance)
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

bool CmangosModuleMgr::OnCalculateEffectiveParryChance(const Unit* unit, const Unit* attacker, uint8 attType, const SpellEntry* ability, float& outChance)
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

bool CmangosModuleMgr::OnCalculateEffectiveCritChance(const Unit* unit, const Unit* victim, uint8 attType, const SpellEntry* ability, float& outChance)
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

bool CmangosModuleMgr::OnCalculateEffectiveMissChance(const Unit* unit, const Unit* victim, uint8 attType, const SpellEntry* ability, const Spell* const* currentSpells, const SpellPartialResistDistribution& spellPartialResistDistribution, float& outChance)
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

bool CmangosModuleMgr::OnCalculateSpellMissChance(const Unit* unit, const Unit* victim, uint32 schoolMask, const SpellEntry* spell, float& outChance)
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

bool CmangosModuleMgr::OnGetAttackDistance(const Unit* unit, const Unit* target, float& outDistance)
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

void CmangosModuleMgr::OnDealDamage(Unit* unit, Unit* victim, uint32 health, uint32 damage)
{
    for (Module* module : modules)
    {
        module->OnDealDamage(unit, victim, health, damage);
    }
}

void CmangosModuleMgr::OnKill(Unit* unit, Unit* victim)
{
    for (Module* module : modules)
    {
        module->OnKill(unit, victim);
    }
}

void CmangosModuleMgr::OnDealHeal(Unit* unit, Unit* victim, int32 gain, uint32 addHealth)
{
    for (Module* module : modules)
    {
        module->OnDealHeal(unit, victim, gain, addHealth);
    }
}

void CmangosModuleMgr::OnHit(Spell* spell, Unit* caster, Unit* victim)
{
    for (Module* module : modules)
    {
        module->OnHit(spell, caster, victim);
    }
}

void CmangosModuleMgr::OnCast(Spell* spell, Unit* caster, Unit* victim)
{
    for (Module* module : modules)
    {
        module->OnCast(spell, caster, victim);
    }
}

bool CmangosModuleMgr::OnFillLoot(Loot* loot, Player* owner)
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

bool CmangosModuleMgr::OnGenerateMoneyLoot(Loot* loot, uint32& outMoney)
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

void CmangosModuleMgr::OnAddItem(Loot* loot, LootItem* lootItem)
{
    for (Module* module : modules)
    {
        module->OnAddItem(loot, lootItem);
    }
}

void CmangosModuleMgr::OnSendGold(Loot* loot, Player* player, uint32 gold, uint8 lootMethod)
{
    for (Module* module : modules)
    {
        module->OnSendGold(loot, player, gold, lootMethod);
    }
}

void CmangosModuleMgr::OnHandleLootMasterGive(Loot* loot, Player* target, LootItem* lootItem)
{
    for (Module* module : modules)
    {
        module->OnHandleLootMasterGive(loot, target, lootItem);
    }
}

void CmangosModuleMgr::OnPlayerRoll(Loot* loot, Player* player, uint32 itemSlot, uint8 rollType)
{
    for (Module* module : modules)
    {
        module->OnPlayerRoll(loot, player, itemSlot, rollType);
    }
}

void CmangosModuleMgr::OnPlayerWinRoll(Loot* loot, Player* player, uint8 rollType, uint8 rollAmount, uint32 itemSlot, uint8 inventoryResult)
{
    for (Module* module : modules)
    {
        module->OnPlayerWinRoll(loot, player, rollType, rollAmount, itemSlot, inventoryResult);
    }
}

void CmangosModuleMgr::OnStartBattleGround(BattleGround* battleground)
{
    for (Module* module : modules)
    {
        module->OnStartBattleGround(battleground);
    }
}

void CmangosModuleMgr::OnEndBattleGround(BattleGround* battleground, uint32 winnerTeam)
{
    for (Module* module : modules)
    {
        module->OnEndBattleGround(battleground, winnerTeam);
    }
}

void CmangosModuleMgr::OnUpdatePlayerScore(BattleGround* battleground, Player* player, uint8 scoreType, uint32 value)
{
    for (Module* module : modules)
    {
        module->OnUpdatePlayerScore(battleground, player, scoreType, value);
    }
}

void CmangosModuleMgr::OnLeaveBattleGround(BattleGround* battleground, Player* player)
{
    for (Module* module : modules)
    {
        module->OnLeaveBattleGround(battleground, player);
    }
}

void CmangosModuleMgr::OnJoinBattleGround(BattleGround* battleground, Player* player)
{
    for (Module* module : modules)
    {
        module->OnJoinBattleGround(battleground, player);
    }
}

void CmangosModuleMgr::OnPickUpFlag(BattleGroundWS* battleground, Player* player, uint32 team)
{
    for (Module* module : modules)
    {
        module->OnPickUpFlag(battleground, player, team);
    }
}

void CmangosModuleMgr::OnSellItem(AuctionEntry* auctionEntry, Player* player)
{
    for (Module* module : modules)
    {
        module->OnSellItem(auctionEntry, player);
    }
}

void CmangosModuleMgr::OnSellItem(Player* player, Item* item, uint32 money)
{
    for (Module* module : modules)
    {
        module->OnSellItem(player, item, money);
    }
}

void CmangosModuleMgr::OnBuyBackItem(Player* player, Item* item, uint32 money)
{
    for (Module* module : modules)
    {
        module->OnBuyBackItem(player, item, money);
    }
}

void CmangosModuleMgr::OnSummoned(Player* player, const ObjectGuid& summoner)
{
    for (Module* module : modules)
    {
        module->OnSummoned(player, summoner);
    }
}

void CmangosModuleMgr::OnAreaExplored(Player* player, uint32 areaId)
{
    for (Module* module : modules)
    {
        module->OnAreaExplored(player, areaId);
    }
}

void CmangosModuleMgr::OnUpdateHonor(Player* player)
{
    for (Module* module : modules)
    {
        module->OnUpdateHonor(player);
    }
}

void CmangosModuleMgr::OnSendMail(Player* player, const ObjectGuid& receiver, uint32 cost)
{
    for (Module* module : modules)
    {
        module->OnSendMail(player, receiver, cost);
    }
}

void CmangosModuleMgr::OnAbandonQuest(Player* player, uint32 questId)
{
    for (Module* module : modules)
    {
        module->OnAbandonQuest(player, questId);
    }
}

void CmangosModuleMgr::OnUpdateBid(AuctionEntry* auctionEntry, Player* player, uint32 newBid)
{
    for (Module* module : modules)
    {
        module->OnUpdateBid(auctionEntry, player, newBid);
    }
}

void CmangosModuleMgr::OnWriteDump(uint32 playerId, std::string& dump)
{
    for (Module* module : modules)
    {
        module->OnWriteDump(playerId, dump);
    }
}

bool CmangosModuleMgr::IsModuleDumpTable(const std::string& dbTableName)
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

bool CmangosModuleMgr::OnExecuteCommand(ChatHandler* chatHandler, const std::string& cmd)
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