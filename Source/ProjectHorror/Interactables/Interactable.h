#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.generated.h"

class UWidgetComponent;
class UInteraction;
UCLASS()
class PROJECTHORROR_API AInteractable : public AActor
{
	GENERATED_BODY()
	
public:	
	AInteractable();
	
	virtual void Tick(float DeltaTime) override;

	//INTERACTION
	UPROPERTY(VisibleAnywhere)
	UInteraction* Interaction;

	//SETTERS
	FORCEINLINE UStaticMeshComponent* GetMesh() { return Mesh; };

protected:
	virtual void BeginPlay() override;

	//COMPONENTS
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;

	//PROPERTIES
	UPROPERTY(EditDefaultsOnly, Category = Item)
	FString Name;

	UPROPERTY(EditAnywhere, Category = Item)
	bool bIsReplicated;

	//INTERACTION CLASS
	UPROPERTY(EditDefaultsOnly, Category = Item)
	TSubclassOf<UInteraction> PrimaryInteractionClass;
};
