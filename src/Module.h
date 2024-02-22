#ifndef MANGOS_MODULE_H
#define MANGOS_MODULE_H

#include "Platform/Define.h"
#include "Entities/Unit.h"

#include <map>
#include <string>

class Creature;
class GameObject;
class Item;
class Module;
class ModuleConfig;
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
struct PlayerLevelInfo;
struct SpellEntry;

typedef std::map<uint8, ActionButton> ActionButtonList;
typedef std::array<uint32, NUM_SPELL_PARTIAL_RESISTS> SpellPartialResistChanceEntry;
typedef std::vector<SpellPartialResistChanceEntry> SpellPartialResistDistribution;

class Module
{
public:
    Module(const std::string& name);
    virtual ~Module();

    void Initialize();

    // Module Hooks
    virtual void OnInitialize() {}
    virtual void OnUpdate(uint32 elapsed) {}

    // World Hooks
    // Called after world has loaded completely
    virtual void OnWorldInitialized() {}
    // Called every time the world updates
    virtual void OnWorldUpdated(uint32 elapsed) {}

    // Player Hooks
    // Called when a player uses an item. Return true to override default logic
    virtual bool OnUseItem(Player* player, Item* item) { return false; }
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
    // Called when a player learns a new talent
    virtual void OnLearnTalent(Player* player, uint32 spellId) {}
    // Called when a player resets his talents
    virtual void OnResetTalents(Player* player, uint32 cost) {}
    // Called before loading the player from DB
    virtual void OnPreLoadFromDB(uint32 playerId) {}
    // Called after the player has been loaded from DB
    virtual void OnLoadFromDB(Player* player) {}
    // Called when a player is saved to DB
    virtual void OnSaveToDB(Player* player) {}
    // Called when a character gets deleted
    virtual void OnDeleteFromDB(uint32 playerId) {}
    // Called when a player logs out of the game
    virtual void OnLogOut(Player* player) {}
    // Called when a new character is created
    virtual void OnCharacterCreated(Player* player) {}
    // Called when the action buttons of a character are loaded. Return true to override default logic
    virtual bool OnLoadActionButtons(Player* player, ActionButtonList& actionButtons) { return false; }
    // Called when the action buttons of a character are saved to DB. Return true to override default logic
    virtual bool OnSaveActionButtons(Player* player, ActionButtonList& actionButtons) { return false; }
    // Called when processing the fall damage of a player. Return true to override default logic
    virtual bool OnHandleFall(Player* player, const MovementInfo& movementInfo, float lastFallZ) { return false; }
    // Called when a player has been resurrected
    virtual void OnResurrect(Player* player) {}
    // Called when a player receives experience
    virtual void OnGiveXP(Player* player, uint32 xp, Creature* victim) {}
    // Called when a player receives a level
    virtual void OnGiveLevel(Player* player, uint32 level) {}
    // Called when a player receives or spends money (positive/negative)
    virtual void OnModifyMoney(Player* player, int32 diff) {}
    // Called when the reputation of a player changes
    virtual void OnSetReputation(Player* player, const FactionEntry* factionEntry, int32 standing, bool incremental) {}
    // Called when a player receives a quest reward
    virtual void OnRewardQuest(Player* player, const Quest* quest) {}
    // Called when retrieving the player level info
    virtual void OnGetPlayerLevelInfo(Player* player, PlayerLevelInfo& info) {}

    // Creature Hooks
    // Called before a creature respawns into the world. Return true to override default logic
    virtual bool OnRespawn(Creature* creature, time_t& respawnTime) { return false; }
    // Called when a creature manual respawn is requested
    virtual void OnRespawnRequest(Creature* creature) {}

    // Game Object Hooks
    // Called when a player uses a fishing node. Return true to override default logic
    virtual bool OnUseFishingNode(GameObject* gameObject, Player* player) { return false; }

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

protected:
    ModuleConfig* GetConfigInternal() { return config; }
    virtual ModuleConfig* CreateConfig() = 0;
    virtual ModuleConfig* GetConfig() = 0;

private:
    ModuleConfig* config;
    std::string name;
};

#endif