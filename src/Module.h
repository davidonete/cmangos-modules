#ifndef CMANGOS_MODULE_H
#define CMANGOS_MODULE_H

#include "Platform/Define.h"
#include "Entities/Unit.h"

#include <map>
#include <string>

class BattleGround;
class BattleGroundWS;
class ChatHandler;
class Creature;
class GameObject;
class Item;
class Loot;
class MovementInfo;
class ObjectGuid;
class Player;
class Quest;
class Spell;
namespace Taxi { class Tracker; }
class Unit;
class WorldObject;
class WorldPacket;

struct ActionButton;
struct AuctionEntry;
struct FactionEntry;
struct GossipMenuItems;
struct LootItem;
struct PlayerLevelInfo;
struct SpellEntry;
struct WorldSafeLocsEntry;

typedef std::map<uint8, ActionButton> ActionButtonList;
typedef std::array<uint32, NUM_SPELL_PARTIAL_RESISTS> SpellPartialResistChanceEntry;
typedef std::vector<SpellPartialResistChanceEntry> SpellPartialResistDistribution;

namespace cmangos_module
{
    class ModuleConfig;

    struct ModuleChatCommand
    {
        std::string name;
        std::function<bool(WorldSession*, const std::string&)> callback;
        uint32 securityLevel;
    };

    class Module
    {
        friend class ModuleMgr;

    public:
        Module(const std::string& name, ModuleConfig* config);
        virtual ~Module();

        void LoadConfig();
        void Initialize();

        // Module Hooks
        // Use it to initialize module (gets called when OnWorldInitialized)
        virtual void OnInitialize() {}
        // Used to update the module (gets called when OnWorldUpdated)
        virtual void OnUpdate(uint32 elapsed) {}

        // World Hooks
        // Called before the world loads
        virtual void OnWorldPreInitialized() {}
        // Called after world has loaded completely
        virtual void OnWorldInitialized() {}
        // Called every time the world updates
        virtual void OnWorldUpdated(uint32 elapsed) {}

        // Player Item Hooks
        // Called when a player uses an item. Return true to override default logic
        virtual bool OnUseItem(Player* player, Item* item) { return false; }
        // Called when updating the player equipment visibility
        virtual void OnSetVisibleItemSlot(Player* player, uint8 slot, Item* item) {}
        // Called when a player moves an item from the inventory
        virtual void OnMoveItemFromInventory(Player* player, Item* item) {}
        // Called when a player moves an item to the inventory
        virtual void OnMoveItemToInventory(Player* player, Item* item) {}
        // Called when a player stores an item into the inventory from looting
        virtual void OnStoreItem(Player* player, Loot* loot, Item* item) {}
        // Called when a player stores an item into the inventory
        virtual void OnStoreItem(Player* player, Item* item) {}
        // Called when a player equips an item
        virtual void OnEquipItem(Player* player, Item* item) {}
        // Called when a player sells an item to a vendor
        virtual void OnSellItem(Player* player, Item* item, uint32 money) {}
        // Called when a player buys back a sold item from a vendor
        virtual void OnBuyBackItem(Player* player, Item* item, uint32 money) {}

        // Player Gossip Hooks
        // Called before generating a gossip menu dialog. Return true to override default logic
        virtual bool OnPreGossipHello(Player* player, GameObject* gameObject) { return false; }
        // Called before generating a gossip menu dialog. Return true to override default logic
        virtual bool OnPreGossipHello(Player* player, Creature* creature) { return false; }
        // Called after generating a gossip menu dialog (just before sending to the client)
        virtual void OnGossipHello(Player* player, GameObject* gameObject) {}
        // Called after generating a gossip menu dialog (just before sending to the client)
        virtual void OnGossipHello(Player* player, Creature* creature) {}
        // Called when a player selects an option on a dialog. Return true to override default logic
        virtual bool OnGossipSelect(Player* player, GameObject* gameObject, uint32 sender, uint32 action, const std::string& code, uint32 gossipListId) { return false; }
        // Called when a player selects an option on a dialog. Return true to override default logic
        virtual bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action, const std::string& code, uint32 gossipListId) { return false; }
        // Called when a player selects an option on a dialog. Return true to override default logic
        virtual bool OnGossipSelect(Player* player, Item* item, uint32 sender, uint32 action, const std::string& code, uint32 gossipListId) { return false; }
    
        // Player Talent Hooks
        // Called when a player learns a new talent
        virtual void OnLearnTalent(Player* player, uint32 spellId) {}
        // Called when a player resets his talents
        virtual void OnResetTalents(Player* player, uint32 cost) {}

        // Player DB Hooks
        // Called before loading the player from DB
        virtual void OnPreLoadFromDB(Player* player) {}
        virtual void OnPreLoadFromDB(uint32 playerId) {}
        // Called after the player has been loaded from DB
        virtual void OnLoadFromDB(Player* player) {}
        // Called when a player is saved to DB
        virtual void OnSaveToDB(Player* player) {}
        // Called when a character gets deleted
        virtual void OnDeleteFromDB(uint32 playerId) {}

        // Player Session Hooks
        // Called when a player logs out of the game
        virtual void OnLogOut(Player* player) {}
        // Called before a new character is created
        virtual void OnPreCharacterCreated(Player* player) {}
        // Called when a new character is created
        virtual void OnCharacterCreated(Player* player) {}

        // Player Action Button Hooks
        // Called when the action buttons of a character are loaded. Return true to override default logic
        virtual bool OnLoadActionButtons(Player* player, ActionButtonList& actionButtons) { return false; }
        // Called when the action buttons of a character are saved to DB. Return true to override default logic
        virtual bool OnSaveActionButtons(Player* player, ActionButtonList& actionButtons) { return false; }
    
        // Player Action Hooks
        // Called before processing the fall damage of a player. Return true to override default logic
        virtual bool OnPreHandleFall(Player* player, const MovementInfo& movementInfo, float lastFallZ, uint32& outDamage) { return false; }
        // Called after processing the fall damage of a player
        virtual void OnHandleFall(Player* player, const MovementInfo& movementInfo, float lastFallZ, uint32 damage) {}
        // Called before a player resurrects. Return true to override default logic
        virtual bool OnPreResurrect(Player* player) { return false; }
        // Called when a player has been resurrected
        virtual void OnResurrect(Player* player) {}
        // Called when a player releases spirit
        virtual void OnReleaseSpirit(Player* player, const WorldSafeLocsEntry* closestGrave) {}
        // Called when a player has died
        virtual void OnDeath(Player* player, Unit* killer) {}
        // Called when a player has died from environment damage
        virtual void OnDeath(Player* player, uint8 environmentalDamageType) {}
        // Called when a player receives experience
        virtual void OnGiveXP(Player* player, uint32 xp, Creature* victim) {}
        // Called when a player receives a level
        virtual void OnGiveLevel(Player* player, uint32 level) {}
        // Called when a player receives or spends money (positive/negative)
        virtual void OnModifyMoney(Player* player, int32 diff) {}
        // Called when the reputation of a player changes
        virtual void OnSetReputation(Player* player, const FactionEntry* factionEntry, int32 standing, bool incremental) {}
        // Called when retrieving the player level info
        virtual void OnGetPlayerLevelInfo(Player* player, PlayerLevelInfo& info) {}
        // Called when a player skill changes
        virtual void OnUpdateSkill(Player* player, uint16 skillId) {}
        // Called when a players kills a unit that rewards honor
        virtual void OnRewardHonor(Player* player, Unit* victim) {}
        // Called when a duel has completed
        virtual void OnDuelComplete(Player* player, Player* opponent, uint8 duelCompleteType) {}
        // Called when a player kills a creature and receives credit for it
        virtual void OnKilledMonsterCredit(Player* player, uint32 entry, ObjectGuid& guid) {}
        // Called when a player kills a unit and before it processes the reward. Return true to override default logic
        virtual bool OnPreRewardPlayerAtKill(Player* player, Unit* victim) { return false; }
        // Called when a player not in a group kills a unit and processes the reward
        virtual void OnRewardPlayerAtKill(Player* player, Unit* victim) {}
        // Called when a player requests a page text data. Return true to override default logic
        virtual bool OnHandlePageTextQuery(Player* player, const WorldPacket& packet) { return false; }
        // Called when a player receives a quest reward
        virtual void OnRewardQuest(Player* player, const Quest* quest) {}
        // Called when a player starts a taxi flight route
        virtual void OnTaxiFlightRouteStart(Player* player, const Taxi::Tracker& taxiTracker, bool initial) {}
        // Called when a player finishes a taxi flight route
        virtual void OnTaxiFlightRouteEnd(Player* player, const Taxi::Tracker& taxiTracker, bool final) {}
        // Called when a player learns a spell
        virtual void OnAddSpell(Player* player, uint32 spellId) {}
        // Called when a player makes an emote
        virtual void OnEmote(Player* player, Unit* target, uint32 emote) {}
        // Called when a player buys a bank slot
        virtual void OnBuyBankSlot(Player* player, uint32 slot, uint32 price) {}
        // Called before a player gets summoned
        virtual void OnSummoned(Player* player, const ObjectGuid& summoner) {}
        // Called when a player explores a new area
        virtual void OnAreaExplored(Player* player, uint32 areaId) {}
        // Called when updating player honor
        virtual void OnUpdateHonor(Player* player) {}
        // Called when a player sends a mail
        virtual void OnSendMail(Player* player, const ObjectGuid& receiver, uint32 cost) {}
        // Called when a player abandons a quest
        virtual void OnAbandonQuest(Player* player, uint32 questId) {}

        // Creature Hooks
        // Called before a creature respawns into the world. Return true to override default logic
        virtual bool OnRespawn(Creature* creature, time_t& respawnTime) { return false; }
        // Called when a creature manual respawn is requested
        virtual void OnRespawnRequest(Creature* creature) {}

        // Game Object Hooks
        // Called when a unit uses a game object. Return true to override default logic
        virtual bool OnUse(GameObject* gameObject, Unit* user) { return false; }

        // Unit Hooks
        // Called when calculating the effective dodge chance of an attack. Return true to override default logic
        virtual bool OnCalculateEffectiveDodgeChance(const Unit* unit, const Unit* attacker, uint8 attType, const SpellEntry* ability, float& outChance) { return false; }
        // Called when calculating the effective block chance of an attack. Return true to override default logic
        virtual bool OnCalculateEffectiveBlockChance(const Unit* unit, const Unit* attacker, uint8 attType, const SpellEntry* ability, float& outChance) { return false; }
        // Called when calculating the effective parry chance of an attack. Return true to override default logic
        virtual bool OnCalculateEffectiveParryChance(const Unit* unit, const Unit* attacker, uint8 attType, const SpellEntry* ability, float& outChance) { return false; }
        // Called when calculating the effective crit chance of an attack. Return true to override default logic
        virtual bool OnCalculateEffectiveCritChance(const Unit* unit, const Unit* victim, uint8 attType, const SpellEntry* ability, float& outChance) { return false; }
        // Called when calculating the effective miss chance of an attack. Return true to override default logic
        virtual bool OnCalculateEffectiveMissChance(const Unit* unit, const Unit* victim, uint8 attType, const SpellEntry* ability, const Spell* const* currentSpells, const SpellPartialResistDistribution& spellPartialResistDistribution, float& outChance) { return false; }
        // Called when calculating the spell miss chance of an attack. Return true to override default logic
        virtual bool OnCalculateSpellMissChance(const Unit* unit, const Unit* victim, uint32 schoolMask, const SpellEntry* spell, float& outChance) { return false; }
        // Called when calculating the attack distance. Return true to override default logic
        virtual bool OnGetAttackDistance(const Unit* unit, const Unit* target, float& outDistance) { return false; }
        // Called when a unit deals damage to another unit
        virtual void OnDealDamage(Unit* unit, Unit* victim, uint32 health, uint32 damage) {}
        // Called when a unit kills another unit
        virtual void OnKill(Unit* unit, Unit* victim) {}
        // Called when a unit heals another unit
        virtual void OnDealHeal(Unit* unit, Unit* victim, int32 gain, uint32 addHealth) {}

        // Spell Hooks
        // Called when a spell hits a unit
        virtual void OnHit(Spell* spell, Unit* caster, Unit* victim) {}
        // Called when a spell is casted
        virtual void OnCast(Spell* spell, Unit* caster, Unit* victim) {}

        // Loot Hooks
        // Called when generating the loot table. Return true to override default logic
        virtual bool OnFillLoot(Loot* loot, Player* owner) { return false; }
        // Called when generating the loot money. Return true to override default logic
        virtual bool OnGenerateMoneyLoot(Loot* loot, uint32& outMoney) { return false; }
        // Called when an item gets added into the loot table
        virtual void OnAddItem(Loot* loot, LootItem* lootItem) {}
        // Called when the gold is taken from a loot
        virtual void OnSendGold(Loot* loot, Player* player, uint32 gold, uint8 lootMethod) {}
        // Called when a Loot Master sends an item to a player
        virtual void OnHandleLootMasterGive(Loot* loot, Player* target, LootItem* lootItem) {}
        // Called when a player rolls for an item
        virtual void OnPlayerRoll(Loot* loot, Player* player, uint32 itemSlot, uint8 rollType) {}
        // Called when a player wins a roll for an item
        virtual void OnPlayerWinRoll(Loot* loot, Player* player, uint8 rollType, uint8 rollAmount, uint32 itemSlot, uint8 inventoryResult) {}

        // Battleground Hooks
        // Called when a battleground starts
        virtual void OnStartBattleGround(BattleGround* battleground) {}
        // Called when a battleground ends
        virtual void OnEndBattleGround(BattleGround* battleground, uint32 winnerTeam) {}
        // Called when the battleground score gets updated for a player
        virtual void OnUpdatePlayerScore(BattleGround* battleground, Player* player, uint8 scoreType, uint32 value) {}
        // Called when a player leaves a battleground
        virtual void OnLeaveBattleGround(BattleGround* battleground, Player*player) {}
        // Called when a player joins a battleground
        virtual void OnJoinBattleGround(BattleGround* battleground, Player* player) {}
        // Called when a player picks up the flag from the base
        virtual void OnPickUpFlag(BattleGroundWS* battleground, Player* player, uint32 team) {}

        // Auction House Hooks
        // Called when a player puts an item for sale into the auction house
        virtual void OnSellItem(AuctionEntry* auctionEntry, Player* player) {}
        // Called when a player puts a bid for an auction item
        virtual void OnUpdateBid(AuctionEntry* auctionEntry, Player* player, uint32 newBid) {}

        // Player Dump Hooks
        // Called when dumping a player character
        virtual void OnWriteDump(uint32 playerId, std::string& dump) {}
        // Called when loading a player dump file. Return true if loading a dumped module db table
        virtual bool IsModuleDumpTable(const std::string& dbTableName) { return false; }

        // Chat Commands
        // Override these to enable chat commands for the module
        virtual const char* GetChatCommandPrefix() const { return nullptr; }
        virtual std::vector<ModuleChatCommand>* GetCommandTable() { return nullptr; }

    protected:
        virtual const ModuleConfig* GetConfig() const { return config; }

    private:
        ModuleConfig* config;
        std::string name;
    };
}

#endif