#include "InspectInteraction.h"

#include "Blueprint/UserWidget.h"
#include "Components/SphereComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "ProjectHorror/Character/CharacterController.h"
#include "ProjectHorror/Character/Components/CharacterInteractionComponent.h"
#include "ProjectHorror/Interactables/Inspectable.h"

void UInspectInteraction::Interact(ACharacterController* Character, AInteractable* Interactable)
{
	Super::Interact(Character, Interactable);

	//CHECK CHARACTER'S STATE
	if (Character->GetCharacterState() == ECharacterState::ECS_Normal)
	{
		if (GetWorld() == nullptr || Character == nullptr || Character->GetMesh() == nullptr) return;
		if (!Character->IsLocallyControlled()) return;

		//SOCKET LOCATION
		const FTransform SocketTransform = Character->GetMesh()->GetSocketTransform("HeadItemSocket");
		
		if (InspectingItem = GetWorld()->SpawnActor<AInspectable>(Interactable->GetClass(), SocketTransform.GetLocation(), SocketTransform.Rotator()))
		{
			//ORIGINAL ITEM
			Interactable->GetMesh()->SetVisibility(false, true);

			//CHARACTER
			Character->SetCharacterState(ECharacterState::ECS_Inspecting);
			Character->GetCharacterInteraction()->SetInteractableInTrace(Interactable);
			Character->GetCharacterInteraction()->GetInspectWidget()->SetVisibility(ESlateVisibility::Visible);

			InspectingItem->GetInteractionSphere()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
	else if(Character->GetCharacterState() == ECharacterState::ECS_Inspecting)
	{
		//ORIGINAL ITEM
		Interactable->GetMesh()->SetVisibility(true, true);

		//CHARACTER
		InspectingItem->Destroy();

		//NEW ITEM
		Character->SetCharacterState(ECharacterState::ECS_Normal);
		Character->GetCharacterInteraction()->GetInspectWidget()->SetVisibility(ESlateVisibility::Hidden);
	}
}

//FUNCTIONS TO ROTATE ITEM
void UInspectInteraction::RotateItemX(float Value)
{
	if (!bCanRotate) return;
	
	InspectingItem->AddActorLocalRotation(FRotator(0,0,Value), false, nullptr, ETeleportType::TeleportPhysics);
}

void UInspectInteraction::RotateItemY(float Value)
{
	if (!bCanRotate) return;

	InspectingItem->AddActorLocalRotation(FRotator((Value * -1),0,0),false,nullptr, ETeleportType::TeleportPhysics);
}


