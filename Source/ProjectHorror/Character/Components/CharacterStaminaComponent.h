#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterStaminaComponent.generated.h"

class UCharacterStaminaWidget;
class ACharacterController;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTHORROR_API UCharacterStaminaComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	friend class ACharacterController;
	
	UCharacterStaminaComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

	//CHARACTER
	ACharacterController* Character;

	//STAMINA
	float Stamina = 100.f;
	float MaxStamina = 100.f;
	float const StaminaIncreaser = 10.f;
	float const StaminaDecreaser = 20.f;
	float ActualExhaustedTimer = 0.f;
	float ExhaustedTimer = 3.f;

	//STAMINA HUD
	UPROPERTY(EditAnywhere, Category = HUD)
	TSubclassOf<UUserWidget> StaminaWidgetClass;
	UPROPERTY(VisibleAnywhere, Category = HUD)
	UCharacterStaminaWidget* StaminaWidget;

	//STAMINA FUNCTIONS
	void StaminaIncrease(float DeltaTime);
	void StaminaDecrease(float DeltaTime);
	void ExhaustTimer(float DeltaTime);
};
