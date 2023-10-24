#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterInteractionWidget.generated.h"

class ACharacterController;
class UTextBlock;
UCLASS()
class PROJECTHORROR_API UCharacterInteractionWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	//PROPERTIES
	UPROPERTY(meta = (BindWidget))
	UTextBlock* InteractionText;

	//SET UI TEXT
	void SetText(bool bIsVisible, FString NewText = FString(""));
};
