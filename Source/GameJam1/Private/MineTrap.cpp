// Fill out your copyright notice in the Description page of Project Settings.


#include "MineTrap.h"

#include "Components/SphereComponent.h"


// Sets default values
AMineTrap::AMineTrap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_approachBox = CreateDefaultSubobject<UApproachBoxTraps>("ApproachBox");
	m_approachBox->m_triggerBox = CreateDefaultSubobject<USphereComponent>("TriggerBox");
}

void AMineTrap::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	m_approachBox->m_triggerBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	m_approachBox->m_triggerBox->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);

	Cast<USphereComponent>(m_approachBox->m_triggerBox)->SetSphereRadius(m_hitboxRadius);
}

// Called when the game starts or when spawned
void AMineTrap::BeginPlay()
{
	Super::BeginPlay();
	m_approachBox->m_triggerBox->OnComponentBeginOverlap.AddDynamic(this, &AMineTrap::OnApproachOverlap);
}

// Called every frame
void AMineTrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

