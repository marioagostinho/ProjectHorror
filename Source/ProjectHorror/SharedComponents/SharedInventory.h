#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SharedInventory.generated.h"

class AItem;
class ACharacterController;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTHORROR_API USharedInventory : public UActorComponent
{
	GENERATED_BODY()

public:	
	USharedInventory();

protected:
	//REPLICATION
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void OnRep_Items();

	UFUNCTION()
	void AddItem(ACharacterController* Character, AItem* Item);
	UFUNCTION()
	void RemoveItem(ACharacterController* Character, AItem* Item);

private:
	UPROPERTY(Replicated)
	TArray<AItem*> Items;

	//REPLICATION
	UFUNCTION(Server, Reliable)
	void Server_AddItem(AItem* Item);
	UFUNCTION(Server, Reliable)
	void Server_RemoveItem(AItem* Item);
};
