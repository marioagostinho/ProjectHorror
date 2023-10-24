#include "DropInteraction.h"

#include "../Items/Item.h"
#include "ProjectHorror/Character/CharacterController.h"
#include "ProjectHorror/Character/Components/CharacterInventoryComponent.h"

void UDropInteraction::Interact(ACharacterController* Character, AInteractable* Interactable)
{
	Super::Interact(Character, Interactable);

	if (Character == nullptr || Character->GetCharacterInventory() == nullptr || Interactable == nullptr) return;

	if (AItem* Item = Cast<AItem>(Interactable))
	{
		Character->GetCharacterInventory()->RemoveItem(Item, Character);
	}
}