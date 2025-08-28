// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <Components/SphereComponent.h>
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"
#include <RoombaFSMComponent.h>
#include "RoombaEnemy.generated.h"

UCLASS()
class GAMEJAM1_API ARoombaEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARoombaEnemy();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Roomba Components")
	UStaticMeshComponent* m_RoombaMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Roomba Components")
	USkeletalMeshComponent* m_NotRoombaMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Roomba Components")
	bool m_UseSkeletalMesh = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Roomba Components")
	USphereComponent* m_DetectionSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Roomba Components")
	URoombaFSMComponent* m_FSMComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Roomba Detection")
	float m_DetectionRadius = 900.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Roomba Detection")
	float m_ViewConeHalfAngleDeg = 35.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Roomba Detection")
	int32 m_ViewConeRayCount = 11;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Roomba Detection")
	float m_ViewConeRange = 900.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float m_DefaultSpeed = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float m_ChaseSpeed = 400.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float m_RotSpeed = 400.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Roomba Debug")
	bool m_DebugViewCone = false;

	UPROPERTY(VisibleAnywhere, Category = "Roomba Detection")
	TArray<TWeakObjectPtr<AActor>> m_OverlapDetectedActors;

	UPROPERTY(VisibleAnywhere, Category = "Roomba Detection")
	TArray<TWeakObjectPtr<AActor>> m_ConeDetectedActors;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Roomba Audio")
	UAudioComponent* m_AudioComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Roomba Audio")
	USoundBase* m_DefaultSound = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Roomba Audio")
	USoundBase* m_ChasingSound = nullptr;

	UPROPERTY(EditAnywhere)
	UAnimSequence* m_RunAnim = nullptr;

	UPROPERTY(EditAnywhere)
	UAnimSequence* m_WalkAnim = nullptr;

	UPROPERTY(EditAnywhere)
	UAnimSequence* m_IdleAnim = nullptr;

	UFUNCTION()
	void PlayStateLoop(USoundBase* a_Sound, float a_Volume = 1.f, float a_Pitch = 1.f);

	UFUNCTION()
	void StopStateAudio();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION()
	void OnDetectionBegin(UPrimitiveComponent* a_OverlappedComp, AActor* a_OtherActor, UPrimitiveComponent* a_OtherComp, int32 a_OtherBodyIndex, bool a_FromSweep, const FHitResult& a_SweepResult);

	UFUNCTION()
	void OnDetectionEnd(UPrimitiveComponent* a_OverlappedComp, AActor* a_OtherActor, UPrimitiveComponent* a_OtherComp, int32 a_OtherBodyIndex);

	void UpdateViewCone();

	TSet<TWeakObjectPtr<AActor>> m_OverlapSet;
	TSet<TWeakObjectPtr<AActor>> m_ConeSet;

};
