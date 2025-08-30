// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectSpawnPoint.h"

// Sets default values
AObjectSpawnPoint::AObjectSpawnPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AObjectSpawnPoint::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	DrawDebugSphere(
		GetWorld(),
		GetActorLocation(),
		10,
		16,
		FColor::Red,
		false,
		5
	);
}

// Called when the game starts or when spawned
void AObjectSpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AObjectSpawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

