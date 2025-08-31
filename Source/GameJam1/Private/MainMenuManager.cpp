// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuManager.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
AMainMenuManager::AMainMenuManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMainMenuManager::BeginPlay()
{
	Super::BeginPlay();
    if (m_UIClass && !m_UI)
    {
        m_UI = CreateWidget<UUserWidget>(GetWorld(), m_UIClass);
    }
    if (!m_UI) return;

    m_UI->AddToViewport();
    if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
    {
        PC->bShowMouseCursor = true;
        PC->SetInputMode(FInputModeUIOnly());
    }
    UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.001f);
}

// Called every frame
void AMainMenuManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

