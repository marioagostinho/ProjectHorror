#include "CharacterInteractionComponent.h"

#include "../CharacterController.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "ProjectHorror/HUD/CharacterCrosshairWidget.h"
#include "ProjectHorror/HUD/CharacterInteractionWidget.h"
#include "ProjectHorror/Interactables/Interactions/Interaction.h"
#include "ProjectHorror/Interactables/Items/Flashlight.h"

UCharacterInteractionComponent::UCharacterInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
	//REPLICATION
	SetIsReplicatedByDefault(true);
}

void UCharacterInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	if (Character == nullptr || !Character->IsLocallyControlled()) return;
	
	//INITIALIZE INTERACTION HUD
	if (InteractionWidgetClass == nullptr) return;

	InteractionWidget = CreateWidget<UCharacterInteractionWidget>(GetWorld(), InteractionWidgetClass);

	if (InteractionWidget == nullptr) return;
	
	InteractionWidget->SetVisibility(ESlateVisibility::Hidden);
	InteractionWidget->AddToViewport();
	
	//INITIALIZE CROSSHAIR HUD
	if (CrosshairWidgetClass == nullptr) return;
	
	CrosshairWidget = CreateWidget<UCharacterCrosshairWidget>(GetWorld(), CrosshairWidgetClass);

	if(CrosshairWidget == nullptr) return;
	
	CrosshairWidget->SetVisibility(ESlateVisibility::Visible);
	CrosshairWidget->AddToViewport();

	//INITIALIZE INSPECT HUD
	if (InspectWidgetClass == nullptr) return;

	InspectWidget = CreateWidget<UUserWidget>(GetWorld(), InspectWidgetClass);

	if(InspectWidget == nullptr) return;
	
	InspectWidget->SetVisibility(ESlateVisibility::Hidden);
	InspectWidget->AddToViewport();
}

void UCharacterInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (Character->GetCharacterState() != ECharacterState::ECS_Normal)
	{
		InteractionWidget->SetText(false);
		CrosshairWidget->SetCrosshairOpacityByInteraction(false);
	}
	else
	{
		//TRACE A LINE TO CHECK IF IS HITTING AN INTERACTABLE
		TraceHitLineIteractable();
	}
}

//CREATE A TRACE LINE TO CHECK IF IS HITTING AN INTERACTABLE
void UCharacterInteractionComponent::TraceHitLineIteractable()
{
	if (Character == nullptr || Character->GetController() == nullptr) return;
	
	//GET CAMERA LOCATION AND ROTATION
	FVector CameraLocation;
	FRotator CameraRotation;
	Character->GetController()->GetPlayerViewPoint(CameraLocation, CameraRotation);

	//CALCULATE WHERE WOULD THE TRACE END BASED ON THE CAMERA LOCATION, ROTATION AND THE TRACE DISTANCE
	FVector TraceEnd = CameraLocation + (CameraRotation.Vector() * TraceDistance);

	//HIT RESULT
	FHitResult HitResult;

	FCollisionQueryParams TraceParams(
		FName(TEXT("CameraTrace")),
		false,
		Character
	);
	
	//TRACE
	bool bHitSomething = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		CameraLocation,
		TraceEnd,
		ECollisionChannel::ECC_Visibility,
		TraceParams
	);

	//CHECK IF HIT SOMETHING
	if (bHitSomething == false)
	{
		SetHitInteractable(nullptr);
	}
	else if(HitResult.GetActor())
	{
		//IF THE TRACE IS HITTING AN AACTOR AND THE AACTOR HAS THE CLASS AITEM RETURN 
		if (AInteractable* Interactable = Cast<AInteractable>(HitResult.GetActor()))
		{
			SetHitInteractable(Interactable);
		}
		else if (Interactable == nullptr)
		{
			SetHitInteractable(nullptr);
		}	
	}
}

//HIT w/ SERVER AND MULTICAST
void UCharacterInteractionComponent::SetHitInteractable(AInteractable* Interactable)
{
	//SEND HIT INTERACTABLE TO SERVER
	ServerHandleHitInteractable(Interactable);

	//SET INTERACTION UI BASE ON THE HIT INTERACTABLE
	if (InteractionWidget == nullptr || !Character->IsLocallyControlled()) return;
	
	if (Interactable && Interactable->Interaction)
	{
		InteractionWidget->SetText(true, Interactable->Interaction->GetDescription());
		CrosshairWidget->SetCrosshairOpacityByInteraction(true);
	}
	else
	{
		InteractionWidget->SetText(false);
		CrosshairWidget->SetCrosshairOpacityByInteraction(false);
	}
}

//SERVER SET HIT INTERACTABLE
void UCharacterInteractionComponent::ServerHandleHitInteractable_Implementation(AInteractable* Interactable)
{
	MulticastHandleHitInteractable(Interactable);
}

void UCharacterInteractionComponent::MulticastHandleHitInteractable_Implementation(AInteractable* Interactable)
{
	InteractableInTrace = Interactable;
}





