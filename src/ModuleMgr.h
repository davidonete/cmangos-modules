#ifndef MANGOS_MODULE_MGR_H
#define MANGOS_MODULE_MGR_H

#include "Platform/Define.h"

#include <map>
#include <string>
#include <vector>

class Creature;
class GameObject;
class Item;
class Module;
class MovementInfo;
class ObjectGuid;
class Player;
class Quest;
class WorldObject;

struct ActionButton;
struct FactionEntry;
struct GossipMenuItems;
struct PlayerLevelInfo;
struct SpellEntry;

typedef std::map<uint8, ActionButton> ActionButtonList;

class ModuleMgr
{
public:
    ModuleMgr() {}
    ~ModuleMgr();

    void RegisterModule(Module* module, const std::string& name);

    // World Hooks
    void OnWorldInitialized();
    void OnWorldUpdated(uint32 elapsed);

    // Player Hooks
    bool OnUseItem(Player* player, Item* item);
    bool OnGossipHello(Player* player, Creature* creature);
    bool OnGossipSelect(Player* player, const ObjectGuid& guid, uint32 sender, uint32 action, const std::string& code, uint32 gossipListId);
    void OnLearnTalent(Player* player, uint32 spellId);
    void OnResetTalents(Player* player, uint32 cost);
    void OnPreLoadFromDB(uint32 playerId);
    void OnLoadFromDB(Player* player);
    void OnSaveToDB(Player* player);
    void OnDeleteFromDB(uint32 playerId);
    void OnLogOut(Player* player);
    void OnCharacterCreated(Player* player);
    bool OnLoadActionButtons(Player* player, ActionButtonList& actionButtons);
    bool OnSaveActionButtons(Player* player, ActionButtonList& actionButtons);
    bool OnHandleFall(Player* player, const MovementInfo& movementInfo, float lastFallZ);
    void OnResurrect(Player* player);
    void OnGiveXP(Player* player, uint32 xp, Creature* victim);
    void OnGiveLevel(Player* player, uint32 level);
    void OnModifyMoney(Player* player, int32 diff);
    void OnSetReputation(Player* player, const FactionEntry* factionEntry, int32 standing, bool incremental);
    void OnRewardQuest(Player* player, const Quest* quest);
    bool OnPrepareGossipMenu(Player* player, WorldObject* source, const GossipMenuItems& gossipMenu);
    void OnGetPlayerLevelInfo(Player* player, PlayerLevelInfo& info);

    // Creature Hooks
    bool OnRespawn(Creature* creature);

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

private:
    std::map<std::string, Module*> modules;
};

#define sModuleMgr MaNGOS::Singleton<ModuleMgr>::Instance()
#endif