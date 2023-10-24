#include "CharacterController.h"

#include "Components/CharacterInteractionComponent.h"
#include "Components/CharacterInventoryComponent.h"
#include "Components/CharacterStaminaComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "ProjectHorror/HUD/CharacterInventoryWidget.h"
#include "ProjectHorror/Interactables/Interactions/InspectInteraction.h"
#include "ProjectHorror/Interactables/Interactions/Interaction.h"
#include "ProjectHorror/Interactables/Items/Item.h"

ACharacterController::ACharacterController()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	//COMPONENTS
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(GetMesh());

	CharacterStamina = CreateDefaultSubobject<UCharacterStaminaComponent>(TEXT("CharacterStamina"));
	CharacterStamina->SetIsReplicated(true);
	
	CharacterInteraction = CreateDefaultSubobject<UCharacterInteractionComponent>(TEXT("CharacterInteraction"));
	CharacterInteraction->SetIsReplicated(true);

	CharacterInventory = CreateDefaultSubobject<UCharacterInventoryComponent>(TEXT("CharacterInventory"));
	CharacterInventory->SetIsReplicated(true);
	
	//MOVEMENT
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
}

void ACharacterController::BeginPlay()
{
	Super::BeginPlay();
}

void ACharacterController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//PLAYER'S CAMERA
	if(Camera == nullptr || GetCharacterMovement() == nullptr) return;
	
	AimOffset(DeltaTime);
	
	// SET CAMERA'S LOCATION TO "HEADCAMERA" (SOCKET) LOCATION
	Camera->SetWorldLocation(GetMesh()->GetSocketLocation(FName("HeadCamera")));
}

// REPLICATION
void ACharacterController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

// SETUP INPUT
void ACharacterController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// MOVEMENT FUNCTIONS BINDING
	PlayerInputComponent->BindAxis("MoveFoward", this, &ACharacterController::MoveFoward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACharacterController::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &ACharacterController::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &ACharacterController::LookUp);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ACharacterController::CrouchButtonPressed);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ACharacterController::SprintButtonPressed);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ACharacterController::SprintButtonReleased);

	//INVENTORY INPUT FUNCTION w/ Keyboard and Mouse
	PlayerInputComponent->BindAxis("InventoryKeyboard", this, &ACharacterController::InventoryKeyboard);
	PlayerInputComponent->BindAxis("InventoryMouse", this, &ACharacterController::InventoryMouse);

	//INTERACTION INPUT FUNCTION
	PlayerInputComponent->BindAction("Flashlight", IE_Pressed, this, &ACharacterController::FlashlightButtonPressed);
	PlayerInputComponent->BindAction("UnpickedInteraction", IE_Pressed, this, &ACharacterController::UnpickedInteractionButtonPressed);
	PlayerInputComponent->BindAction("EquippedInteraction", IE_Pressed, this, &ACharacterController::EquippedInteractionButtonPressed);
	PlayerInputComponent->BindAction("EquippedInteraction", IE_Released, this, &ACharacterController::EquippedInteractionButtonReleased);
	PlayerInputComponent->BindAction("DropInteraction", IE_Pressed, this, &ACharacterController::DropInteractionButtonPressed);
}

void ACharacterController::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (CharacterInteraction == nullptr) return;

	CharacterInteraction->SetCharacter(this);
}

/* INPUT FUNCTIONS */
// MOVEMENT CHARACTER
void ACharacterController::MoveFoward(float Value)
{
	if (GetCharacterMovement() == nullptr || GetCharacterMovement()->IsFalling()) return;
	
	if (CharacterState != ECharacterState::ECS_Normal)
		Value = 0;
	
	if (YawMovementValue != Value)
	{
		YawMovementValue = Value;
		ServerSetYawMovementValue(Value);
	}
	
	if (Controller == nullptr || Value == 0.f) return;

	const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
	const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X));

	AddMovementInput(Direction, Value);
}

void ACharacterController::MoveRight(float Value)
{
	if (GetCharacterMovement() == nullptr || GetCharacterMovement()->IsFalling()) return;
	
	if (CharacterState != ECharacterState::ECS_Normal)
		Value = 0;
	
	if (PitchMovementValue != Value)
	{
		PitchMovementValue = Value;
		ServerSetPitchMovementValue(Value);
	}
	
	if (Controller == nullptr || Value == 0.f) return;

	const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
	const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y));

	AddMovementInput(Direction, Value);
}

// CHARACTER TURNING
void ACharacterController::Turn(float Value)
{
	if (Value == 0) return;
	
	if (CharacterState == ECharacterState::ECS_Normal)
	{
		AddControllerYawInput(Value);
	}
	else if (UInspectInteraction* InspectInteraction = Cast<UInspectInteraction>(CharacterInteraction->GetInteractableInTrace()->Interaction);
		CharacterState == ECharacterState::ECS_Inspecting && CharacterInteraction->GetInteractableInTrace() && InspectInteraction)
	{
		InspectInteraction->RotateItemX(Value);
	}
}

// CHARACTER LOOKING
void ACharacterController::LookUp(float Value)
{
	if (Camera == nullptr || Value == 0) return;
	
	if (CharacterState == ECharacterState::ECS_Normal)
	{
		const float CameraPitch = Camera->GetRelativeRotation().Pitch + Value;

		if (CameraPitch < 60 && CameraPitch > -70)
		{
			Camera->AddLocalRotation(FRotator(Value, 0, 0));
		}
	}
	else if (UInspectInteraction* InspectInteraction = Cast<UInspectInteraction>(CharacterInteraction->GetInteractableInTrace()->Interaction);
		CharacterState == ECharacterState::ECS_Inspecting && CharacterInteraction->GetInteractableInTrace() && InspectInteraction)
	{
		InspectInteraction->RotateItemY(Value);
	}
}

// CHARACTER CROUCHING
void ACharacterController::CrouchButtonPressed()
{
	if (CharacterState != ECharacterState::ECS_Normal) return;
	
	if (bIsCrouched)
	{
		UnCrouch();
	}
	else
	{
		Crouch();
	}
}

// CHARACTER SPRINTING
void ACharacterController::SprintButtonPressed()
{
	if (GetCharacterMovement() == nullptr) return;
	if (CharacterStamina->Stamina <= 0 || CharacterStamina->ActualExhaustedTimer > 0) return;

	if(bIsCrouched)
	{
		UnCrouch();
	}
	
	SetMaxWalkSpeed(BaseSprintSpeed);
}

void ACharacterController::SprintButtonReleased()
{
	if (GetCharacterMovement() == nullptr) return;

	SetMaxWalkSpeed(BaseWalkSpeed);
}

void ACharacterController::FlashlightButtonPressed()
{
	//if(CharacterInteraction == nullptr || CharacterInteraction->Flashlight == nullptr) return;

	//CharacterInteraction->Flashlight->ToggleFlashlight();
}

void ACharacterController::UnpickedInteractionButtonPressed()
{
	if (CharacterInteraction == nullptr ||
		CharacterInteraction->GetInteractableInTrace() == nullptr ||
		CharacterInteraction->GetInteractableInTrace()->Interaction == nullptr) return;
	
	CharacterInteraction->InteractableInTrace->Interaction->Interact(this, CharacterInteraction->InteractableInTrace);
}

void ACharacterController::EquippedInteractionButtonPressed()
{
	if (CharacterState == ECharacterState::ECS_Normal)
	{
		AItem* SelectedItem = CharacterInventory->GetItemWithIndex(InventoryIndex);
	
		if (CharacterInventory == nullptr ||
			SelectedItem == nullptr ||
			SelectedItem->Interaction == nullptr) return;

		SelectedItem->Interaction->Interact(this, SelectedItem);
	}
	else if (UInspectInteraction* InspectInteraction = Cast<UInspectInteraction>(CharacterInteraction->GetInteractableInTrace()->Interaction);
		CharacterState == ECharacterState::ECS_Inspecting && CharacterInteraction->GetInteractableInTrace() && InspectInteraction)
	{
		InspectInteraction->SetCanRotate(true);
	}
}

void ACharacterController::EquippedInteractionButtonReleased()
{
	if (CharacterState != ECharacterState::ECS_Inspecting && CharacterInteraction->GetInteractableInTrace() == nullptr) return;
	
	if (UInspectInteraction* InspectInteraction = Cast<UInspectInteraction>(CharacterInteraction->GetInteractableInTrace()->Interaction))
	{
		InspectInteraction->SetCanRotate(false);
	}
}

void ACharacterController::DropInteractionButtonPressed()
{
	if (CharacterState != ECharacterState::ECS_Normal) return;
	
	AItem* SelectedItem = CharacterInventory->GetItemWithIndex(InventoryIndex);

	if (CharacterInventory == nullptr ||
		SelectedItem == nullptr ||
		SelectedItem->DropInteraction == nullptr) return;

	SelectedItem->DropInteraction->Interact(this, SelectedItem);
}

void ACharacterController::InventoryKeyboard(float Value)
{
	if (CharacterInventory == nullptr || Value == 0.f || InventoryIndex == Value) return;
	if (CharacterState != ECharacterState::ECS_Normal) return;

	const int NewValue = FMath::RoundToInt32(Value);
	SetInventoryIndex(NewValue);
}

void ACharacterController::InventoryMouse(float Value)
{
	if (CharacterInventory == nullptr || Value == 0.f) return;
	if (CharacterState != ECharacterState::ECS_Normal) return;

	const int32 MaxItems = CharacterInventory->GetMaxItems();
	int NewValue = InventoryIndex + FMath::RoundToInt32(Value);

	if (NewValue > MaxItems)
	{
		NewValue = 1;
	}
	else if(NewValue < 1)
	{
		NewValue = MaxItems ;
	}

	SetInventoryIndex(NewValue);
}

void ACharacterController::SetInventoryIndex(int NewValue)
{
	if (CharacterInventory->GetItemWithIndex(InventoryIndex))
	{
		AItem* OldItem = CharacterInventory->GetItemWithIndex(InventoryIndex);

		if (OldItem->GetIcon() == nullptr) return;
		
		CharacterInventory->GetInventoryWidget()->SetImage(InventoryIndex, OldItem->GetIcon());
		OldItem->SetState(EItemState::ETS_Unequipped);
	}
	
	InventoryIndex = NewValue;

	AItem* Item = CharacterInventory->GetItemWithIndex(InventoryIndex);

	if (Item == nullptr) return;

	CharacterInventory->GetInventoryWidget()->SetImage(InventoryIndex, Item->GetSelectedIcon());

	Item->SetState(EItemState::EIS_Equipped);
}

//
void ACharacterController::SetMaxWalkSpeed(float MaxWalkSpeed)
{
	GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;

	if (HasNetOwner())
	{
		ServerSetMaxWalkSpeed(MaxWalkSpeed);
	}
}

// AIM OFFSET
void ACharacterController::AimOffset(float DeltaTime)
{
	AO_Pitch = GetBaseAimRotation().Pitch;
	
	if (AO_Pitch > 60 && !IsLocallyControlled())
	{
		FVector2d InRange(310.f, 360.f);
		FVector2d OutRange(-90.f, 0.f);

		AO_Pitch = FMath::GetMappedRangeValueClamped(InRange, OutRange, AO_Pitch);
	}

	if (HasNetOwner())
	{
		ServerSetAO_Pitch(AO_Pitch);
	}
}

/* SERVER FUNCTIONS */

// YAW MOVEMENT
void ACharacterController::ServerSetYawMovementValue_Implementation(const float YawValue)
{
	MulticastSetYawMovementValue(YawValue);
}

void ACharacterController::MulticastSetYawMovementValue_Implementation(const float YawValue)
{
	YawMovementValue = YawValue;
}

// PITCH MOVEMENT
void ACharacterController::ServerSetPitchMovementValue_Implementation(const float PitchValue)
{
	MulticastSetPitchMovementValue(PitchValue);
}

void ACharacterController::MulticastSetPitchMovementValue_Implementation(const float PitchValue)
{
	PitchMovementValue = PitchValue;
}

// CHANGE MAX WALKSPEED
void ACharacterController::ServerSetMaxWalkSpeed_Implementation(const float MaxWalkSpeed)
{
	MulticastSetMaxWalkSpeed(MaxWalkSpeed);
}

void ACharacterController::MulticastSetMaxWalkSpeed_Implementation(const float MaxWalkSpeed)
{
	GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
}

// AIM OFFSET
void ACharacterController::ServerSetAO_Pitch_Implementation(const float AOPitch)
{
	MulticastSetAO_Pitch(AOPitch);
}

void ACharacterController::MulticastSetAO_Pitch_Implementation(const float AOPitch)
{
	AO_Pitch = AOPitch;

	if (AO_Pitch > 60 && !IsLocallyControlled())
	{
		FVector2d InRange(310.f, 360.f);
		FVector2d OutRange(-90.f, 0.f);

		AO_Pitch = FMath::GetMappedRangeValueClamped(InRange, OutRange, AO_Pitch);
	}
}