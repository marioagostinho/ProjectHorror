#include "DoorInteraction.h"

#include "ProjectHorror/Character/CharacterController.h"
#include "ProjectHorror/Interactables/Items/Item.h"


void UDoorInteraction::Interact(ACharacterController* Character, AInteractable* Interactable)
{
	Super::Interact(Character, Interactable);

	if (Character == nullptr || Character->GetCharacterInventory() == nullptr || Interactable == nullptr) return;

	if (AItem* Item = Cast<AItem>(Interactable))
	{
		ServerInteract(Character, Item);
	}
}

void UDoorInteraction::ServerInteract_Implementation(ACharacterController* Character, AItem* Item)
{
	MulticastInteract(Character, Item);
}

void UDoorInteraction::MulticastInteract_Implementation(ACharacterController* Character, AItem* Item)
{
}
