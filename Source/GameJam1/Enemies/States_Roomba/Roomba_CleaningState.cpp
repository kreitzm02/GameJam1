// Fill out your copyright notice in the Description page of Project Settings.


#include "Roomba_CleaningState.h"
#include <NavigationSystem.h>
#include "AIController.h"
#include "Navigation/PathFollowingComponent.h"
#include "RoombaEnemy.h"

void URoomba_CleaningState::OnEnter(AActor* a_Owner)
{
	m_Time = 0.0f;
	m_Index = -1;
	m_Phase = 1 - m_Phase;

	if (ACharacter* ch = Cast<ACharacter>(a_Owner))
	{
		if (ARoombaEnemy* ro = Cast<ARoombaEnemy>(ch))
		{
			if (UCharacterMovementComponent* move = ro->GetCharacterMovement())
				move->MaxWalkSpeed = ro->m_DefaultSpeed;
		}
		
	}

	// build path
	m_Path.Reset();
	const FTransform t = a_Owner->GetActorTransform();
	const int numLanes = FMath::Max(1, FMath::FloorToInt((m_AreaHalfY * 2.0f) / m_LaneWidth));
	const float yStart = -m_AreaHalfY + (m_Phase * (m_LaneWidth * 0.5f));

	UWorld* world = a_Owner->GetWorld();

	UNavigationSystemV1* nav = FNavigationSystem::GetCurrent<UNavigationSystemV1>(world);
	const FVector projectExtent(50.0f, 50.0f, 200.0f);

	for (int i = 0; i < numLanes; i++)
	{
		const float y = yStart + i * m_LaneWidth;
		FVector aLocal(-m_AreaHalfX, y, 0.0f);
		FVector bLocal(m_AreaHalfX, y, 0.0f);
		if (i % 2 == 1) Swap(aLocal, bLocal);

		FVector a = t.TransformPosition(aLocal);
		FVector b = t.TransformPosition(bLocal);

		if (nav)
		{
			FNavLocation locA, locB;
			const bool okA = nav->ProjectPointToNavigation(a, locA, projectExtent);
			const bool okB = nav->ProjectPointToNavigation(b, locB, projectExtent);
			if (okA && okB)
			{
				m_Path.Add(locA.Location);
				m_Path.Add(locB.Location);
			}
		}
	}

	// go to first position

	APawn* pawn = Cast<APawn>(a_Owner);
	if (!pawn) return;
	if (!pawn->GetController()) pawn->SpawnDefaultController();
	AAIController* ai = Cast<AAIController>(pawn->GetController());
	if (!ai) return;

	if (m_Path.Num() > 0)
	{
		m_Index = 0;
		ai->MoveToLocation(m_Path[m_Index], m_AcceptanceRadius, true);
	}

	UE_LOG(LogTemp, Warning, TEXT("Entered Cleaning State"));
}

void URoomba_CleaningState::OnUpdate(AActor* a_Owner, float a_DeltaTime)
{
	m_Time += a_DeltaTime;
	if (!a_Owner) return;

	APawn* pawn = Cast<APawn>(a_Owner);
	if (!pawn) return;
	if (!pawn->GetController()) pawn->SpawnDefaultController();
	AAIController* ai = Cast<AAIController>(pawn->GetController());
	if (!ai) return;

	if (!m_Path.IsValidIndex(m_Index))
	{
		if (m_Path.Num() > 0)
		{
			m_Index = 0;
			ai->MoveToLocation(m_Path[m_Index], m_AcceptanceRadius, true);
		}
		return;
	}

	const float dist = FVector::Dist2D(a_Owner->GetActorLocation(), m_Path[m_Index]);

	EPathFollowingStatus::Type status = EPathFollowingStatus::Idle;
	if (auto* pfc = ai->GetPathFollowingComponent())
		status = pfc->GetStatus();

	const bool navIdle = (status != EPathFollowingStatus::Moving);
	const bool close = (dist <= m_AcceptanceRadius * 1.5f);

	if (navIdle || close)
	{
		m_Index = (m_Index + 1) % m_Path.Num();
		ai->MoveToLocation(m_Path[m_Index], m_AcceptanceRadius, true);
	}
}

void URoomba_CleaningState::OnExit(AActor* a_Owner)
{
	if (APawn* pawn = Cast<APawn>(a_Owner))
	{
		if (AAIController* ai = Cast<AAIController>(pawn->GetController())) ai->StopMovement();
	}
}

bool URoomba_CleaningState::IsAtGoal(AActor* a_Owner) const
{
	if (!a_Owner || !m_Path.IsValidIndex(m_Index)) return false;

	const float dist = FVector::Dist2D(a_Owner->GetActorLocation(), m_Path[m_Index]);

	const APawn* pawn = Cast<APawn>(a_Owner);
	const AAIController* ai = pawn ? Cast<AAIController>(pawn->GetController()) : nullptr;
	const UPathFollowingComponent* pfc = ai ? ai->GetPathFollowingComponent() : nullptr;
	const bool navIdle = pfc ? (pfc->GetStatus() != EPathFollowingStatus::Moving) : false;

	return navIdle || (dist <= m_AcceptanceRadius * 1.25f);
}
