#ifndef __ENEMY_AI_COMPONENT_H__
#define __ENEMY_AI_COMPONENT_H__

#include "../../../SharedDefines.h"
#include "../../ActorComponent.h"

#include "../ControllerComponents/HealthComponent.h"
#include "EnemyAIStateComponent.h"

class BaseEnemyAIStateComponent;
class PositionComponent;
class ActorRenderComponent;

typedef std::map<std::string, BaseEnemyAIStateComponent*> EnemyStateMap;
typedef std::vector<std::string> SoundList;

class EnemyAIComponent : public ActorComponent, public HealthObserver
{
public:
    EnemyAIComponent();
    ~EnemyAIComponent();

    static const char* g_Name;
    virtual const char* VGetName() const override { return g_Name; }

    virtual bool VInit(TiXmlElement* pData) override;
    virtual void VPostInit() override;
    virtual void VPostPostInit() override;
    virtual void VUpdate(uint32 msDiff) override;

    virtual TiXmlElement* VGenerateXml() override { return NULL; }

    void RegisterState(std::string stateName, BaseEnemyAIStateComponent* pState) { m_StateMap[stateName] = pState; }

    virtual void VOnHealthBelowZero(DamageType damageType) override;
    virtual void VOnHealthChanged(int32 oldHealth, int32 newHealth, DamageType damageType, Point impactPoint) override;
    virtual bool VCanResistDamage(DamageType damageType, Point impactPoint) override;
    virtual void VOnResistDamage(DamageType damageType, Point impactPoint) override;

    Point FindClosestHostileActorOffset();

    //void OnStateCanFinish();
    bool EnterBestState(bool canForceEnter);

    const SoundList& GetTakeDamageSounds() const { return m_TakeDamageSounds; }
    const SoundList& GetMeleeAttackSounds() const { return m_MeleeAttackSounds; }
    const SoundList& GetRangedAttackSounds() const { return m_RangedAttackSounds; }
    const SoundList& GetDeathSounds() const { return m_DeathSounds; }
    const SoundList& GetQuotToHostileUnitSounds() const { return m_QuoteToHostileUnitSounds; }

private:
    void LeaveAllStates();
    bool HasState(std::string stateName);
    bool HasState(EnemyAIState state);
    void EnterState(std::string stateName);
    void EnterState(EnemyAIState state);
    void EnterState(BaseEnemyAIStateComponent* pState);
    void AcquireStateLock();
    BaseEnemyAIStateComponent* GetCurrentState();
    BaseEnemyAIStateComponent* GetState(EnemyAIState state);

    bool m_bInitialized;
    bool m_bDead;
    bool m_bHasStateLock;
    EnemyStateMap m_StateMap;

    std::string m_DeathAnimation;

    shared_ptr<PositionComponent> m_pPositionComponent;
    shared_ptr<ActorRenderComponent> m_pRenderComponent;

    SoundList m_TakeDamageSounds;
    SoundList m_MeleeAttackSounds;
    SoundList m_RangedAttackSounds;
    SoundList m_DeathSounds;
    SoundList m_QuoteToHostileUnitSounds;
};

#endif