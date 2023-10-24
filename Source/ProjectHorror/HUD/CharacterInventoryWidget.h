#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterInventoryWidget.generated.h"

class UImage;
UCLASS()
class PROJECTHORROR_API UCharacterInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UImage* FirstIcon;
	UPROPERTY(meta = (BindWidget))
    UImage* SecondIcon;
	UPROPERTY(meta = (BindWidget))
	UImage* ThirdIcon;

	void SetImage(int index, UTexture2D* NewImage);

protected:
	virtual void NativeDestruct() override;

private:
	UImage* GetImageByNumber(int num);
	
};
