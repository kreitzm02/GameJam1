// Fill out your copyright notice in the Description page of Project Settings.


#include "GameManager.h"
#include "Components/AudioComponent.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
AGameManager::AGameManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGameManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGameManager::ShowDeathScreen()
{
    if (m_DeathScreenUIClass && !m_DeathScreenUI)
    {
        m_DeathScreenUI = CreateWidget<UUserWidget>(GetWorld(), m_DeathScreenUIClass);
    }
    if (!m_DeathScreenUI) return;

    m_DeathScreenUI->AddToViewport();
    if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
    {
        PC->bShowMouseCursor = true;
        PC->SetInputMode(FInputModeUIOnly());
    }
    GetWorld()->GetTimerManager().SetTimer(m_LoadLevelTimerHandle, FTimerDelegate::CreateUObject(this, &AGameManager::LoadMainMenu), 3.0f, false);
}

void AGameManager::ShowWinScreen()
{
    if (m_WinScreenUIClass && !m_WinScreenUI)
    {
        m_WinScreenUI = CreateWidget<UUserWidget>(GetWorld(), m_WinScreenUIClass);
    }
    if (!m_WinScreenUI) return;

    m_WinScreenUI->AddToViewport();
    if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
    {
        PC->bShowMouseCursor = true;
        PC->SetInputMode(FInputModeUIOnly());
    }
    GetWorld()->GetTimerManager().SetTimer(m_LoadLevelTimerHandle, FTimerDelegate::CreateUObject(this, &AGameManager::LoadMainMenu), 3.0f, false);
}

void AGameManager::ShowFailScreen()
{
    if (m_FailScreenUIClass && !m_FailScreenUI)
    {
        m_FailScreenUI = CreateWidget<UUserWidget>(GetWorld(), m_FailScreenUIClass);
    }
    if (!m_FailScreenUI) return;

    m_FailScreenUI->AddToViewport();
    if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
    {
        PC->bShowMouseCursor = true;
        PC->SetInputMode(FInputModeUIOnly());
    }
    GetWorld()->GetTimerManager().SetTimer(m_LoadLevelTimerHandle, FTimerDelegate::CreateUObject(this, &AGameManager::LoadMainMenu), 3.0f, false);
}

void AGameManager::MuteBackgroundMusic()
{
    TArray<AActor*> found;
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("BGMusic"), found);
    if (found.Num() == 0) return;

    if (UAudioComponent* ac = found[0]->FindComponentByClass<UAudioComponent>())
    {
        ac->SetVolumeMultiplier(0.0f);
    }
}

void AGameManager::LoadMainMenu()
{
    UGameplayStatics::OpenLevel(GetWorld(), FName("MainMenu"));
}

