#include "Inspectable.h"

#include "Components/SphereComponent.h"

AInspectable::AInspectable()
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
}
