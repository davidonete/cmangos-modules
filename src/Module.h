#ifndef MANGOS_MODULE_H
#define MANGOS_MODULE_H

#include "Platform/Define.h"

#include <map>
#include <string>

class Creature;
class GameObject;
class Item;
class ModuleConfig;
class Player;
class Unit;

struct ActionButton;

typedef std::map<uint8, ActionButton> ActionButtonList;

class Module
{
public:
    Module(const std::string& name);
    virtual ~Module();

    void Initialize();

    // Module Hooks
    virtual void OnInitialize() {}

    // World Hooks
    // Called after world has loaded completely
    virtual void OnWorldInitialized() {}

    // Player Hooks
    // Called when a player uses an item. Return true to override default logic
    virtual bool OnUseItem(Player* player, Item* item) { return false; }
    // Called when a player opens a dialog. Return true to override default logic
    virtual bool OnGossipHello(Player* player, Creature* creature) { return false; }
    // Called when a player selects an option on a dialog. Return true to override default logic
    virtual bool OnGossipSelect(Player* player, GameObject* gameObject, uint32 sender, uint32 action, const std::string& code) { return false; }
    // Called when a player selects an option on a dialog. Return true to override default logic
    virtual bool OnGossipSelect(Player* player, Unit* creature, uint32 sender, uint32 action, const std::string& code) { return false; }
    // Called when a player selects an option on a dialog. Return true to override default logic
    virtual bool OnGossipSelect(Player* player, Item* item, uint32 sender, uint32 action, const std::string& code) { return false; }
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
    // Called when a player logs out of the game
    virtual void OnLogOut(Player* player) {}
    // Called when a new character is created
    virtual void OnCharacterCreated(Player* player) {}
    // Called when a character gets deleted
    virtual void OnCharacterDeleted(uint32 playerId) {}
    // Called when the action buttons of a character are loaded. Return true to override default logic
    virtual bool OnLoadActionButtons(Player* player, ActionButtonList& actionButtons) { return false; }
    // Called when the action buttons of a character are saved to DB. Return true to override default logic
    virtual bool OnSaveActionButtons(Player* player, ActionButtonList& actionButtons) { return false; }

protected:
    ModuleConfig* GetConfigInternal() { return config; }
    virtual ModuleConfig* CreateConfig() = 0;
    virtual ModuleConfig* GetConfig() = 0;

private:
    ModuleConfig* config;
};

#endif