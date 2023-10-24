#include "SharedInventory.h"

#include "Net/UnrealNetwork.h"
#include "ProjectHorror/Character/CharacterController.h"

USharedInventory::USharedInventory()
{
	PrimaryComponentTick.bCanEverTick = false;
	
	SetIsReplicated(true);
}

void USharedInventory::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USharedInventory, Items);
}

void USharedInventory::OnRep_Items()
{
}

void USharedInventory::AddItem(ACharacterController* Character, AItem* Item)
{
	if (!Character->HasAuthority())
	{
		Server_AddItem(Item);
		return;
	}

	Items.Add(Item);
}

void USharedInventory::Server_AddItem_Implementation(AItem* Item)
{
	Items.Add(Item);
}

void USharedInventory::RemoveItem(ACharacterController* Character, AItem* Item)
{
}

void USharedInventory::Server_RemoveItem_Implementation(AItem* Item)
{
}



