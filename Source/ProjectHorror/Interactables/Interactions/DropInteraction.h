#pragma once

#include "CoreMinimal.h"
#include "Interaction.h"
#include "DropInteraction.generated.h"

UCLASS()
class PROJECTHORROR_API UDropInteraction : public UInteraction
{
	GENERATED_BODY()
	
public:
	virtual void Interact(ACharacterController* Character, AInteractable* Interactable) override;
};
