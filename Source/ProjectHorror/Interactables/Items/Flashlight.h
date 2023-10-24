#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Flashlight.generated.h"

class USpotLightComponent;
class UStaticMeshComponent;
UCLASS()
class PROJECTHORROR_API AFlashlight : public AItem
{
	GENERATED_BODY()
	
public:	
	AFlashlight();

	void ToggleFlashlight();

	//GETTERS
	FORCEINLINE bool GetFlashlightState() { return FlashlightState; }
	FORCEINLINE USpotLightComponent* GetSpotLight() { return SpotLight; }
	
	//SETTERS
	FORCEINLINE void SetFlashlightState(bool bNewState) { FlashlightState = bNewState; }

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
private:
	//COMPONENTS
	UPROPERTY(Replicated, VisibleAnywhere)
	USpotLightComponent* SpotLight;

	//FLASHLIGHTSTATE (TRUE = ON, FALSE = OFF)
	UPROPERTY(ReplicatedUsing = OnRep_FlashlightState, VisibleAnywhere)
	bool FlashlightState;

	//REPLICATION
	UFUNCTION()
	void OnRep_FlashlightState();

	//SERVER
	UFUNCTION(Server, Reliable)
	void ServerToggleFlashlight(const bool bFlashlightState);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastToggleFlashlight(const bool bFlashlightState);
};
