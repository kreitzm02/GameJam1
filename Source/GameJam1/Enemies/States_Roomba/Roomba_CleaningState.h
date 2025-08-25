// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StateMachine.h"

class GAMEJAM1_API FRoomba_CleaningState : IBaseState
{
public:
	
	const TCHAR* Name() const override;
	void OnEnter(AActor* a_Owner) override;
	void OnUpdate(AActor* a_Owner, float a_DeltaTime) override;
	void OnExit(AActor* a_Owner) override;

private:

	float m_LaneWidth = 200.0f;
	float m_AreaHalfX = 800.0f;
	float m_AreaHalfY = 800.0f;
	float m_AcceptanceRadius = 35.0f;
	TArray<FVector> m_Path;
	int32 m_Index = -1;
	int32 m_Phase = 0;
	float m_Time = 0.0f;
};
