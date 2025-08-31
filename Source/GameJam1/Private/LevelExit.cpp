// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelExit.h"
#include "GameJam1Character.h"
#include <Kismet/GameplayStatics.h>
#include <GameManager.h>

// Sets default values
ALevelExit::ALevelExit()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

    // Trigger Box
    m_BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
    m_BoxCollider->SetupAttachment(RootComponent);
    m_BoxCollider->SetBoxExtent(FVector(50.0f, 10.0f, 200.0f));
    m_BoxCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    m_BoxCollider->SetCollisionObjectType(ECC_WorldDynamic);
    m_BoxCollider->SetCollisionResponseToAllChannels(ECR_Ignore);
    m_BoxCollider->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    m_BoxCollider->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);
    m_BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &ALevelExit::OnOverlapBegin);

    // Visual Mesh (optional)
    m_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    m_Mesh->SetupAttachment(RootComponent);
    m_Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void ALevelExit::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALevelExit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALevelExit::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (AGameJam1Character* player = Cast<AGameJam1Character>(OtherActor))
    {
        if (player->GetPlayerCurrentCookies() >= 14)
        {
            TArray<AActor*> found;
            UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Manager"), found);
            if (found.Num() == 0) return;

            if (AGameManager* gm = Cast<AGameManager>(found[0]))
            {
                gm->ShowWinScreen();
            }
        }
        else
        {
            TArray<AActor*> found;
            UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Manager"), found);
            if (found.Num() == 0) return;

            if (AGameManager* gm = Cast<AGameManager>(found[0]))
            {
                gm->ShowFailScreen();
            }
        }
    }
}

