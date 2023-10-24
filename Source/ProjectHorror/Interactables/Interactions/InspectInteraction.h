#pragma once

#include "CoreMinimal.h"
#include "Interaction.h"
#include "InspectInteraction.generated.h"

class AInspectable;
UCLASS()
class PROJECTHORROR_API UInspectInteraction : public UInteraction
{
	GENERATED_BODY()

public:
	virtual void Interact(ACharacterController* Character, AInteractable* Interactable) override;

	//FUNCTION TO ROTATE THE ITEM
	void RotateItemX(float Value);
	void RotateItemY(float Value);

	//SETTERS
	void SetCanRotate(bool NewRotate) { bCanRotate = NewRotate; }

private:
	//ITEM TO BE INSPECTED
	AInspectable* InspectingItem;

	//PROPERTIES
	bool bIsInteracting = false;
	bool bCanRotate = false;
};
