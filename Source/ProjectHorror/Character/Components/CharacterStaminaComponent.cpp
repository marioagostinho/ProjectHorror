#include "CharacterStaminaComponent.h"

#include "../CharacterController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ProjectHorror/HUD/CharacterStaminaWidget.h"

UCharacterStaminaComponent::UCharacterStaminaComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	if (ACharacterController* CharacterController = Cast<ACharacterController>(GetOwner()))
	{
		Character = CharacterController;
	}
}

void UCharacterStaminaComponent::BeginPlay()
{
	Super::BeginPlay();

	//HUD
	if (Character->IsLocallyControlled())
	{
		//STAMINA HUD
		if (StaminaWidgetClass == nullptr) return;
	
		StaminaWidget = CreateWidget<UCharacterStaminaWidget>(GetWorld(), StaminaWidgetClass);

		if(StaminaWidget == nullptr) return;
	
		StaminaWidget->SetVisibility(ESlateVisibility::Hidden);
		StaminaWidget->AddToViewport();
	}
}

void UCharacterStaminaComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (Character == nullptr) return;
	
	if (Character->GetCharacterMovement()->MaxWalkSpeed > Character->GetBaseWalkSpeed() && ActualExhaustedTimer <= 0 && Character->GetCharacterState() == ECharacterState::ECS_Normal)
	{
		StaminaDecrease(DeltaTime);
	}
	else if (Stamina < MaxStamina && ActualExhaustedTimer <= 0)
	{
		StaminaIncrease(DeltaTime);
	}

	if (StaminaWidget == nullptr) return;

	// HIDE STAMINA WIDGET IF HAS MAX STAMINA AND IS VISIBLE
	if (Stamina >= 100 && StaminaWidget->GetVisibility() == ESlateVisibility::Visible)
	{
		StaminaWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	// EXHAUSTED SYSTEM
	if (ActualExhaustedTimer > 0)
	{
		ExhaustTimer(DeltaTime);
	}
}

void UCharacterStaminaComponent::StaminaIncrease(float DeltaTime)
{
	if (Stamina >= MaxStamina) return;

	const float NewStamina = Stamina + (StaminaIncreaser * DeltaTime);
	Stamina = FMath::Clamp(NewStamina, 0, MaxStamina);

	if (StaminaWidget == nullptr) return;

	if (StaminaWidget->GetVisibility() != ESlateVisibility::Visible)
	{
		StaminaWidget->SetVisibility(ESlateVisibility::Visible);
	}

	StaminaWidget->SetProgressBarPercentage(Stamina);
}

void UCharacterStaminaComponent::StaminaDecrease(float DeltaTime)
{
	if (Character->GetCharacterMovement() == nullptr) return;

	// CHECK IF STAMINA ENDED
	if (Stamina <= 0)
	{
		// CHANGE SPEED BACK TO WALK SPEED
		Character->SetMaxWalkSpeed(Character->GetBaseWalkSpeed());

		//SET UP EXHAUSTED TIMER
		ActualExhaustedTimer = ExhaustedTimer;

		return;
	}

	const float NewStamina = Stamina - (StaminaDecreaser * DeltaTime);
	Stamina = FMath::Clamp(NewStamina, 0, MaxStamina);

	if (StaminaWidget == nullptr) return;

	if (StaminaWidget->GetVisibility() != ESlateVisibility::Visible)
	{
		StaminaWidget->SetVisibility(ESlateVisibility::Visible);
	}

	StaminaWidget->SetProgressBarPercentage(Stamina);
}

void UCharacterStaminaComponent::ExhaustTimer(float DeltaTime)
{
	const float NewActualExhaustedTimer = ActualExhaustedTimer - DeltaTime;
	ActualExhaustedTimer = FMath::Clamp(NewActualExhaustedTimer, 0, ExhaustedTimer);
}

