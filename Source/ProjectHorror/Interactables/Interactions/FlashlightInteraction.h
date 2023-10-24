#pragma once

#include "CoreMinimal.h"
#include "Interaction.h"
#include "FlashlightInteraction.generated.h"

class AFlashlight;

UCLASS()
class PROJECTHORROR_API UFlashlightInteraction : public UInteraction
{
	GENERATED_BODY()

public:
	virtual void Interact(ACharacterController* Character, AInteractable* Interactable) override;
};
