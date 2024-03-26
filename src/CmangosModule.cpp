#include "CmangosModule.h"
#include "ModuleMgr.h"
#include "CmangosModuleConfig.h"

CmangosModule::CmangosModule(const std::string& name)
: config(nullptr)
, name(name)
{
    sModuleMgr.RegisterModule(this);
}

CmangosModule::~CmangosModule()
{
    delete config;
    config = nullptr;
}

void CmangosModule::LoadConfig()
{
    config = CreateConfig();
    config->Load();
}

void CmangosModule::Initialize()
{
    sLog.outString("Initializing %s module", name.c_str());
    OnInitialize();
}