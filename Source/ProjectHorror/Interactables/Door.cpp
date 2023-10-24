#include "Door.h"

#include "Components/BoxComponent.h"
#include "Net/UnrealNetwork.h"

ADoor::ADoor()
{
	bReplicates = true;
	
	//COMPONENTS
	InteractionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionBox"));
	InteractionBox->SetCollisionResponseToChannels(ECollisionResponse::ECR_Ignore);
	InteractionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	InteractionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	InteractionBox->SetupAttachment(RootComponent);

	
}

void ADoor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADoor, bState);
}

void ADoor::OnRep_bState()
{
}

void ADoor::DoorInteraction()
{
	
}

void ADoor::ServerDoorInteraction_Implementation()
{
}

void ADoor::MulticastDoorInteraction_Implementation()
{
}


