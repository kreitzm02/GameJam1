// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/SphereComponent.h>
#include "CookieBehaviour.generated.h"


UCLASS()
class GAMEJAM1_API ACookieBehaviour : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACookieBehaviour();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* m_OrbMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* m_CollisionSphere;

private:
	int32 m_CookieAmount = 1;

};
