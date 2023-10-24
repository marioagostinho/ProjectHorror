#include "FlashlightInteraction.h"

#include "../Items/Flashlight.h"

void UFlashlightInteraction::Interact(ACharacterController* Character, AInteractable* Interactable)
{
	Super::Interact(Character, Interactable);

	if (AFlashlight* Flashlight = Cast<AFlashlight>(Interactable))
	{
		Flashlight->ToggleFlashlight();
	}
}
