#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "Inspectable.generated.h"

class USphereComponent;

UCLASS()
class PROJECTHORROR_API AInspectable : public AInteractable
{
	GENERATED_BODY()

public:
	AInspectable();

	USphereComponent* GetInteractionSphere() { return InteractionSphere; }
	
protected:
	//COMPONENTS
	UPROPERTY(EditAnywhere)
	USphereComponent* InteractionSphere;
};
