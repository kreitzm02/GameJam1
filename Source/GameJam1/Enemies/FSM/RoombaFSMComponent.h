// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StateMachine.h"
#include "States_Roomba/Roomba_CleaningState.h"
#include "States_Roomba/Roomba_PatrolState.h"
#include "States_Roomba/Roomba_ScanState.h"
#include "States_Roomba/Roomba_AttackState.h"
#include "RoombaFSMComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAMEJAM1_API URoombaFSMComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URoombaFSMComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float m_StateSwitchTimeMin = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float m_StateSwitchTimeMax = 15.0f;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	void BuildFSM();

	TUniquePtr<UStateMachine> m_FSM;
	IBaseState* m_LastState = nullptr;
	float m_TimeInState = 0.0f;
	float m_SwitchAtTime = 5.0f;

	URoomba_CleaningState m_CleaningState;
	URoomba_PatrolState m_PatrolState;
	URoomba_ScanState m_ScanState;
	URoomba_AttackState m_AttackState;
};
