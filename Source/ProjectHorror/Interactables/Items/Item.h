#pragma once

#include "CoreMinimal.h"
#include "EItemState.h"
#include "EItemType.h"
#include "ProjectHorror/Interactables/Interactable.h"
#include "Item.generated.h"

class UPhysicsHandleComponent;
class UWidgetComponent;
class USphereComponent;
class UInteraction;
UCLASS()
class PROJECTHORROR_API AItem : public	AInteractable
{
	GENERATED_BODY()
	
public:	
	AItem();

	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//PROPERTIES
	UPROPERTY(EditAnywhere, Category = Item)
	EItemType Type;
	
	//DROP INTERACTION
	UPROPERTY(VisibleAnywhere)
	UInteraction* DropInteraction;

	//GETTERS
	FORCEINLINE FString GetFName() { return Name; }
	FORCEINLINE UTexture2D* GetIcon() const { return Icon; }
	FORCEINLINE UTexture2D* GetSelectedIcon() const { return SelectedIcon; }
	
	//SETTERS
	void SetState(EItemState NewState);

protected:
	virtual void BeginPlay() override;

	//COMPONENTS
	UPROPERTY(EditAnywhere)
	USphereComponent* InteractionSphere;
	
	//PROPERTIES
	UPROPERTY(Replicated, VisibleAnywhere, Category = Item)
	EItemState State;

	//ICONS
	UPROPERTY(EditAnywhere, Category = Item)
	UTexture2D* Icon;
	UPROPERTY(EditAnywhere, Category = Item)
	UTexture2D* SelectedIcon;

	//INTERACTIONS CLASS
	UPROPERTY(EditDefaultsOnly, Category = HUD)
	TSubclassOf<UInteraction> UnpickedInteractionClass;
	UPROPERTY(EditDefaultsOnly, Category = HUD)
	TSubclassOf<UInteraction> DropInteractionClass;

	//WILL CREATE INTERACTION BASE ON THE INTERACTION CLASS
	UInteraction* CreateInteraction(TSubclassOf<UInteraction> InteractionClass);

	//SERVER TO SET ITEM STATE OF THE ITEM
	UFUNCTION(Server, Reliable)
	void ServerSetState(EItemState NewState);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastSetState(EItemState NewState);
	
private:
	//SIMULATION TIMER
	float SimulationTimer = 0.f;
	float SimulationMaxTimer = 2.f;
};
