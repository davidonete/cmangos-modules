#ifndef MANGOS_MODULE_MGR_H
#define MANGOS_MODULE_MGR_H

#include "Platform/Define.h"

#include <map>
#include <string>
#include <vector>

class Creature;
class Item;
class Module;
class ObjectGuid;
class Player;

struct ActionButton;

typedef std::map<uint8, ActionButton> ActionButtonList;

class ModuleMgr
{
public:
    ModuleMgr() {}
    ~ModuleMgr();

    void RegisterModule(Module* module, const std::string& name);

    // World Hooks
    void OnWorldInitialized();

    // Player Hooks
    bool OnUseItem(Player* player, Item* item);
    bool OnGossipHello(Player* player, Creature* creature);
    bool OnGossipSelect(Player* player, const ObjectGuid& guid, uint32 sender, uint32 action, const std::string& code);
    void OnLearnTalent(Player* player, uint32 spellId);
    void OnResetTalents(Player* player, uint32 cost);
    void OnPreLoadFromDB(uint32 playerId);
    void OnLoadFromDB(Player* player);
    void OnSaveToDB(Player* player);
    void OnLogOut(Player* player);
    void OnCharacterCreated(Player* player);
    void OnCharacterDeleted(uint32 playerId);
    bool OnLoadActionButtons(Player* player, ActionButtonList& actionButtons);
    bool OnSaveActionButtons(Player* player, ActionButtonList& actionButtons);

private:
    std::map<std::string, Module*> modules;
};

#define sModuleMgr MaNGOS::Singleton<ModuleMgr>::Instance()
#endif