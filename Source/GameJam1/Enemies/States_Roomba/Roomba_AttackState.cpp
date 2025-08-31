// Fill out your copyright notice in the Description page of Project Settings.


#include "Roomba_AttackState.h"
#include "RoombaEnemy.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Navigation/PathFollowingComponent.h"

void URoomba_AttackState::OnEnter(AActor* a_Owner)
{
    m_RepathTimer = 0.0f;
    m_AttackTimer = 0.0f;

    APawn* pawn = Cast<APawn>(a_Owner);
    if (!pawn) return;
    if (!pawn->GetController()) pawn->SpawnDefaultController();
    AAIController* ai = Cast<AAIController>(pawn->GetController());
    if (!ai) return;

    if (ACharacter* ch = Cast<ACharacter>(a_Owner))
    {
        if (ARoombaEnemy* ro = Cast<ARoombaEnemy>(ch))
        {
            if (UCharacterMovementComponent* move = ro->GetCharacterMovement())
            {
                move->MaxWalkSpeed = ro->m_ChaseSpeed;
                move->RotationRate = FRotator(0.0f, m_ChaseYawRateDeg, 0.0f);
                move->bOrientRotationToMovement = true;
            }

            ro->GetMesh()->PlayAnimation(ro->m_RunAnim, true);
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
    if (m_RepathTimer <= 0.0f)
    {
        ai->MoveToActor(m_Target.Get(), m_AcceptanceRadius, true);
        m_RepathTimer = m_RepathCooldown;
    }

    m_AttackTimer -= a_DeltaTime;

    if (m_Target.IsValid())
    {
        const float dist2D = FVector::Dist2D(a_Owner->GetActorLocation(), m_Target->GetActorLocation());

        if (dist2D <= m_AttackRange && m_AttackTimer <= 0.f && FMath::Abs(m_Target->GetActorLocation().Z - a_Owner->GetActorLocation().Z) <= 10.0f )
        {
            UGameplayStatics::ApplyDamage(m_Target.Get(), (float)m_Damage, (pawn ? pawn->GetController() : nullptr), a_Owner, UDamageType::StaticClass());

            m_AttackTimer = m_AttackCooldown;
        }
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
