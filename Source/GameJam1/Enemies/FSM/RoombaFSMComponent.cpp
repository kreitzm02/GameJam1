// Fill out your copyright notice in the Description page of Project Settings.


#include "RoombaFSMComponent.h"
#include <RoombaEnemy.h>

// Sets default values for this component's properties
URoombaFSMComponent::URoombaFSMComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void URoombaFSMComponent::BeginPlay()
{
	Super::BeginPlay();

	m_FSM = MakeUnique <UStateMachine>(GetOwner());
	m_SwitchAtTime = FMath::FRandRange(m_StateSwitchTimeMin, m_StateSwitchTimeMax); m_SwitchAtTime = FMath::FRandRange(m_StateSwitchTimeMin, m_StateSwitchTimeMax);

	BuildFSM();
}


// Called every frame
void URoombaFSMComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!m_FSM) return;

	m_TimeInState += DeltaTime;

	IBaseState* before = m_FSM->GetCurrent();
	m_FSM->Update(DeltaTime);
	IBaseState* after = m_FSM->GetCurrent();

	if (after != before)
	{
		m_LastState = after;
		m_TimeInState = 0.0f;
		m_SwitchAtTime = FMath::FRandRange(m_StateSwitchTimeMin, m_StateSwitchTimeMax);
	}
}

void URoombaFSMComponent::BuildFSM()
{
	m_FSM->SetInitial(&m_PatrolState);

	// From: Patrol | To: Scan | Cond: Random Time && Patrol Position Reached
	{
		FTransition t;
		t.m_From = &m_PatrolState;
		t.m_To = &m_ScanState;
		t.m_Condition = [this]() { return m_TimeInState >= m_SwitchAtTime && m_PatrolState.IsAtGoal(GetOwner()); };
		m_FSM->AddTransition(MoveTemp(t));
	}

	// From: Scan | To : Patrol | Cond : Random Time && Scan Rotation Reached
	{
		FTransition t;
		t.m_From = &m_ScanState;
		t.m_To = &m_PatrolState;
		t.m_Condition = [this]() { return m_TimeInState >= m_SwitchAtTime && m_ScanState.IsDone(); };
		m_FSM->AddTransition(MoveTemp(t));
	}

	// From: Scan | To : Chase | Cond : Target In View Cone
	{
		FTransition t;
		t.m_From = &m_ScanState;
		t.m_To = &m_AttackState;
		t.m_Condition = [this]()
			{
				if (ARoombaEnemy* enemy = Cast<ARoombaEnemy>(GetOwner()))
				{
					for (const TWeakObjectPtr<AActor>& e : enemy->m_ConeDetectedActors)
						if (e.IsValid()) return true;
				}
				return false;
			};
		m_FSM->AddTransition(MoveTemp(t));
	}

	// From: Patrol | To : Chase | Cond : Target In View Cone
	{
		FTransition t;
		t.m_From = &m_PatrolState;
		t.m_To = &m_AttackState;
		t.m_Condition = [this]()
			{
				if (ARoombaEnemy* enemy = Cast<ARoombaEnemy>(GetOwner()))
				{
					for (const TWeakObjectPtr<AActor>& e : enemy->m_ConeDetectedActors)
						if (e.IsValid()) return true;
				}
				return false;
			};
		m_FSM->AddTransition(MoveTemp(t));
	}

	// From: Chase | To : Scan | Cond : Target Not In Detection Range
	{
		FTransition t;
		t.m_From = &m_AttackState;
		t.m_To = &m_ScanState;
		t.m_Condition = [this](){ return m_AttackState.IsTargetLost(GetOwner()); };
		m_FSM->AddTransition(MoveTemp(t));
	}
}
