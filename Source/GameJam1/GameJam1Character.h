// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "Animation/AnimSequence.h"
#include "GameJam1Character.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AGameJam1Character : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	//Animations
	UPROPERTY(EditAnywhere)
	UAnimSequence* m_IdleAnim = nullptr;

	UPROPERTY(EditAnywhere)
	UAnimSequence* m_WalkAnim = nullptr;

	UPROPERTY(EditAnywhere)
	UAnimSequence* m_FallAnim = nullptr;

	UPROPERTY(EditAnywhere)
	UAnimSequence* m_JumpAnim = nullptr;

	UPROPERTY(EditAnywhere)
	UAnimSequence* m_DeathAnim = nullptr;

	UPROPERTY(EditAnywhere)
	UAnimSequence* m_HitAnim = nullptr;

	UPROPERTY(EditAnywhere)
	float m_WalkSpeedThreshold = 150.0f;

	UAnimSequence* m_CurrentAnim = nullptr;

	// player health

	UPROPERTY(EditAnywhere, Category = "Health")
	int32 m_MaxHealth = 3;

	UPROPERTY(VisibleAnywhere, Category = "Health")
	int32 m_Health = 3;

	UPROPERTY(EditAnywhere, Category = "Health")
	float HitCooldown = 0.6f; 

	UPROPERTY(EditAnywhere, Category = "Health|Knockback")
	float HitBackImpulse = 800.0f;   

	UPROPERTY(EditAnywhere, Category = "Health|Knockback")
	float HitUpImpulse = 500.0f; 

public: 

	UFUNCTION()
	void ApplyHit(AActor* Source);

	// override methods

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void Jump() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	AGameJam1Character();
	

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
			

protected:

	virtual void NotifyControllerChanged() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	int32 GetPlayerMaxHP() { return m_MaxHealth; }
	int32 GetPlayerCurrentHP() { return m_Health;  }

	float LastHitTime = -1000.0f;
	void Die();
};

