#include "CharacterInventoryWidget.h"
#include "SlateCore.h"

#include "Components/Image.h"

void UCharacterInventoryWidget::NativeDestruct()
{
	RemoveFromParent();
	
	Super::NativeDestruct();
}

void UCharacterInventoryWidget::SetImage(int index, UTexture2D* NewImage)
{
	//CREATE THE BRUSH FOR THE IMAGE
	FSlateBrush Brush;

	if (NewImage != nullptr)
	{
		Brush.SetResourceObject(NewImage);
		Brush.TintColor = FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
	}
	else
	{
		Brush.TintColor = FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f, 0.0f));
	}

	//SET THE IMAGE BRUSH
	GetImageByNumber(index)->SetBrush(Brush);
}

UImage* UCharacterInventoryWidget::GetImageByNumber(int num)
{
	switch (num)
	{
		case 1:
			return FirstIcon;
		case 2:
			return  SecondIcon;
		case 3:
			return ThirdIcon;
		default:
			return nullptr;
	}
}

