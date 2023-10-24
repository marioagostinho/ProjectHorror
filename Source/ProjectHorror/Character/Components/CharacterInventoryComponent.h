#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterInventoryComponent.generated.h"

class UCharacterInventoryWidget;
class AItem;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTHORROR_API UCharacterInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCharacterInventoryComponent();

	// REPLICATION
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//ACCESS INVENTORY
	void AddItem(AItem* Item, ACharacterController* Character);
	void RemoveItem(AItem* Item, ACharacterController* Character);
	AItem* GetItemWithIndex(int32 index);

	//GETTERS
	FORCEINLINE UCharacterInventoryWidget* GetInventoryWidget() { return InventoryWidget; }

	//SETTERS
	FORCEINLINE int32 GetMaxItems() { return MaxItems; }

protected:
	virtual void BeginPlay() override;

private:
	//ARRAY OF THE ITEMS IN THE INVENTORY
	UPROPERTY(Replicated, VisibleAnywhere)
	TArray<AItem*> Items;
	
	//MAX ITEMS
	int32 MaxItems;

	//HUD
	UPROPERTY(EditAnywhere, Category = "HUD")
	TSubclassOf<UCharacterInventoryWidget> InventoryWidgetClass;
	UPROPERTY(VisibleAnywhere, Category = "HUD")
	UCharacterInventoryWidget* InventoryWidget;

	//SERVER FUNCTIONS
	UFUNCTION(Server, Reliable)
	void ServerAddItem(AItem* Item, ACharacterController* Character);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastAddItem(AItem* Item, ACharacterController* Character);
	UFUNCTION(Server, Reliable)
	void ServerRemoveItem(AItem* Item);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRemoveItem(AItem* Item);
};
