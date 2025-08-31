// Fill out your copyright notice in the Description page of Project Settings.


#include "CookieBehaviour.h"
#include "GameJam1Character.h"

// Sets default values
ACookieBehaviour::ACookieBehaviour()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    m_CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
    m_CollisionSphere->InitSphereRadius(25.f);
    m_CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    m_CollisionSphere->SetCollisionObjectType(ECC_WorldDynamic);
    m_CollisionSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
    m_CollisionSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    m_CollisionSphere->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);
    m_CollisionSphere->SetGenerateOverlapEvents(true);
    RootComponent = m_CollisionSphere;

    m_CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &ACookieBehaviour::OnOverlap);

    m_OrbMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OrbMesh"));
    m_OrbMesh->SetupAttachment(m_CollisionSphere);
    m_OrbMesh->SetRelativeScale3D(FVector(0.3f));
    m_OrbMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void ACookieBehaviour::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACookieBehaviour::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AGameJam1Character* player = Cast<AGameJam1Character>(OtherActor))
	{
		player->ChangeCookieAmount(m_CookieAmount);
		UE_LOG(LogTemp, Error, TEXT("Added Cookies"))
			Destroy();
	}
}

