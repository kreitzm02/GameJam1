// Fill out your copyright notice in the Description page of Project Settings.


#include "Roomba_PatrolState.h"
#include "Navigation/PathFollowingComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "AIController.h"
#include <NavigationSystem.h>
#include "RoombaEnemy.h"

void URoomba_PatrolState::OnEnter(AActor* a_Owner)
{
	m_Time = 0.0f;
	m_RotatingBeforeMove = false;

	APawn* pawn = Cast<APawn>(a_Owner);
	if (!pawn) return;
	if (!pawn->GetController()) pawn->SpawnDefaultController();
	AAIController* ai = Cast<AAIController>(pawn->GetController());

	if (!ai) return;

	if (UNavigationSystemV1* nav = FNavigationSystem::GetCurrent<UNavigationSystemV1>(a_Owner->GetWorld()))
	{
		FNavLocation loc;
		if (nav->GetRandomReachablePointInRadius(a_Owner->GetActorLocation(), m_Radius, loc))
		{
			m_CurrentTargetPos = loc.Location;

			m_RotatingBeforeMove = true;
			ai->StopMovement();
            if (ACharacter* ch = Cast<ACharacter>(a_Owner))
            {
                if (ARoombaEnemy* ro = Cast<ARoombaEnemy>(ch))
                {
                    if (UCharacterMovementComponent* move = ro->GetCharacterMovement())
                        move->MaxWalkSpeed = 0.0f;

                    ro->GetMesh()->PlayAnimation(ro->m_IdleAnim, true);
                }

            }
		}
	}

    if (ARoombaEnemy* enemy = Cast<ARoombaEnemy>(a_Owner))
        enemy->PlayStateLoop(enemy->m_DefaultSound, 1.0f, 1.0f);

	UE_LOG(LogTemp, Warning, TEXT("Entered Patrol State"));
}

void URoomba_PatrolState::OnUpdate(AActor* a_Owner, float a_DeltaTime)
{
    m_Time += a_DeltaTime;
    if (!a_Owner) return;

    APawn* pawn = Cast<APawn>(a_Owner);
    if (!pawn) return;
    AAIController* ai = Cast<AAIController>(pawn->GetController());
    if (!ai) return;

    if (m_RotatingBeforeMove)
    {
        const FRotator look = UKismetMathLibrary::FindLookAtRotation(a_Owner->GetActorLocation(), m_CurrentTargetPos);
        const float currentYaw = a_Owner->GetActorRotation().Yaw;

        const float deltaYaw = UKismetMathLibrary::NormalizedDeltaRotator(FRotator(0, look.Yaw, 0), FRotator(0, currentYaw, 0)).Yaw;

        const float maxStep = m_RotateSpeedDegPerSec * a_DeltaTime;
        const float step = FMath::Clamp(deltaYaw, -maxStep, maxStep);

        a_Owner->AddActorWorldRotation(FRotator(0.f, step, 0.f));

        if (FMath::Abs(deltaYaw) <= m_RotateYawToleranceDeg)
        {
            if (ACharacter* ch = Cast<ACharacter>(a_Owner))
            {
                if (ARoombaEnemy* ro = Cast<ARoombaEnemy>(ch))
                {
                    if (UCharacterMovementComponent* move = ro->GetCharacterMovement())
                        move->MaxWalkSpeed = ro->m_DefaultSpeed;

                    ro->GetMesh()->PlayAnimation(ro->m_WalkAnim, true);
                }

            }

            ai->MoveToLocation(m_CurrentTargetPos, m_AcceptanceRadius, true);
            m_RotatingBeforeMove = false;
        }
        return;
    }

    const float dist2d = FVector::Dist2D(a_Owner->GetActorLocation(), m_CurrentTargetPos);
    if (dist2d <= m_AcceptanceRadius)
    {
        if (UNavigationSystemV1* nav = FNavigationSystem::GetCurrent<UNavigationSystemV1>(a_Owner->GetWorld()))
        {
            FNavLocation loc;
            if (nav->GetRandomReachablePointInRadius(a_Owner->GetActorLocation(), m_Radius, loc))
            {
                m_CurrentTargetPos = loc.Location;
                m_RotatingBeforeMove = true;
                ai->StopMovement();
                if (ACharacter* ch = Cast<ACharacter>(a_Owner))
                {
                    if (ARoombaEnemy* ro = Cast<ARoombaEnemy>(ch))
                    {
                        if (UCharacterMovementComponent* move = ro->GetCharacterMovement())
                            move->MaxWalkSpeed = 0.0f;

                        ro->GetMesh()->PlayAnimation(ro->m_IdleAnim, true);
                    }

                }
            }
        }
    }
}

void URoomba_PatrolState::OnExit(AActor* a_Owner)
{
	if (APawn* pawn = Cast<APawn>(a_Owner))
	{
		if (AAIController* ai = Cast<AAIController>(pawn->GetController())) ai->StopMovement();
	}
}

bool URoomba_PatrolState::IsAtGoal(AActor* a_Owner) const
{
	if (!a_Owner) return false;

	const float dist = FVector::Dist2D(a_Owner->GetActorLocation(), m_CurrentTargetPos);

	const APawn* pawn = Cast<APawn>(a_Owner);
	const AAIController* ai = pawn ? Cast<AAIController>(pawn->GetController()) : nullptr;
	const UPathFollowingComponent* pfc = ai ? ai->GetPathFollowingComponent() : nullptr;
	const bool navIdle = pfc ? (pfc->GetStatus() != EPathFollowingStatus::Moving) : false;

	return navIdle || (dist <= m_AcceptanceRadius * 1.25f);
}

