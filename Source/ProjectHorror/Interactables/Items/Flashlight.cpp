#include "Flashlight.h"

#include "Components/SpotLightComponent.h"
#include "Net/UnrealNetwork.h"

AFlashlight::AFlashlight()
{
	//REPLICATON
	bReplicates = true;

	//COMPONENTS
	SpotLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("PointLight"));
	SpotLight->SetupAttachment(RootComponent);

	FlashlightState = true;
}

void AFlashlight::BeginPlay()
{
	Super::BeginPlay();
}

//REPLICATION
void AFlashlight::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFlashlight, FlashlightState);
	DOREPLIFETIME(AFlashlight, SpotLight);
}

void AFlashlight::OnRep_FlashlightState()
{
	if (SpotLight == nullptr) return;
	
	SpotLight->SetVisibility(FlashlightState);
}

//TOGGLE TO TURN ON/OFF
void AFlashlight::ToggleFlashlight()
{
	FlashlightState = !FlashlightState;
	
	ServerToggleFlashlight(FlashlightState);
}

//SERVER
void AFlashlight::ServerToggleFlashlight_Implementation(const bool bFlashlightState)
{
	MulticastToggleFlashlight(bFlashlightState);
}

void AFlashlight::MulticastToggleFlashlight_Implementation(const bool bFlashlightState)
{
	if (SpotLight == nullptr) return;
	
	FlashlightState = bFlashlightState;
	SpotLight->SetVisibility(bFlashlightState);
}



