// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StateMachine.h"
/**
 * 
 */
class GAMEJAM1_API URoomba_AttackState : public IBaseState
{
public:
    void OnEnter(AActor* a_Owner) override;
    void OnUpdate(AActor* a_Owner, float a_DeltaTime) override;
    void OnExit(AActor* a_Owner) override;

    bool HasTarget() const { return m_Target.IsValid(); }
    bool IsTargetLost(AActor* a_Owner) const;

private:
    TWeakObjectPtr<AActor> m_Target;

    float m_AcceptanceRadius = 60.0f;
    float m_ChaseSpeed = 400.0f;          
    float m_ChaseYawRateDeg = 720.0f;     

    float m_RepathCooldown = 0.2f;       
    float m_RepathTimer = 0.0f;

    float m_AttackRange = 125.0f;   
    float m_AttackCooldown = 0.6f;    
    float m_AttackTimer = 0.0f;    
    int32 m_Damage = 1;       

    AActor* PickTargetFromOwner(AActor* a_Owner);
};
