// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/WidgetComponent.h>
#include "GameManager.generated.h"

UCLASS()
class GAMEJAM1_API AGameManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> m_DeathScreenUIClass;

	UPROPERTY()
	UUserWidget* m_DeathScreenUI;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> m_WinScreenUIClass;

	UPROPERTY()
	UUserWidget* m_WinScreenUI;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> m_FailScreenUIClass;

	UPROPERTY()
	UUserWidget* m_FailScreenUI;

	void ShowDeathScreen();

	void ShowWinScreen();

	void ShowFailScreen();

	UFUNCTION(BlueprintCallable)
	void MuteBackgroundMusic();

private:
	void LoadMainMenu();

	FTimerHandle m_LoadLevelTimerHandle;
};
