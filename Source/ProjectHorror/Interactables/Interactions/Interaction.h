#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Interaction.generated.h"

class AInteractable;
class ACharacterController;

UCLASS(Blueprintable, BlueprintType)
class PROJECTHORROR_API UInteraction : public UObject
{
	GENERATED_BODY()

public:
	virtual void Interact(ACharacterController* Character, AInteractable* Interactable);

	//GETTERS
	FString GetFName() { return Name; }
	FString GetDescription() { return Description; }

protected:
	UPROPERTY(EditDefaultsOnly)
	FString Name;
	UPROPERTY(EditDefaultsOnly)
	FString Description;
};
