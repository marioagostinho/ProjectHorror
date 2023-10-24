#pragma once

#include "CoreMinimal.h"
#include "Interaction.h"
#include "PickInteraction.generated.h"

UCLASS()
class PROJECTHORROR_API UPickInteraction : public UInteraction
{
	GENERATED_BODY()

public:
	virtual void Interact(ACharacterController* Character, AInteractable* Interactable) override;
};
