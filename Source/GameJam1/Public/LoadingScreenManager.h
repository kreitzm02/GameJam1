// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LoadingScreenManager.generated.h"

/**
 * 
 */
UCLASS()
class GAMEJAM1_API ULoadingScreenManager : public UObject
{
	GENERATED_BODY()

public:
	static ULoadingScreenManager* Get(UWorld* a_World);

	UFUNCTION()
	void StartLoading(UWorld* a_World);

	UFUNCTION()
	void EndLoading();

private:
	UPROPERTY()
	UUserWidget* m_LoadingWidget;

	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<UUserWidget> m_LoadingScreenWidgetClass;

	UPROPERTY()
	FTimerHandle m_EndLoadingDelayTimer;

	UPROPERTY()
	UWorld* m_World;

	UFUNCTION()
	void EndLoadingDelegate();

	
};
