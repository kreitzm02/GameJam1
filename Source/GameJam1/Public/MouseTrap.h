// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ApproachBoxTraps.h"
#include "GameFramework/Actor.h"
#include "MouseTrap.generated.h"

UCLASS()
class GAMEJAM1_API AMouseTrap : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMouseTrap();

	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent)
	void OnApproachOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

private:
	UPROPERTY(EditAnywhere)
	UApproachBoxTraps* m_approachBox = nullptr;

	UPROPERTY(EditAnywhere, Category = "Hitbox Size")
	FVector m_hitboxSize = FVector::OneVector;

};
