// Fill out your copyright notice in the Description page of Project Settings.


#include "MouseTrap.h"

#include "Components/BoxComponent.h"

// Sets default values
AMouseTrap::AMouseTrap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>("RootComponent");

	m_approachBox = CreateDefaultSubobject<UApproachBoxTraps>("ApproachBox");
	m_approachBox->m_triggerBox = CreateDefaultSubobject<UBoxComponent>("TriggerBox");

	m_approachBox->m_triggerBox->SetupAttachment(RootComponent);
}

void AMouseTrap::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	m_approachBox->m_triggerBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	m_approachBox->m_triggerBox->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);

	Cast<UBoxComponent>(m_approachBox->m_triggerBox)->SetRelativeScale3D(m_hitboxSize);
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

void AMouseTrap::DestroyApproachBox()
{
	m_approachBox->m_triggerBox->OnComponentBeginOverlap.RemoveAll(this);
	m_approachBox->m_triggerBox->DestroyComponent();
}

