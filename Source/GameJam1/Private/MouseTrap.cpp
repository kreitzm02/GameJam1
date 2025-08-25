// Fill out your copyright notice in the Description page of Project Settings.


#include "MouseTrap.h"

#include "Components/BoxComponent.h"

// Sets default values
AMouseTrap::AMouseTrap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_approachBox = CreateDefaultSubobject<UApproachBoxTraps>("ApproachBox");
	m_approachBox->m_triggerBox = CreateDefaultSubobject<UBoxComponent>("TriggerBox");
}

void AMouseTrap::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

}

// Called when the game starts or when spawned
void AMouseTrap::BeginPlay()
{
	Super::BeginPlay();
	m_approachBox->m_triggerBox->OnComponentBeginOverlap.AddDynamic(this, &AMouseTrap::OnApproachOverlap);
}

// Called every frame
void AMouseTrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

