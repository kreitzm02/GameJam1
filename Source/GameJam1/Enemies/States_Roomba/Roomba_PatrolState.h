// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StateMachine.h"

/**
 * 
 */
class GAMEJAM1_API URoomba_PatrolState : public IBaseState
{
public:
	void OnEnter(AActor* a_Owner) override;
	void OnUpdate(AActor* a_Owner, float a_DeltaTime) override;
	void OnExit(AActor* a_Owner) override;

	bool IsAtGoal(AActor* a_Owner) const;

private:
	float m_Radius = 1200.0f;
	float m_AcceptanceRadius = 35.0f;
	FVector m_CurrentTargetPos = FVector::ZeroVector;
	float m_Time = 0.0f;
	float m_MovementSpeed = 150.0f;

	bool  m_RotatingBeforeMove = false;
	float m_RotateSpeedDegPerSec = 100.0f;   
	float m_RotateYawToleranceDeg = 2.0f;           
};
