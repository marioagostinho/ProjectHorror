#include "Interactable.h"

#include "Components/WidgetComponent.h"
#include "Interactions/Interaction.h"
#include "ProjectHorror/HUD/CharacterInteractionWidget.h"

AInteractable::AInteractable()
{
	PrimaryActorTick.bCanEverTick = true;
	
	//REPLICATE ACTOR AND ACTOR'S MOVEMENT
	bReplicates = bIsReplicated;
	AActor::SetReplicateMovement(bIsReplicated);

	//COMPONENTS
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetCollisionResponseToChannels(ECollisionResponse::ECR_Block);
	Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetRootComponent(Mesh);
}

void AInteractable::BeginPlay()
{
	Super::BeginPlay();

	if(PrimaryInteractionClass == nullptr) return;

	Interaction = NewObject<UInteraction>(this, PrimaryInteractionClass);
}

void AInteractable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

