#include "CharacterInteractionWidget.h"

#include "Components/TextBlock.h"

void UCharacterInteractionWidget::SetText(bool bIsVisible, FString NewText)
{
	SetVisibility(bIsVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	
	if (InteractionText == nullptr) return;

	InteractionText->SetText(FText::FromString(NewText));
	
}
