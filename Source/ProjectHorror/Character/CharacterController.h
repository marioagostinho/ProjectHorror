#pragma once

#include "CoreMinimal.h"
#include "ECharacterState.h"
#include "GameFramework/Character.h"
#include "CharacterController.generated.h"

class UCharacterStaminaComponent;
class UCharacterInventoryComponent;
class UCharacterInteractionComponent;
class UCharacterStaminaWidget;
class UCameraComponent;

UCLASS()
class PROJECTHORROR_API ACharacterController : public ACharacter
{
	GENERATED_BODY()

public:
	ACharacterController();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents() override;

	// REPLICATION
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	// GETTERS
	FORCEINLINE float GetYawMovementValue() const { return YawMovementValue; }
	FORCEINLINE float GetPitchMovementValue() const { return PitchMovementValue; }
	FORCEINLINE float GetBaseWalkSpeed() const { return BaseWalkSpeed; }
	FORCEINLINE float GetAO_Pitch() const { return AO_Pitch; }
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }
	FORCEINLINE UCameraComponent* GetCamera() const { return Camera; }
	FORCEINLINE UCharacterInventoryComponent* GetCharacterInventory() const { return CharacterInventory; }
	FORCEINLINE UCharacterInteractionComponent* GetCharacterInteraction() const { return CharacterInteraction; }
	FORCEINLINE int GetInventoryIndex() const { return InventoryIndex; }

	//SETTERS
	FORCEINLINE void SetCharacterState(ECharacterState NewState) { CharacterState = NewState; }
	void SetInventoryIndex(int NewValue);
	void SetMaxWalkSpeed(float MaxWalkSpeed);

protected:
	virtual void BeginPlay() override;

	/* SERVER FUNCTIONS */
	
	// YAW MOVEMENT
	UFUNCTION(Server, Reliable)
	void ServerSetYawMovementValue(const float YawValue);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastSetYawMovementValue(const float YawValue);
	// PITCH MOVEMENT
	UFUNCTION(Server, Reliable)
	void ServerSetPitchMovementValue(const float PitchValue);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastSetPitchMovementValue(const float PitchValue);
	//SPRINTING
	UFUNCTION(Server, Reliable)
	void ServerSetMaxWalkSpeed(const float MaxWalkSpeed);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastSetMaxWalkSpeed(const float MaxWalkSpeed);
	//AIM OFFSET
	UFUNCTION(Server, Reliable)
	void ServerSetAO_Pitch(const float AOPitch);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastSetAO_Pitch(const float AOPitch);

	//MOVEMENT FUNCTIONS
	void MoveFoward(float Value);
	void MoveRight(float Value);
	void Turn(float Value);
	void LookUp(float Value);

	void CrouchButtonPressed();
	void SprintButtonPressed();
	void SprintButtonReleased();

	//INVENTORY INPUT FUNCTIONS
	void InventoryKeyboard(float Value);
	void InventoryMouse(float Value);

	//INTERACTIONS INPUT FUNCTIONS
	void FlashlightButtonPressed();
	void UnpickedInteractionButtonPressed();
	void EquippedInteractionButtonPressed();
	void EquippedInteractionButtonReleased();
	void DropInteractionButtonPressed();

	//MOVEMENT PROPERTIES
	float BaseWalkSpeed = 200.f;
	float BaseSprintSpeed = 500.f;
	float const BaseInterpSpeed = 50.f;

	//INVENTORY
	int32 InventoryIndex = 1;

	//AIM OFFSET
	void AimOffset(float DeltaTime);

private:
	//COMPONENTS
	UPROPERTY(VisibleAnywhere, Category = Camera)
	UCameraComponent* Camera;

	//STAMINA
	UPROPERTY(VisibleAnywhere, Category = Stamina)
	UCharacterStaminaComponent* CharacterStamina;
	
	//INTERACTION
	UPROPERTY(VisibleAnywhere, Category = Interaction)
	UCharacterInteractionComponent* CharacterInteraction;

	//INVENTORY
	UPROPERTY(VisibleAnywhere, Category = Inventory)
	UCharacterInventoryComponent* CharacterInventory;
	
	//LOCK CHARACTER
	ECharacterState CharacterState = ECharacterState::ECS_Normal;
	
	//MOVEMENT
	float YawMovementValue = 0.f;
	float PitchMovementValue = 0.f;

	//AIM OFFSET
	UPROPERTY(VisibleAnywhere)
	float AO_Pitch;
};
