#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterAnimInstance.generated.h"

class ACharacterController;
UCLASS()
class PROJECTHORROR_API UCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

private:
	UPROPERTY(BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
	ACharacterController* Character;

	// MOVEMENT
	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float YawMovementValue = 0;
	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float PitchMovementValue = 0;
	float const MovementInterpSpeed = 20.f;
	
	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bIsInAir;
	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bIsCrouch;
	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bIsSprinting;

	//AIM OFFSET
	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float AO_Pitch;
};
