#ifndef MANGOS_MODULE_H
#define MANGOS_MODULE_H

class ModuleConfig;

class Module
{
public:
    Module();
    virtual ~Module() {}

    void Initialize();

    // Hooks
    virtual void OnInitialize() {}

    // World
    virtual void OnWorldInitialized() {}

protected:
    virtual ModuleConfig* CreateConfig() = 0;
    virtual ModuleConfig* GetConfig() = 0;
    ModuleConfig* GetConfigInternal() { return config; }

private:
    ModuleConfig* config;
};

#endif