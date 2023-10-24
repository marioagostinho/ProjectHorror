#include "CharacterCrosshairWidget.h"

#include "Components/Image.h"

void UCharacterCrosshairWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UCharacterCrosshairWidget::SetCrosshairOpacityByInteraction(bool bIsInteracting)
{
	//CREATE THE BRUSH FOR THE IMAGE
	FSlateBrush Brush = Crosshair->Brush;

	//SET TINTCOLOR DEPENDING IF IS INTERACTING OR NOT
	Brush.TintColor = (bIsInteracting) 
		? FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f))
		: FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f, 0.7f));

	//SET THE IMAGE BRUSH
	Crosshair->SetBrush(Brush);
}
