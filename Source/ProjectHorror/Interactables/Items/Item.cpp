#include "Item.h"

#include "../Interactions/Interaction.h"
#include "Components/SphereComponent.h"

#include "Net/UnrealNetwork.h"

AItem::AItem()
{
	//SET MESH COMPONENT COLLISION RESPONSE
	Mesh->SetCollisionResponseToChannels(ECollisionResponse::ECR_Block);
	Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
	Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	Mesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	//SET INTERACTION SPHERE COMPONENT COLLISION RESPONSE
	InteractionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionSphere"));
	InteractionSphere->SetCollisionResponseToChannels(ECollisionResponse::ECR_Ignore);
	InteractionSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	InteractionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	InteractionSphere->SetupAttachment(RootComponent);

	//SET INITIAL STATE TO UNPICKED
	State = EItemState::EIS_Unpicked;
}

void AItem::BeginPlay()
{
	Super::BeginPlay();

	SetActorTickEnabled(false);

	//SET DEFAULT INTERACTION
	if (UnpickedInteractionClass == nullptr) return;

	Interaction = NewObject<UInteraction>(this, UnpickedInteractionClass);

	if (DropInteractionClass == nullptr) return;

	DropInteraction = NewObject<UInteraction>(this, DropInteractionClass);
}

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//IF THE STATE IS EIS_DROPPED TIME START TIMING TO CHANGE TO UNPICKED
	if (State != EItemState::EIS_Dropped) return;

	SimulationTimer += DeltaTime;

	if (SimulationTimer >= SimulationMaxTimer)
	{
		SetState(EItemState::EIS_Unpicked);
	}
}

//REPLICATION
void AItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AItem, State);
}

//WILL CREATE INTERACTION BASE ON THE INTERACTION CLASS
UInteraction* AItem::CreateInteraction(TSubclassOf<UInteraction> InteractionClass)
{
	if (InteractionClass)
	{
		return NewObject<UInteraction>(this, InteractionClass);
	}

	return nullptr;
}

//SET ITEM STATE w/ SERVER
void AItem::SetState(EItemState NewState)
{
	State = NewState;

	//ONLY GO TO SERVER IF IS OWNER OF THE CONNECTION
	if (HasNetOwner())
		ServerSetState(NewState);

	//CHANGE PROPERTIES DEPENDING ON THE ITEM STATE
	switch (State)
	{
		case EItemState::EIS_Dropped:
			SimulationTimer = 0.f;
			SetActorTickEnabled(true);
			Mesh->SetSimulatePhysics(true);
			SetActorHiddenInGame(false);
			Interaction = nullptr;
			break;
		case EItemState::EIS_Unpicked:
			SetActorTickEnabled(false);
			Mesh->SetSimulatePhysics(false);
			InteractionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			Interaction = CreateInteraction(UnpickedInteractionClass);
			SetOwner(nullptr);
		case EItemState::EIS_Equipped:
			SetActorHiddenInGame(false);
			break;
		case EItemState::ETS_Unequipped:
			InteractionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			SetActorHiddenInGame(true);
			Interaction = CreateInteraction(PrimaryInteractionClass);
			break;
		case EItemState::ETS_Destroyed:
			GEngine->AddOnScreenDebugMessage( -1,10.0,FColor::Yellow, "Destroyed" );
			break;
	}
}

void AItem::ServerSetState_Implementation(EItemState NewState)
{
	MulticastSetState(NewState);
}

void AItem::MulticastSetState_Implementation(EItemState NewState)
{
	State = NewState;

	//CHANGE PROPERTIES DEPENDING ON THE ITEM STATE
	switch (State)
	{
		case EItemState::EIS_Dropped:
			SimulationTimer = 0.f;
			SetActorTickEnabled(true);
			Mesh->SetSimulatePhysics(true);
			SetActorHiddenInGame(false);
			Interaction = nullptr;
			break;
		case EItemState::EIS_Unpicked:
			SetActorTickEnabled(false);
			Mesh->SetSimulatePhysics(false);
			InteractionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			Interaction = CreateInteraction(UnpickedInteractionClass);
			SetOwner(nullptr);
		case EItemState::EIS_Equipped:
			SetActorHiddenInGame(false);
			break;
		case EItemState::ETS_Unequipped:
			InteractionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			SetActorHiddenInGame(true);
			Interaction = CreateInteraction(PrimaryInteractionClass);
			break;
		case EItemState::ETS_Inspected:
			InteractionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			break;
		case EItemState::ETS_Destroyed:
			GEngine->AddOnScreenDebugMessage( -1,10.0,FColor::Yellow, "Destroyed" );
			break;
	}
}

