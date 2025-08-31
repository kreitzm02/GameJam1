// Fill out your copyright notice in the Description page of Project Settings.


#include "LoadingScreenMPSubSystem.h"
#include "Kismet/GameplayStatics.h"                 
#include "Widgets/SWeakWidget.h"          
#include "Widgets/Layout/SBorder.h"      
#include "Widgets/Images/SThrobber.h" 
#include "MoviePlayer.h"

ULoadingScreenMPSubsystem::ULoadingScreenMPSubsystem()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> Finder(TEXT("/Game/LoadingScreen/WBP_Loading.WBP_Loading_C"));
	if (Finder.Succeeded())
	{
		m_LoadingScreenWidgetClass = Finder.Class;
	}
}

void ULoadingScreenMPSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &ULoadingScreenMPSubsystem::HandlePreLoadMap);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &ULoadingScreenMPSubsystem::HandlePostLoadMap);
}

void ULoadingScreenMPSubsystem::Deinitialize()
{
	FCoreUObjectDelegates::PreLoadMap.RemoveAll(this);
	FCoreUObjectDelegates::PostLoadMapWithWorld.RemoveAll(this);
	Super::Deinitialize();
}

void ULoadingScreenMPSubsystem::StartLoadingScreen()
{
	FLoadingScreenAttributes attributes;
	attributes.bAutoCompleteWhenLoadingCompletes = true;
	attributes.MinimumLoadingScreenDisplayTime = 2.0f;
	m_OverlayWidget = CreateWidget<UUserWidget>(GetGameInstance(), m_LoadingScreenWidgetClass);
	attributes.WidgetLoadingScreen = m_OverlayWidget->TakeWidget();
	attributes.PlaybackType = EMoviePlaybackType::MT_LoadingLoop;

	GetMoviePlayer()->SetupLoadingScreen(attributes);
	GetMoviePlayer()->PlayMovie();
}

void ULoadingScreenMPSubsystem::HandlePreLoadMap(const FString& a_MapName)
{
	StartLoadingScreen();
}

void ULoadingScreenMPSubsystem::HandlePostLoadMap(UWorld* s_LoadedWorld)
{

}

