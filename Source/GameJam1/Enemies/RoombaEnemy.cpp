// Fill out your copyright notice in the Description page of Project Settings.


#include "RoombaEnemy.h"
#include "DrawDebugHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "AIController.h"
#include "DrawDebugHelpers.h"

// Sets default values
ARoombaEnemy::ARoombaEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_RoombaMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RoombaMesh"));
	m_RoombaMesh->SetupAttachment(GetRootComponent());
	m_RoombaMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	m_DetectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DetectionSphere"));
	m_DetectionSphere->SetupAttachment(GetRootComponent());
	m_DetectionSphere->SetSphereRadius(m_DetectionRadius);

	m_DetectionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	m_DetectionSphere->SetCollisionObjectType(ECC_WorldDynamic);
	m_DetectionSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	m_DetectionSphere->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);

	m_FSMComponent = CreateDefaultSubobject<URoombaFSMComponent>(TEXT("RoombaFSM"));

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = AAIController::StaticClass();

	m_AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio"));
	m_AudioComp->SetupAttachment(GetRootComponent());
	m_AudioComp->bAutoActivate = false;
	m_AudioComp->bAutoDestroy = false;
}

// Called when the game starts or when spawned
void ARoombaEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	if (m_DetectionSphere)
	{
		m_DetectionSphere->SetSphereRadius(m_DetectionRadius);
		m_DetectionSphere->OnComponentBeginOverlap.AddDynamic(this, &ARoombaEnemy::OnDetectionBegin);
		m_DetectionSphere->OnComponentEndOverlap.AddDynamic(this, &ARoombaEnemy::OnDetectionEnd);
	}
}

// Called every frame
void ARoombaEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateViewCone();
}

void ARoombaEnemy::OnDetectionBegin(UPrimitiveComponent* a_OverlappedComp, AActor* a_OtherActor, UPrimitiveComponent* a_OtherComp, int32 a_OtherBodyIndex, bool a_FromSweep, const FHitResult& a_SweepResult)
{
	if (!a_OtherActor || a_OtherActor == this) return;
	m_OverlapSet.Add(a_OtherActor);
	m_OverlapDetectedActors = m_OverlapSet.Array();
}

void ARoombaEnemy::OnDetectionEnd(UPrimitiveComponent* a_OverlappedComp, AActor* a_OtherActor, UPrimitiveComponent* a_OtherComp, int32 a_OtherBodyIndex)
{
	if (!a_OtherActor) return;
	m_OverlapSet.Remove(a_OtherActor);
	m_OverlapDetectedActors = m_OverlapSet.Array();
}

void ARoombaEnemy::UpdateViewCone()
{
    m_ConeSet.Reset();

    UWorld* world = GetWorld();
    if (!world) return;

    const FVector origin = GetActorLocation();

    FVector fwd = GetActorForwardVector();
    fwd.Z = 0.f;
    if (!fwd.Normalize())
        return;

    const float halfAngleDeg = FMath::Clamp(m_ViewConeHalfAngleDeg, 0.0f, 179.0f);
    const float cosThreshold = FMath::Cos(FMath::DegreesToRadians(halfAngleDeg));

    float range = m_ViewConeRange;
    if (range <= 0.0f && m_DetectionSphere)
        range = m_DetectionSphere->GetScaledSphereRadius();

    const float rangeSq = (range > 0.0f) ? range * range : FLT_MAX;

    for (const TWeakObjectPtr<AActor>& a : m_OverlapDetectedActors)
    {
        AActor* other = a.Get();
        if (!other || other == this) continue;

        FVector to = other->GetActorLocation() - origin;
        to.Z = 0.f;
        const float distSq = to.SizeSquared();
        if (distSq < KINDA_SMALL_NUMBER || distSq > rangeSq) continue;

        to.Normalize();
        const float dot = FVector::DotProduct(fwd, to);

        if (dot >= cosThreshold)
        {
            m_ConeSet.Add(other);
        }
    }

    m_ConeDetectedActors = m_ConeSet.Array();

    // Debug
    if (m_DebugViewCone)
    {
        const FRotator yawRot = GetActorRotation();
        const FRotator leftRot(0.f, yawRot.Yaw - halfAngleDeg, 0.f);
        const FRotator rightRot(0.f, yawRot.Yaw + halfAngleDeg, 0.f);

        const FVector leftEnd = origin + leftRot.Vector() * (range > 0.f ? range : 900.f);
        const FVector rightEnd = origin + rightRot.Vector() * (range > 0.f ? range : 900.f);

        DrawDebugLine(world, origin, leftEnd, FColor::Cyan, false, 0.f, 0, 2.f);
        DrawDebugLine(world, origin, rightEnd, FColor::Cyan, false, 0.f, 0, 2.f);

        for (const TWeakObjectPtr<AActor>& a : m_ConeDetectedActors)
            if (a.IsValid())
                DrawDebugSphere(world, a->GetActorLocation(), 16.f, 12, FColor::Yellow, false, 0.f);
    }
}

void ARoombaEnemy::PlayStateLoop(USoundBase* a_Sound, float a_Volume, float a_Pitch)
{
	if (!m_AudioComp) return;

	if (!a_Sound) { m_AudioComp->Stop(); return; }

	if (m_AudioComp->Sound != a_Sound)
		m_AudioComp->SetSound(a_Sound);

	m_AudioComp->SetVolumeMultiplier(a_Volume);
	m_AudioComp->SetPitchMultiplier(a_Pitch);

	if (!m_AudioComp->IsPlaying())
		m_AudioComp->Play(0.f);
}

void ARoombaEnemy::StopStateAudio()
{
	if (m_AudioComp) m_AudioComp->Stop();
}

