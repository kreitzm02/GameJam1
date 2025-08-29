// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ObjectSpawnPoint.generated.h"

UCLASS()
class GAMEJAM1_API AObjectSpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AObjectSpawnPoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SpawnRandomActor();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawnable Actors", meta=(AllowPrivateAccess="true"))
	TArray<UClass*> m_spawnableActors;
};
