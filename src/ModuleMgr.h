#ifndef CMANGOS_MODULE_MGR_H
#define CMANGOS_MODULE_MGR_H

#include "Platform/Define.h"
#include "Entities/Unit.h"

#include <map>
#include <string>
#include <vector>

class BattleGround;
class BattleGroundWS;
class ChatHandler;
class Creature;
class GameObject;
class Item;
class Loot;
class MailDraft;
class MovementInfo;
class ObjectGuid;
class Player;
class Quest;
class Spell;
namespace Taxi { class Tracker; }
class TradeData;
class Unit;
class WorldObject;
class WorldPacket;

struct ActionButton;
struct AuctionEntry;
struct FactionEntry;
struct GossipMenuItems;
struct LootItem;
struct Mail;
struct PlayerClassLevelInfo;
struct PlayerLevelInfo;
struct ProcExecutionData;
struct SpellEntry;
struct WorldSafeLocsEntry;

typedef std::map<uint8, ActionButton> ActionButtonList;
typedef std::array<uint32, NUM_SPELL_PARTIAL_RESISTS> SpellPartialResistChanceEntry;
typedef std::vector<SpellPartialResistChanceEntry> SpellPartialResistDistribution;

namespace cmangos_module
{
    class Module;

    class ModuleMgr
    {
    public:
        ModuleMgr() {}
        ~ModuleMgr();

        void RegisterModule(Module* module);

        // World Hooks
        void OnWorldPreInitialized();
        void OnWorldInitialized();
        void OnWorldUpdated(uint32 elapsed);

        // Player Item Hooks
        bool OnUseItem(Player* player, Item* item);
        void OnSetVisibleItemSlot(Player* player, uint8 slot, Item* item);
        void OnMoveItemFromInventory(Player* player, Item* item);
        void OnMoveItemToInventory(Player* player, Item* item);
        void OnStoreItem(Player* player, Loot* loot, Item* item);
        void OnStoreItem(Player* player, Item* item);
        void OnEquipItem(Player* player, Item* item);
        void OnSellItem(Player* player, Item* item, uint32 money);
        void OnBuyBackItem(Player* player, Item* item, uint32 money);
        void OnCreateItem(Player* player, Item* item, uint32 amount);

        // Player Gossip Hooks
        bool OnPreGossipHello(Player* player, const ObjectGuid& guid);
        void OnGossipHello(Player* player, const ObjectGuid& guid);
        bool OnGossipSelect(Player* player, const ObjectGuid& guid, uint32 sender, uint32 action, const std::string& code, uint32 gossipListId);
        void OnGossipQuestDetails(Player* player, const Quest* quest, const ObjectGuid& questGiverGuid);
        void OnGossipQuestReward(Player* player, const Quest* quest, const ObjectGuid& questGiverGuid);

        // Player Talent Hooks
        void OnLearnTalent(Player* player, uint32 spellId);
        void OnResetTalents(Player* player, uint32 cost);

        // Player DB Hooks
        void OnPreLoadFromDB(Player* player);
        void OnLoadFromDB(Player* player);
        void OnSaveToDB(Player* player);
        void OnDeleteFromDB(uint32 playerId);

        // Player Session Hooks
        void OnLogOut(Player* player);
        void OnPreCharacterCreated(Player* player);
        void OnCharacterCreated(Player* player);

        // Player Action Button Hooks
        bool OnLoadActionButtons(Player* player, ActionButtonList (&actionButtons)[2]);
        bool OnLoadActionButtons(Player* player, ActionButtonList& actionButtons);
        bool OnSaveActionButtons(Player* player, ActionButtonList(&actionButtons)[2]);
        bool OnSaveActionButtons(Player* player, ActionButtonList& actionButtons);

        // Player Action Hooks
        bool OnPreHandleFall(Player* player, const MovementInfo& movementInfo, float lastFallZ, uint32& outDamage);
        void OnHandleFall(Player* player, const MovementInfo& movementInfo, float lastFallZ, uint32 damage);
        bool OnPreResurrect(Player* player);
        void OnResurrect(Player* player);
        void OnReleaseSpirit(Player* player, const WorldSafeLocsEntry* closestGrave);
        void OnDeath(Player* player, Unit* killer);
        void OnDeath(Player* player, uint8 environmentalDamageType);
        bool OnPreGiveXP(Player* player, uint32& xp, Creature* victim);
        void OnGiveXP(Player* player, uint32 xp, Creature* victim);
        void OnGiveLevel(Player* player, uint32 level);
        void OnModifyMoney(Player* player, int32 diff);
        void OnSetReputation(Player* player, const FactionEntry* factionEntry, int32 standing, bool incremental);
        void OnRewardQuest(Player* player, const Quest* quest);
        void OnGetPlayerClassLevelInfo(Player* player, PlayerClassLevelInfo& info);
        void OnGetPlayerLevelInfo(Player* player, PlayerLevelInfo& info);
        void OnAddSpell(Player* player, uint32 spellId);
        void OnDuelComplete(Player* player, Player* opponent, uint8 duelCompleteType);
        void OnKilledMonsterCredit(Player* player, uint32 entry, ObjectGuid& guid);
        bool OnPreRewardPlayerAtKill(Player* player, Unit* victim);
        void OnRewardPlayerAtKill(Player* player, Unit* victim);
        bool OnHandlePageTextQuery(Player* player, const WorldPacket& packet);
        void OnUpdateSkill(Player* player, uint16 skillId);
        void OnRewardHonor(Player* player, Unit* victim);
        void OnTaxiFlightRouteStart(Player* player, const Taxi::Tracker& taxiTracker, bool initial);
        void OnTaxiFlightRouteEnd(Player* player, const Taxi::Tracker& taxiTracker, bool final);
        void OnEmote(Player* player, Unit* target, uint32 emote);
        void OnBuyBankSlot(Player* player, uint32 slot, uint32 price);
        void OnSummoned(Player* player, const ObjectGuid& summoner);
        void OnAreaExplored(Player* player, uint32 areaId);
        void OnUpdateHonor(Player* player);
        void OnAcceptQuest(Player* player, uint32 questId, const ObjectGuid* questGiver);
        void OnAbandonQuest(Player* player, uint32 questId);
        bool OnPreHandleInitializeTrade(Player* player, Player* trader);
        void OnTradeAccepted(Player* player, Player* trader, TradeData* playerTrade, TradeData* traderTrade);
        void OnRegenerate(Player* player, uint8 power, uint32 diff, float& addedValue);

        // Player Mail Hooks
        bool OnCanCheckMailBox(Player* player, const ObjectGuid& mailboxGuid, bool& outResult);

        // Creature Hooks
        void OnAddToWorld(Creature* creature);
        bool OnRespawn(Creature* creature, time_t& respawnTime);
        void OnRespawnRequest(Creature* creature);

        // Game Object Hooks
        bool OnUse(GameObject* gameObject, Unit* user);

        // Unit Hooks
        bool OnCalculateEffectiveDodgeChance(const Unit* unit, const Unit* attacker, uint8 attType, const SpellEntry* ability, float& outChance);
        bool OnCalculateEffectiveBlockChance(const Unit* unit, const Unit* attacker, uint8 attType, const SpellEntry* ability, float& outChance);
        bool OnCalculateEffectiveParryChance(const Unit* unit, const Unit* attacker, uint8 attType, const SpellEntry* ability, float& outChance);
        bool OnCalculateEffectiveCritChance(const Unit* unit, const Unit* victim, uint8 attType, const SpellEntry* ability, float& outChance);
        bool OnCalculateEffectiveMissChance(const Unit* unit, const Unit* victim, uint8 attType, const SpellEntry* ability, const Spell* const* currentSpells, const SpellPartialResistDistribution& spellPartialResistDistribution, float& outChance);
        bool OnCalculateSpellMissChance(const Unit* unit, const Unit* victim, uint32 schoolMask, const SpellEntry* spell, float& outChance);
        bool OnGetAttackDistance(const Unit* unit, const Unit* target, float& outDistance);
        void OnDealDamage(Unit* unit, Unit* victim, uint32 health, uint32 damage);
        void OnKill(Unit* unit, Unit* victim);
        void OnDealHeal(Unit* unit, Unit* victim, int32 gain, uint32 addHealth);
        void OnSetPower(Unit* unit, uint8 power, uint32& value);
        bool OnGetReactionTo(const Unit* unit, const Unit* target, ReputationRank& outReaction);
        bool OnGetSpellRank(const Unit* unit, const SpellEntry* spellInfo, uint32& outSpellRank);

        // Spell Hooks
        void OnHit(Spell* spell, Unit* caster, Unit* victim);
        void OnCast(Spell* spell, Unit* caster, Unit* victim);
        void OnProc(const ProcExecutionData& data, SpellAuraProcResult& procResult);
        bool OnPeriodicTick(Aura* aura);

        // Loot Hooks
        bool OnFillLoot(Loot* loot, Player* owner);
        bool OnGenerateMoneyLoot(Loot* loot, uint32& outMoney);
        void OnAddItem(Loot* loot, LootItem* lootItem);
        void OnSendGold(Loot* loot, Player* player, uint32 gold, uint8 lootMethod);
        void OnHandleLootMasterGive(Loot* loot, Player* target, LootItem* lootItem);
        void OnPlayerRoll(Loot* loot, Player* player, uint32 itemSlot, uint8 rollType);
        void OnPlayerWinRoll(Loot* loot, Player* player, uint8 rollType, uint8 rollAmount, uint32 itemSlot, uint8 inventoryResult);

        // Battleground Hooks
        void OnStartBattleGround(BattleGround* battleground);
        void OnEndBattleGround(BattleGround* battleground, uint32 winnerTeam);
        void OnUpdatePlayerScore(BattleGround* battleground, Player* player, uint8 scoreType, uint32 value);
        void OnLeaveBattleGround(BattleGround* battleground, Player* player);
        void OnJoinBattleGround(BattleGround* battleground, Player* player);
        void OnPickUpFlag(BattleGroundWS* battleground, Player* player, uint32 team);

        // Group Hooks
        void OnAddMember(Group* group, Player* player, uint8 method);
        void OnRemoveMember(Group* group, Player* player, uint8 method);
        bool OnPreInviteMember(Group* group, Player* player, Player* recipient);

        // Auction House Hooks
        void OnSellItem(AuctionEntry* auctionEntry, Player* player);
        void OnUpdateBid(AuctionEntry* auctionEntry, Player* player, uint32 newBid);
        void OnActionBidWinning(AuctionEntry* auctionEntry, const ObjectGuid& owner, const ObjectGuid& bidder);

        // Mail Hooks
        void OnSendMail(const MailDraft& mail, Player* player, const ObjectGuid& receiver, uint32 cost);
        void OnMailTakeItem(Mail* mail, Player* player, Item* item, const ObjectGuid& sender);
        void OnMailTakeMoney(Mail* mail, Player* player, uint32 amount, const ObjectGuid& sender);

        // Player Dump Hooks
        void OnWriteDump(uint32 playerId, std::string& dump);
        bool IsModuleDumpTable(const std::string& dbTableName);

        // Chat Commands
        bool OnExecuteCommand(ChatHandler* chatHandler, const std::string& cmd);

    private:
        std::vector<Module*> modules;
    };
}

#define sModuleMgr MaNGOS::Singleton<cmangos_module::ModuleMgr>::Instance()
#endif