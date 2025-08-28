// Fill out your copyright notice in the Description page of Project Settings.


#include "Roomba_ScanState.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <RoombaEnemy.h>

void URoomba_ScanState::OnEnter(AActor* a_Owner)
{
    m_RotatedDegrees = 0.0f;
    m_TotalScanDegrees = FMath::FRandRange(150.0f, 300.0f);

    if (APawn* pawn = Cast<APawn>(a_Owner))
    {
        if (!pawn->GetController()) pawn->SpawnDefaultController();
        if (AAIController* ai = Cast<AAIController>(pawn->GetController()))
            ai->StopMovement();

        if (ACharacter* ch = Cast<ACharacter>(a_Owner))
        {
            if (ARoombaEnemy* ro = Cast<ARoombaEnemy>(ch))
            {
                m_ScanYawSpeedDegPerSec = ro->m_RotSpeed;
                if (UCharacterMovementComponent* move = ro->GetCharacterMovement())
                    move->MaxWalkSpeed = 0.0f;
                
                ro->GetMesh()->PlayAnimation(ro->m_IdleAnim, true);
            }
        }
    }

    if (ARoombaEnemy* enemy = Cast<ARoombaEnemy>(a_Owner))
        enemy->PlayStateLoop(enemy->m_DefaultSound, 1.0f, 1.0f);
}

void URoomba_ScanState::OnUpdate(AActor* a_Owner, float a_DeltaTime)
{
    if (!a_Owner || IsDone()) return;

    const float yawDelta = m_ScanYawSpeedDegPerSec * a_DeltaTime;
    a_Owner->AddActorWorldRotation(FRotator(0.0f, yawDelta, 0.0f));
    m_RotatedDegrees += FMath::Abs(yawDelta);
}

void URoomba_ScanState::OnExit(AActor* a_Owner)
{
    
}

