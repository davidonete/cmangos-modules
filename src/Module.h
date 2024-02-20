#ifndef MANGOS_MODULE_H
#define MANGOS_MODULE_H

class ModuleConfig;

class Module
{
public:
    Module();
    virtual ~Module();

    void Initialize();


    // Hooks
    // World
    virtual void OnWorldInitialized() {}

protected:
    virtual ModuleConfig* CreateConfig() = 0;

private:
    ModuleConfig* config;
};

#endif