#ifndef MANGOS_MODULE_MGR_H
#define MANGOS_MODULE_MGR_H

#include "Platform/Define.h"
#include "Entities/Unit.h"

#include <map>
#include <string>
#include <vector>

class ChatHandler;
class Creature;
class GameObject;
class Item;
class Loot;
class Module;
class MovementInfo;
class ObjectGuid;
class Player;
class Spell;
class Quest;
class Unit;
class WorldObject;

struct ActionButton;
struct FactionEntry;
struct GossipMenuItems;
struct LootItem;
struct PlayerLevelInfo;
struct SpellEntry;
struct WorldSafeLocsEntry;

typedef std::map<uint8, ActionButton> ActionButtonList;
typedef std::array<uint32, NUM_SPELL_PARTIAL_RESISTS> SpellPartialResistChanceEntry;
typedef std::vector<SpellPartialResistChanceEntry> SpellPartialResistDistribution;

class ModuleMgr
{
public:
    ModuleMgr() {}
    ~ModuleMgr();

    void RegisterModule(Module* module, const std::string& name);

    // World Hooks
    void OnWorldPreInitialized();
    void OnWorldInitialized();
    void OnWorldUpdated(uint32 elapsed);

    // Player Hooks
    bool OnUseItem(Player* player, Item* item);
    bool OnPreGossipHello(Player* player, const ObjectGuid& guid);
    void OnGossipHello(Player* player, const ObjectGuid& guid);
    bool OnGossipSelect(Player* player, const ObjectGuid& guid, uint32 sender, uint32 action, const std::string& code, uint32 gossipListId);
    void OnLearnTalent(Player* player, uint32 spellId);
    void OnResetTalents(Player* player, uint32 cost);
    void OnPreLoadFromDB(Player* player);
    void OnLoadFromDB(Player* player);
    void OnSaveToDB(Player* player);
    void OnDeleteFromDB(uint32 playerId);
    void OnLogOut(Player* player);
    void OnPreCharacterCreated(Player* player);
    void OnCharacterCreated(Player* player);
    bool OnLoadActionButtons(Player* player, ActionButtonList& actionButtons);
    bool OnSaveActionButtons(Player* player, ActionButtonList& actionButtons);
    bool OnHandleFall(Player* player, const MovementInfo& movementInfo, float lastFallZ);
    bool OnPreResurrect(Player* player);
    void OnResurrect(Player* player);
    void OnReleaseSpirit(Player* player, const WorldSafeLocsEntry* closestGrave);
    void OnDeath(Player* player, Unit* killer);
    void OnGiveXP(Player* player, uint32 xp, Creature* victim);
    void OnGiveLevel(Player* player, uint32 level);
    void OnModifyMoney(Player* player, int32 diff);
    void OnSetReputation(Player* player, const FactionEntry* factionEntry, int32 standing, bool incremental);
    void OnRewardQuest(Player* player, const Quest* quest);
    void OnGetPlayerLevelInfo(Player* player, PlayerLevelInfo& info);
    void OnSetVisibleItemSlot(Player* player, uint8 slot, Item* item);
    void OnMoveItemFromInventory(Player* player, Item* item);
    void OnMoveItemToInventory(Player* player, Item* item);
    void OnStoreNewItem(Player* player, Loot* loot, Item* item);
    void OnAddSpell(Player* player, uint32 spellId);
    void OnDuelComplete(Player* player, Player* opponent, uint8 duelCompleteType);
    void OnKilledMonsterCredit(Player* player, uint32 entry, ObjectGuid& guid);
    void OnRewardSinglePlayerAtKill(Player* player, Unit* victim);

    // Creature Hooks
    bool OnRespawn(Creature* creature, time_t& respawnTime);
    void OnRespawnRequest(Creature* creature);

    // Game Object Hooks
    bool OnUseFishingNode(GameObject* gameObject, Player* player);

    // Unit Hooks
    bool OnCalculateEffectiveDodgeChance(const Unit* unit, const Unit* attacker, uint8 attType, const SpellEntry* ability, float& outChance);
    bool OnCalculateEffectiveBlockChance(const Unit* unit, const Unit* attacker, uint8 attType, const SpellEntry* ability, float& outChance);
    bool OnCalculateEffectiveParryChance(const Unit* unit, const Unit* attacker, uint8 attType, const SpellEntry* ability, float& outChance);
    bool OnCalculateEffectiveCritChance(const Unit* unit, const Unit* victim, uint8 attType, const SpellEntry* ability, float& outChance);
    bool OnCalculateEffectiveMissChance(const Unit* unit, const Unit* victim, uint8 attType, const SpellEntry* ability, const Spell* const* currentSpells, const SpellPartialResistDistribution& spellPartialResistDistribution, float& outChance);
    bool OnCalculateSpellMissChance(const Unit* unit, const Unit* victim, uint32 schoolMask, const SpellEntry* spell, float& outChance);
    bool OnGetAttackDistance(const Unit* unit, const Unit* target, float& outDistance);

    // Loot Hooks
    bool OnFillLoot(Loot* loot, Player* owner);
    bool OnGenerateMoneyLoot(Loot* loot, uint32& outMoney);
    void OnAddItem(Loot* loot, LootItem* lootItem);
    void OnSendGold(Loot* loot, uint32 gold);

    // Player Dump Hooks
    void OnWriteDump(uint32 playerId, std::string& dump);
    bool IsModuleDumpTable(const std::string& dbTableName);

    // Chat Commands
    bool OnExecuteCommand(ChatHandler* chatHandler, const std::string& cmd);

private:
    std::map<std::string, Module*> modules;
};

#define sModuleMgr MaNGOS::Singleton<ModuleMgr>::Instance()
#endif