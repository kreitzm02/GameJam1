// Fill out your copyright notice in the Description page of Project Settings.


#include "Roomba_CleaningState.h"
#include <NavigationSystem.h>
#include "AIController.h"

const TCHAR* FRoomba_CleaningState::Name() const
{
	return TEXT("RoombaState_PatrolCleaning");
}

void FRoomba_CleaningState::OnEnter(AActor* a_Owner)
{
	m_Time = 0.0f;
	m_Index = -1;
	m_Phase = 1 - m_Phase;

	APawn* pawn = Cast<APawn>(a_Owner);
	if (!pawn) return;
	if (!pawn->GetController()) pawn->SpawnDefaultController();
	AAIController* ai = Cast<AAIController>(pawn->GetController());
	if (!ai) return;

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
			FNavLocation loc;
			if (nav->ProjectPointToNavigation(a, loc, projectExtent)) a = loc.Location;
			if (nav->ProjectPointToNavigation(b, loc, projectExtent)) b = loc.Location;
		}

		m_Path.Add(a);
		m_Path.Add(b);
	}

	// go to first point
	if (m_Path.Num() > 0)
	{
		m_Index = 0;
		ai->MoveToLocation(m_Path[m_Index], m_AcceptanceRadius, true);
	}
}

void FRoomba_CleaningState::OnUpdate(AActor* a_Owner, float a_DeltaTime)
{
	m_Time += a_DeltaTime;

	if (!a_Owner || !m_Path.IsValidIndex(m_Index)) return;
	APawn* pawn = Cast<APawn>(a_Owner);
	if (!pawn) return;
	AAIController* ai = Cast<AAIController>(pawn->GetController());
	if (!ai) return;

	const float Dist2D = FVector::Dist2D(a_Owner->GetActorLocation(), m_Path[m_Index]);
	if (Dist2D <= m_AcceptanceRadius)
	{
		m_Index = (m_Index + 1) % m_Path.Num();
		ai->MoveToLocation(m_Path[m_Index], m_AcceptanceRadius, true);
	}
}

void FRoomba_CleaningState::OnExit(AActor* a_Owner)
{
	if (APawn* pawn = Cast<APawn>(a_Owner))
	{
		if (AAIController* ai = Cast<AAIController>(pawn->GetController())) ai->StopMovement();
	}
}
