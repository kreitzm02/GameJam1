// Fill out your copyright notice in the Description page of Project Settings.


#include "Roomba_AttackState.h"
#include "RoombaEnemy.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Navigation/PathFollowingComponent.h"

void URoomba_AttackState::OnEnter(AActor* a_Owner)
{
    m_RepathTimer = 0.f;

    APawn* pawn = Cast<APawn>(a_Owner);
    if (!pawn) return;
    if (!pawn->GetController()) pawn->SpawnDefaultController();
    AAIController* ai = Cast<AAIController>(pawn->GetController());
    if (!ai) return;

    if (ACharacter* ch = Cast<ACharacter>(a_Owner))
    {
        if (UCharacterMovementComponent* move = ch->GetCharacterMovement())
        {
            move->MaxWalkSpeed = m_ChaseSpeed;
            move->RotationRate = FRotator(0.f, m_ChaseYawRateDeg, 0.f);
            move->bOrientRotationToMovement = true;
        }
    }

    m_Target = PickTargetFromOwner(a_Owner);

    if (m_Target.IsValid())
    {
        ai->MoveToActor(m_Target.Get(), m_AcceptanceRadius, true);
    }
}

void URoomba_AttackState::OnUpdate(AActor* a_Owner, float a_DeltaTime)
{
    if (!a_Owner) return;

    APawn* pawn = Cast<APawn>(a_Owner);
    if (!pawn) return;
    AAIController* ai = Cast<AAIController>(pawn->GetController());
    if (!ai) return;

    if (!m_Target.IsValid())
    {
        m_Target = PickTargetFromOwner(a_Owner);
        if (m_Target.IsValid())
            ai->MoveToActor(m_Target.Get(), m_AcceptanceRadius, true);
        return;
    }

    m_RepathTimer -= a_DeltaTime;
    if (m_RepathTimer <= 0.f)
    {
        ai->MoveToActor(m_Target.Get(), m_AcceptanceRadius, true);
        m_RepathTimer = m_RepathCooldown;
    }
}

void URoomba_AttackState::OnExit(AActor* a_Owner)
{
    if (APawn* pawn = Cast<APawn>(a_Owner))
        if (AAIController* ai = Cast<AAIController>(pawn->GetController()))
            ai->StopMovement();

    m_Target = nullptr;
}

bool URoomba_AttackState::IsTargetLost(AActor* a_Owner) const
{
    if (!m_Target.IsValid()) return true;

    const ARoombaEnemy* enemy = Cast<ARoombaEnemy>(a_Owner);
    if (!enemy) return true;

    for (const TWeakObjectPtr<AActor>& t : enemy->m_OverlapDetectedActors)
        if (t.Get() == m_Target.Get())
            return false;

    return true;
}

AActor* URoomba_AttackState::PickTargetFromOwner(AActor* a_Owner)
{
    if (ARoombaEnemy* enemy = Cast<ARoombaEnemy>(a_Owner))
    {
        for (const TWeakObjectPtr<AActor>& t : enemy->m_ConeDetectedActors)
            if (t.IsValid()) return t.Get();
        for (const TWeakObjectPtr<AActor>& t : enemy->m_OverlapDetectedActors)
            if (t.IsValid()) return t.Get();
    }
    return nullptr;
}
