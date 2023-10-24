#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterInteractionComponent.generated.h"

class UCharacterCrosshairWidget;
class UWidgetComponent;
class UCharacterInteractionWidget;
class AInteractable;
class AItem;
class ACharacterController;
class AFlashlight;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTHORROR_API UCharacterInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCharacterInteractionComponent();
	
	friend class ACharacterController;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//GETTERS
	FORCEINLINE AInteractable* GetInteractableInTrace() { return InteractableInTrace; }
	FORCEINLINE UUserWidget* GetInspectWidget() { return InspectWidget; }
	
	//SETTERS
	FORCEINLINE void SetCharacter(ACharacterController* NewCharacter) { Character = NewCharacter; }
	FORCEINLINE void SetInteractableInTrace(AInteractable* NewInteractable) { InteractableInTrace = NewInteractable; }

protected:
	virtual void BeginPlay() override;

	void TraceHitLineIteractable();
	
private:
	//INTERACTION OWNER
	UPROPERTY(VisibleAnywhere)
	ACharacterController* Character;

	//PROPERTIES ITEM TO INTERACT
	UPROPERTY(EditDefaultsOnly)
	float TraceDistance = 150.f;
	UPROPERTY(VisibleAnywhere)
	AInteractable* InteractableInTrace;

	void SetHitInteractable(AInteractable* Interactable);

	//SERVER
	UFUNCTION(Server, Reliable)
	void ServerHandleHitInteractable(AInteractable* Interactable);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastHandleHitInteractable(AInteractable* Interactable);

	//HUD
	UPROPERTY(EditAnywhere, Category = "HUD")
	TSubclassOf<UCharacterInteractionWidget> InteractionWidgetClass;
	UPROPERTY(VisibleAnywhere, Category = "HUD")
	UCharacterInteractionWidget* InteractionWidget;

	//CROSSHAIR HUD
	UPROPERTY(EditAnywhere, Category = HUD)
	TSubclassOf<UCharacterCrosshairWidget> CrosshairWidgetClass;
	UPROPERTY(VisibleAnywhere, Category = HUD)
	UCharacterCrosshairWidget* CrosshairWidget;

	//INSPECT HUD
	UPROPERTY(EditAnywhere, Category = HUD)
	TSubclassOf<UUserWidget> InspectWidgetClass;
	UPROPERTY(VisibleAnywhere, Category = HUD)
	UUserWidget* InspectWidget;
};
