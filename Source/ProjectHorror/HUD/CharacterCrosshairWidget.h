#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterCrosshairWidget.generated.h"

class UImage;
UCLASS()
class PROJECTHORROR_API UCharacterCrosshairWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UImage* Crosshair;

	void SetCrosshairOpacityByInteraction(bool bIsInteracting);

protected:
	virtual void NativeDestruct() override;
};
