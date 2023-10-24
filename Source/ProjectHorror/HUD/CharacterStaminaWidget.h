#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterStaminaWidget.generated.h"

class UProgressBar;
UCLASS()
class PROJECTHORROR_API UCharacterStaminaWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetProgressBarPercentage(float Stamina);

	UPROPERTY(meta = (BindWidget))
	UProgressBar* StaminaProgressBar;

protected:
	virtual void NativeDestruct() override;
};
