// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StateMachine.h"

class GAMEJAM1_API URoomba_ScanState : public IBaseState
{
public:
    void OnEnter(AActor* a_Owner) override;
    void OnUpdate(AActor* a_Owner, float a_DeltaTime) override;
    void OnExit(AActor* a_Owner) override;

    bool IsDone() const { return m_RotatedDegrees >= m_TotalScanDegrees; }

private:
    float m_ScanYawSpeedDegPerSec = 100.0f; 
    float m_TotalScanDegrees = 360.0f;

    // intern
    float m_RotatedDegrees = 0.0f;
};
