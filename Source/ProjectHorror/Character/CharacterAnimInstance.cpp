#include "CharacterAnimInstance.h"

#include "CharacterController.h"
#include "GameFramework/CharacterMovementComponent.h"

void UCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Character = Cast<ACharacterController>(TryGetPawnOwner());
}

void UCharacterAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (Character == nullptr)
	{
		Character = Cast<ACharacterController>(TryGetPawnOwner());
	}

	if(Character == nullptr || Character->GetCharacterMovement() == nullptr) return;
	
	// GET MOVEMENT
	YawMovementValue = FMath::FInterpTo(YawMovementValue, Character->GetYawMovementValue(), DeltaTime, MovementInterpSpeed);
	PitchMovementValue = FMath::FInterpTo(PitchMovementValue, Character->GetPitchMovementValue(), DeltaTime, MovementInterpSpeed);
	
	// CHECK IF IS FALLING, SPRINTING OR CROUCH
	bIsInAir = Character->GetCharacterMovement()->IsFalling();
	bIsSprinting = (Character->GetCharacterMovement()->GetMaxSpeed() > Character->GetBaseWalkSpeed());
	bIsCrouch = Character->bIsCrouched;

	//AIM OFFSET
	AO_Pitch = Character->GetAO_Pitch();
}
