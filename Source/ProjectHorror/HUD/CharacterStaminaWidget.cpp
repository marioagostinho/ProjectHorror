#include "CharacterStaminaWidget.h"

#include "Components/ProgressBar.h"

void UCharacterStaminaWidget::NativeDestruct()
{
	RemoveFromParent();
	
	Super::NativeDestruct();
}

void UCharacterStaminaWidget::SetProgressBarPercentage(float Stamina)
{
	if (StaminaProgressBar == nullptr) return;
	
	StaminaProgressBar->SetPercent(Stamina / 100);
}
