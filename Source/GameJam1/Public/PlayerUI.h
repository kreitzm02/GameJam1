// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerUI.generated.h"

/**
 * 
 */
UCLASS()
class GAMEJAM1_API UPlayerUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void UpdateHealthUI(int32 a_health);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void UpdateCookieUI(int32 a_cookie);

};
