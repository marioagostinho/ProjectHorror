#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "Door.generated.h"

class UBoxComponent;
UCLASS()
class PROJECTHORROR_API ADoor : public AInteractable
{
	GENERATED_BODY()

public:
	ADoor();

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
private:
	//COMPONENTS
	UPROPERTY(EditAnywhere)
	UBoxComponent* InteractionBox;
	
	//PROPERTIES
	UPROPERTY(EditAnywhere, ReplicatedUsing = "OnRep_bState")
	bool bState;

	//REPLICATION
	UFUNCTION()
	void OnRep_bState();

	//OPEN DOOR INTERACTION w/ SERVER
	void DoorInteraction();
	UFUNCTION(Server, Reliable)
	void ServerDoorInteraction();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastDoorInteraction();
};
