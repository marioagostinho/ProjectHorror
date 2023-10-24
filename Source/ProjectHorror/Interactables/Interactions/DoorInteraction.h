#pragma once

#include "CoreMinimal.h"
#include "Interaction.h"
#include "DoorInteraction.generated.h"

UCLASS()
class PROJECTHORROR_API UDoorInteraction : public UInteraction
{
	GENERATED_BODY()

public:
	virtual void Interact(ACharacterController* Character, AInteractable* Interactable) override;

private:
	UFUNCTION(Server, Reliable)
	void ServerInteract(ACharacterController* Character, AItem* Item);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastInteract(ACharacterController* Character, AItem* Item);
};
