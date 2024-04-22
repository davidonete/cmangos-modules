#include "ModuleMgr.h"
#include "Modules.h"
#include "Module.h"

#include "Entities/ObjectGuid.h"
#include "Entities/Player.h"
#include "Entities/Unit.h"

INSTANTIATE_SINGLETON_1(cmangos_module::ModuleMgr);

namespace cmangos_module
{
    ModuleMgr::~ModuleMgr()
    {
        for (Module* mod : modules)
        {
            delete mod;
            mod = nullptr;
        }

        modules.clear();
    }

    void ModuleMgr::RegisterModule(Module* mod)
    {
        modules.push_back(mod);
    }

    void ModuleMgr::OnWorldPreInitialized()
    {
        AddModules();

        for (Module* mod : modules)
        {
            mod->LoadConfig();
            mod->OnWorldPreInitialized();
        }
    }

    void ModuleMgr::OnWorldInitialized()
    {
        for (Module* mod : modules)
        {
            mod->Initialize();
            mod->OnWorldInitialized();
        }
    }

    void ModuleMgr::OnWorldUpdated(uint32 elapsed)
    {
        for (Module* mod : modules)
        {
            mod->OnUpdate(elapsed);
            mod->OnWorldUpdated(elapsed);
        }
    }

    bool ModuleMgr::OnUseItem(Player* player, Item* item)
    {
        bool overriden = false;
        for (Module* mod : modules)
        {
            if (mod->OnUseItem(player, item))
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
                    for (Module* mod : modules)
                    {
                        if (mod->OnPreGossipHello(player, creature))
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
                    for (Module* mod : modules)
                    {
                        if (mod->OnPreGossipHello(player, gameObject))
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
                    for (Module* mod : modules)
                    {
                        mod->OnGossipHello(player, creature);
                    }
                }
            }
            else if (guid.IsGameObject())
            {
                GameObject* gameObject = player->GetGameObjectIfCanInteractWith(guid);
                if (gameObject)
                {
                    for (Module* mod : modules)
                    {
                        mod->OnGossipHello(player, gameObject);
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
                    for (Module* mod : modules)
                    {
                        if (mod->OnGossipSelect(player, creature, sender, action, code, gossipListId))
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
                    for (Module* mod : modules)
                    {
                        if (mod->OnGossipSelect(player, gameObject, sender, action, code, gossipListId))
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
                    for (Module* mod : modules)
                    {
                        if (mod->OnGossipSelect(player, item, sender, action, code, gossipListId))
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
        for (Module* mod : modules)
        {
            mod->OnLearnTalent(player, spellId);
        }
    }

    void ModuleMgr::OnResetTalents(Player* player, uint32 cost)
    {
        for (Module* mod : modules)
        {
            mod->OnResetTalents(player, cost);
        }
    }

    void ModuleMgr::OnPreLoadFromDB(Player* player)
    {
        if (player)
        {
            const uint32 playerId = player->GetObjectGuid().GetCounter();
            for (Module* mod : modules)
            {
                mod->OnPreLoadFromDB(player);
                mod->OnPreLoadFromDB(playerId);
            }
        }

    }

    void ModuleMgr::OnLoadFromDB(Player* player)
    {
        for (Module* mod : modules)
        {
            mod->OnLoadFromDB(player);
        }
    }

    void ModuleMgr::OnSaveToDB(Player* player)
    {
        for (Module* mod : modules)
        {
            mod->OnSaveToDB(player);
        }
    }

    void ModuleMgr::OnDeleteFromDB(uint32 playerId)
    {
        for (Module* mod : modules)
        {
            mod->OnDeleteFromDB(playerId);
        }
    }

    void ModuleMgr::OnLogOut(Player* player)
    {
        for (Module* mod : modules)
        {
            mod->OnLogOut(player);
        }
    }

    void ModuleMgr::OnPreCharacterCreated(Player* player)
    {
        for (Module* mod : modules)
        {
            mod->OnPreCharacterCreated(player);
        }
    }

    void ModuleMgr::OnCharacterCreated(Player* player)
    {
        for (Module* mod : modules)
        {
            mod->OnCharacterCreated(player);
        }
    }

    bool ModuleMgr::OnLoadActionButtons(Player* player, ActionButtonList& actionButtons)
    {
        bool overriden = false;
        for (Module* mod : modules)
        {
            if (mod->OnLoadActionButtons(player, actionButtons))
            {
                overriden = true;
            }
        }

        return overriden;
    }

    bool ModuleMgr::OnLoadActionButtons(Player* player, ActionButtonList(&actionButtons)[2])
    {
        bool overriden = false;
        for (Module* mod : modules)
        {
            for (auto& actionButton : actionButtons)
            {
                if (mod->OnLoadActionButtons(player, actionButton))
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
        for (Module* mod : modules)
        {
            if (mod->OnSaveActionButtons(player, actionButtons))
            {
                overriden = true;
            }
        }

        return overriden;
    }

    bool ModuleMgr::OnSaveActionButtons(Player* player, ActionButtonList(&actionButtons)[2])
    {
        bool overriden = false;
        for (Module* mod : modules)
        {
            for (auto& actionButton : actionButtons)
            {
                if (mod->OnSaveActionButtons(player, actionButton))
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
        for (Module* mod : modules)
        {
            if (mod->OnPreHandleFall(player, movementInfo, lastFallZ, outDamage))
            {
                overriden = true;
            }
        }

        return overriden;
    }

    void ModuleMgr::OnHandleFall(Player* player, const MovementInfo& movementInfo, float lastFallZ, uint32 damage)
    {
        for (Module* mod : modules)
        {
            mod->OnHandleFall(player, movementInfo, lastFallZ, damage);
        }
    }

    bool ModuleMgr::OnPreResurrect(Player* player)
    {
        bool overriden = false;
        for (Module* mod : modules)
        {
            if (mod->OnPreResurrect(player))
            {
                overriden = true;
            }
        }

        return overriden;
    }

    void ModuleMgr::OnResurrect(Player* player)
    {
        for (Module* mod : modules)
        {
            mod->OnResurrect(player);
        }
    }

    void ModuleMgr::OnReleaseSpirit(Player* player, const WorldSafeLocsEntry* closestGrave)
    {
        for (Module* mod : modules)
        {
            mod->OnReleaseSpirit(player, closestGrave);
        }
    }

    void ModuleMgr::OnDeath(Player* player, Unit* killer)
    {
        for (Module* mod : modules)
        {
            mod->OnDeath(player, killer);
        }
    }

    void ModuleMgr::OnDeath(Player* player, uint8 environmentalDamageType)
    {
        for (Module* mod : modules)
        {
            mod->OnDeath(player, environmentalDamageType);
        }
    }

    void ModuleMgr::OnGiveXP(Player* player, uint32 xp, Creature* victim)
    {
        for (Module* mod : modules)
        {
            mod->OnGiveXP(player, xp, victim);
        }
    }

    void ModuleMgr::OnGiveLevel(Player* player, uint32 level)
    {
        for (Module* mod : modules)
        {
            mod->OnGiveLevel(player, level);
        }
    }

    void ModuleMgr::OnModifyMoney(Player* player, int32 diff)
    {
        for (Module* mod : modules)
        {
            mod->OnModifyMoney(player, diff);
        }
    }

    void ModuleMgr::OnSetReputation(Player* player, const FactionEntry* factionEntry, int32 standing, bool incremental)
    {
        for (Module* mod : modules)
        {
            mod->OnSetReputation(player, factionEntry, standing, incremental);
        }
    }

    void ModuleMgr::OnRewardQuest(Player* player, const Quest* quest)
    {
        for (Module* mod : modules)
        {
            mod->OnRewardQuest(player, quest);
        }
    }

    void ModuleMgr::OnGetPlayerLevelInfo(Player* player, PlayerLevelInfo& info)
    {
        for (Module* mod : modules)
        {
            mod->OnGetPlayerLevelInfo(player, info);
        }
    }

    void ModuleMgr::OnSetVisibleItemSlot(Player* player, uint8 slot, Item* item)
    {
        for (Module* mod : modules)
        {
            mod->OnSetVisibleItemSlot(player, slot, item);
        }
    }

    void ModuleMgr::OnMoveItemFromInventory(Player* player, Item* item)
    {
        for (Module* mod : modules)
        {
            mod->OnMoveItemFromInventory(player, item);
        }
    }

    void ModuleMgr::OnMoveItemToInventory(Player* player, Item* item)
    {
        for (Module* mod : modules)
        {
            mod->OnMoveItemToInventory(player, item);
        }
    }

    void ModuleMgr::OnStoreItem(Player* player, Loot* loot, Item* item)
    {
        for (Module* mod : modules)
        {
            mod->OnStoreItem(player, loot, item);
        }
    }

    void ModuleMgr::OnStoreItem(Player* player, Item* item)
    {
        for (Module* mod : modules)
        {
            mod->OnStoreItem(player, item);
        }
    }

    void ModuleMgr::OnAddSpell(Player* player, uint32 spellId)
    {
        for (Module* mod : modules)
        {
            mod->OnAddSpell(player, spellId);
        }
    }

    void ModuleMgr::OnDuelComplete(Player* player, Player* opponent, uint8 duelCompleteType)
    {
        for (Module* mod : modules)
        {
            mod->OnDuelComplete(player, opponent, duelCompleteType);
        }
    }

    void ModuleMgr::OnKilledMonsterCredit(Player* player, uint32 entry, ObjectGuid& guid)
    {
        for (Module* mod : modules)
        {
            mod->OnKilledMonsterCredit(player, entry, guid);
        }
    }

    bool ModuleMgr::OnPreRewardPlayerAtKill(Player* player, Unit* victim)
    {
        bool overriden = false;
        for (Module* mod : modules)
        {
            if (mod->OnPreRewardPlayerAtKill(player, victim))
            {
                overriden = true;
            }
        }

        return overriden;
    }

    void ModuleMgr::OnRewardPlayerAtKill(Player* player, Unit* victim)
    {
        for (Module* mod : modules)
        {
            mod->OnRewardPlayerAtKill(player, victim);
        }
    }

    bool ModuleMgr::OnHandlePageTextQuery(Player* player, const WorldPacket& packet)
    {
        bool overriden = false;
        for (Module* mod : modules)
        {
            if (mod->OnHandlePageTextQuery(player, packet))
            {
                overriden = true;
            }
        }

        return overriden;
    }

    void ModuleMgr::OnUpdateSkill(Player* player, uint16 skillId)
    {
        for (Module* mod : modules)
        {
            mod->OnUpdateSkill(player, skillId);
        }
    }

    void ModuleMgr::OnRewardHonor(Player* player, Unit* victim)
    {
        for (Module* mod : modules)
        {
            mod->OnRewardHonor(player, victim);
        }
    }

    void ModuleMgr::OnEquipItem(Player* player, Item* item)
    {
        for (Module* mod : modules)
        {
            mod->OnEquipItem(player, item);
        }
    }

    void ModuleMgr::OnTaxiFlightRouteStart(Player* player, const Taxi::Tracker& taxiTracker, bool initial)
    {
        for (Module* mod : modules)
        {
            mod->OnTaxiFlightRouteStart(player, taxiTracker, initial);
        }
    }

    void ModuleMgr::OnTaxiFlightRouteEnd(Player* player, const Taxi::Tracker& taxiTracker, bool final)
    {
        for (Module* mod : modules)
        {
            mod->OnTaxiFlightRouteEnd(player, taxiTracker, final);
        }
    }

    void ModuleMgr::OnEmote(Player* player, Unit* target, uint32 emote)
    {
        for (Module* mod : modules)
        {
            mod->OnEmote(player, target, emote);
        }
    }

    void ModuleMgr::OnBuyBankSlot(Player* player, uint32 slot, uint32 price)
    {
        for (Module* mod : modules)
        {
            mod->OnBuyBankSlot(player, slot, price);
        }
    }

    void ModuleMgr::OnAddToMap(Creature* creature)
    {
        for (Module* mod : modules)
        {
            mod->OnAddToMap(creature);
        }
    }

    bool ModuleMgr::OnRespawn(Creature* creature, time_t& respawnTime)
    {
        bool overriden = false;
        for (Module* mod : modules)
        {
            if (mod->OnRespawn(creature, respawnTime))
            {
                overriden = true;
            }
        }

        return overriden;
    }

    void ModuleMgr::OnRespawnRequest(Creature* creature)
    {
        for (Module* mod : modules)
        {
            mod->OnRespawnRequest(creature);
        }
    }

    bool ModuleMgr::OnUse(GameObject* gameObject, Unit* user)
    {
        bool overriden = false;
        for (Module* mod : modules)
        {
            if (mod->OnUse(gameObject, user))
            {
                overriden = true;
            }
        }

        return overriden;
    }

    bool ModuleMgr::OnCalculateEffectiveDodgeChance(const Unit* unit, const Unit* attacker, uint8 attType, const SpellEntry* ability, float& outChance)
    {
        bool overriden = false;
        for (Module* mod : modules)
        {
            if (mod->OnCalculateEffectiveDodgeChance(unit, attacker, attType, ability, outChance))
            {
                overriden = true;
            }
        }

        return overriden;
    }

    bool ModuleMgr::OnCalculateEffectiveBlockChance(const Unit* unit, const Unit* attacker, uint8 attType, const SpellEntry* ability, float& outChance)
    {
        bool overriden = false;
        for (Module* mod : modules)
        {
            if (mod->OnCalculateEffectiveBlockChance(unit, attacker, attType, ability, outChance))
            {
                overriden = true;
            }
        }

        return overriden;
    }

    bool ModuleMgr::OnCalculateEffectiveParryChance(const Unit* unit, const Unit* attacker, uint8 attType, const SpellEntry* ability, float& outChance)
    {
        bool overriden = false;
        for (Module* mod : modules)
        {
            if (mod->OnCalculateEffectiveParryChance(unit, attacker, attType, ability, outChance))
            {
                overriden = true;
            }
        }

        return overriden;
    }

    bool ModuleMgr::OnCalculateEffectiveCritChance(const Unit* unit, const Unit* victim, uint8 attType, const SpellEntry* ability, float& outChance)
    {
        bool overriden = false;
        for (Module* mod : modules)
        {
            if (mod->OnCalculateEffectiveCritChance(unit, victim, attType, ability, outChance))
            {
                overriden = true;
            }
        }

        return overriden;
    }

    bool ModuleMgr::OnCalculateEffectiveMissChance(const Unit* unit, const Unit* victim, uint8 attType, const SpellEntry* ability, const Spell* const* currentSpells, const SpellPartialResistDistribution& spellPartialResistDistribution, float& outChance)
    {
        bool overriden = false;
        for (Module* mod : modules)
        {
            if (mod->OnCalculateEffectiveMissChance(unit, victim, attType, ability, currentSpells, spellPartialResistDistribution, outChance))
            {
                overriden = true;
            }
        }

        return overriden;
    }

    bool ModuleMgr::OnCalculateSpellMissChance(const Unit* unit, const Unit* victim, uint32 schoolMask, const SpellEntry* spell, float& outChance)
    {
        bool overriden = false;
        for (Module* mod : modules)
        {
            if (mod->OnCalculateSpellMissChance(unit, victim, schoolMask, spell, outChance))
            {
                overriden = true;
            }
        }

        return overriden;
    }

    bool ModuleMgr::OnGetAttackDistance(const Unit* unit, const Unit* target, float& outDistance)
    {
        bool overriden = false;
        for (Module* mod : modules)
        {
            if (mod->OnGetAttackDistance(unit, target, outDistance))
            {
                overriden = true;
            }
        }

        return overriden;
    }

    void ModuleMgr::OnDealDamage(Unit* unit, Unit* victim, uint32 health, uint32 damage)
    {
        for (Module* mod : modules)
        {
            mod->OnDealDamage(unit, victim, health, damage);
        }
    }

    void ModuleMgr::OnKill(Unit* unit, Unit* victim)
    {
        for (Module* mod : modules)
        {
            mod->OnKill(unit, victim);
        }
    }

    void ModuleMgr::OnDealHeal(Unit* unit, Unit* victim, int32 gain, uint32 addHealth)
    {
        for (Module* mod : modules)
        {
            mod->OnDealHeal(unit, victim, gain, addHealth);
        }
    }

    void ModuleMgr::OnHit(Spell* spell, Unit* caster, Unit* victim)
    {
        for (Module* mod : modules)
        {
            mod->OnHit(spell, caster, victim);
        }
    }

    void ModuleMgr::OnCast(Spell* spell, Unit* caster, Unit* victim)
    {
        for (Module* mod : modules)
        {
            mod->OnCast(spell, caster, victim);
        }
    }

    bool ModuleMgr::OnFillLoot(Loot* loot, Player* owner)
    {
        bool overriden = false;
        for (Module* mod : modules)
        {
            if (mod->OnFillLoot(loot, owner))
            {
                overriden = true;
            }
        }

        return overriden;
    }

    bool ModuleMgr::OnGenerateMoneyLoot(Loot* loot, uint32& outMoney)
    {
        bool overriden = false;
        for (Module* mod : modules)
        {
            if (mod->OnGenerateMoneyLoot(loot, outMoney))
            {
                overriden = true;
            }
        }

        return overriden;
    }

    void ModuleMgr::OnAddItem(Loot* loot, LootItem* lootItem)
    {
        for (Module* mod : modules)
        {
            mod->OnAddItem(loot, lootItem);
        }
    }

    void ModuleMgr::OnSendGold(Loot* loot, Player* player, uint32 gold, uint8 lootMethod)
    {
        for (Module* mod : modules)
        {
            mod->OnSendGold(loot, player, gold, lootMethod);
        }
    }

    void ModuleMgr::OnHandleLootMasterGive(Loot* loot, Player* target, LootItem* lootItem)
    {
        for (Module* mod : modules)
        {
            mod->OnHandleLootMasterGive(loot, target, lootItem);
        }
    }

    void ModuleMgr::OnPlayerRoll(Loot* loot, Player* player, uint32 itemSlot, uint8 rollType)
    {
        for (Module* mod : modules)
        {
            mod->OnPlayerRoll(loot, player, itemSlot, rollType);
        }
    }

    void ModuleMgr::OnPlayerWinRoll(Loot* loot, Player* player, uint8 rollType, uint8 rollAmount, uint32 itemSlot, uint8 inventoryResult)
    {
        for (Module* mod : modules)
        {
            mod->OnPlayerWinRoll(loot, player, rollType, rollAmount, itemSlot, inventoryResult);
        }
    }

    void ModuleMgr::OnStartBattleGround(BattleGround* battleground)
    {
        for (Module* mod : modules)
        {
            mod->OnStartBattleGround(battleground);
        }
    }

    void ModuleMgr::OnEndBattleGround(BattleGround* battleground, uint32 winnerTeam)
    {
        for (Module* mod : modules)
        {
            mod->OnEndBattleGround(battleground, winnerTeam);
        }
    }

    void ModuleMgr::OnUpdatePlayerScore(BattleGround* battleground, Player* player, uint8 scoreType, uint32 value)
    {
        for (Module* mod : modules)
        {
            mod->OnUpdatePlayerScore(battleground, player, scoreType, value);
        }
    }

    void ModuleMgr::OnLeaveBattleGround(BattleGround* battleground, Player* player)
    {
        for (Module* mod : modules)
        {
            mod->OnLeaveBattleGround(battleground, player);
        }
    }

    void ModuleMgr::OnJoinBattleGround(BattleGround* battleground, Player* player)
    {
        for (Module* mod : modules)
        {
            mod->OnJoinBattleGround(battleground, player);
        }
    }

    void ModuleMgr::OnPickUpFlag(BattleGroundWS* battleground, Player* player, uint32 team)
    {
        for (Module* mod : modules)
        {
            mod->OnPickUpFlag(battleground, player, team);
        }
    }

    void ModuleMgr::OnSellItem(AuctionEntry* auctionEntry, Player* player)
    {
        for (Module* mod : modules)
        {
            mod->OnSellItem(auctionEntry, player);
        }
    }

    void ModuleMgr::OnSellItem(Player* player, Item* item, uint32 money)
    {
        for (Module* mod : modules)
        {
            mod->OnSellItem(player, item, money);
        }
    }

    void ModuleMgr::OnBuyBackItem(Player* player, Item* item, uint32 money)
    {
        for (Module* mod : modules)
        {
            mod->OnBuyBackItem(player, item, money);
        }
    }

    void ModuleMgr::OnSummoned(Player* player, const ObjectGuid& summoner)
    {
        for (Module* mod : modules)
        {
            mod->OnSummoned(player, summoner);
        }
    }

    void ModuleMgr::OnAreaExplored(Player* player, uint32 areaId)
    {
        for (Module* mod : modules)
        {
            mod->OnAreaExplored(player, areaId);
        }
    }

    void ModuleMgr::OnUpdateHonor(Player* player)
    {
        for (Module* mod : modules)
        {
            mod->OnUpdateHonor(player);
        }
    }

    void ModuleMgr::OnSendMail(Player* player, const ObjectGuid& receiver, uint32 cost)
    {
        for (Module* mod : modules)
        {
            mod->OnSendMail(player, receiver, cost);
        }
    }

    void ModuleMgr::OnAbandonQuest(Player* player, uint32 questId)
    {
        for (Module* mod : modules)
        {
            mod->OnAbandonQuest(player, questId);
        }
    }

    void ModuleMgr::OnUpdateBid(AuctionEntry* auctionEntry, Player* player, uint32 newBid)
    {
        for (Module* mod : modules)
        {
            mod->OnUpdateBid(auctionEntry, player, newBid);
        }
    }

    void ModuleMgr::OnWriteDump(uint32 playerId, std::string& dump)
    {
        for (Module* mod : modules)
        {
            mod->OnWriteDump(playerId, dump);
        }
    }

    bool ModuleMgr::IsModuleDumpTable(const std::string& dbTableName)
    {
        for (Module* mod : modules)
        {
            if (mod->IsModuleDumpTable(dbTableName))
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
                for (Module* mod : modules)
                {
                    const char* moduleCommandPrefix = mod->GetChatCommandPrefix();
                    if (moduleCommandPrefix && moduleCommandPrefix == cmdPrefix)
                    {
                        std::vector<ModuleChatCommand>* commandTable = mod->GetCommandTable();
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
}