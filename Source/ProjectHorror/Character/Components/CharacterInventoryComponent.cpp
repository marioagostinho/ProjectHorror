#include "CharacterInventoryComponent.h"

#include "../CharacterController.h"
#include "Blueprint/UserWidget.h"
#include "Net/UnrealNetwork.h"
#include "ProjectHorror/HUD/CharacterInventoryWidget.h"
#include "ProjectHorror/Interactables/Items/Item.h"

UCharacterInventoryComponent::UCharacterInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	//REPLICATION
	SetIsReplicatedByDefault(true);

	//SET MAX ITEM THAT YOU CAN HAVE IN THE INVENTORY
	MaxItems = 3;

	//INITIALIZE THEM NONE
	for (int i = 0; i < MaxItems; i++)
	{
		Items.Add(nullptr);
	}
}

void UCharacterInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	//INITIALIZE HUD
	if (InventoryWidgetClass == nullptr) return;

	InventoryWidget = CreateWidget<UCharacterInventoryWidget>(GetWorld(), InventoryWidgetClass);

	if (InventoryWidget == nullptr) return;
	
	InventoryWidget->SetVisibility(ESlateVisibility::Visible);
	InventoryWidget->AddToViewport();
}

//REPLICATION
void UCharacterInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCharacterInventoryComponent, Items);
}

//ADDING ITEM w/ SERVER
void UCharacterInventoryComponent::AddItem(AItem* Item, ACharacterController* Character)
{
	if (Item == nullptr || Character == nullptr || Character->IsLocallyControlled() == false) return;
	if (Items.Find(nullptr) == INDEX_NONE) return;

	const int32 ItemIndex = Items.Find(nullptr) + 1;
	UTexture2D* Icon = (Character->GetInventoryIndex() == ItemIndex) ? Item->GetSelectedIcon() : Item->GetIcon();

	if (Icon == nullptr) return;

	InventoryWidget->SetImage(ItemIndex, Icon);
	
	ServerAddItem(Item, Character);
}

void UCharacterInventoryComponent::ServerAddItem_Implementation(AItem* Item, ACharacterController* Character)
{
	MulticastAddItem(Item, Character);
}

void UCharacterInventoryComponent::MulticastAddItem_Implementation(AItem* Item, ACharacterController* Character)
{
	//SET ITEM OWNER
	Item->SetOwner(Character);

	//ATTACH ITEM TO PLAYER
	const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	Item->AttachToComponent(Character->GetMesh(), AttachmentRules, FName("Item_RH"));

	//SET STATE
	Item->SetState(EItemState::ETS_Unequipped);
	
	//ADD TO ITEM TO INVENTORY
	const int32 ItemIndex = Items.Find(nullptr);
	Items[ItemIndex] = Item;

	//CHECK IF SELECT ITEM IN THE ONE JUST ADDED TO EQUIP
	if(Character->GetInventoryIndex() == ItemIndex + 1)
	{
		Item->SetState(EItemState::EIS_Equipped);
	}	
}

//REMOVING ITEM w/ SERVER
void UCharacterInventoryComponent::RemoveItem(AItem* Item, ACharacterController* Character)
{
	if (Item == nullptr || Character->IsLocallyControlled() == false) return;
	
	const int32 ItemIndex = Items.Find(Item) + 1;
	InventoryWidget->SetImage(ItemIndex, nullptr);	
	
	ServerRemoveItem(Item);
}

void UCharacterInventoryComponent::ServerRemoveItem_Implementation(AItem* Item)
{
	MulticastRemoveItem(Item);
}

void UCharacterInventoryComponent::MulticastRemoveItem_Implementation(AItem* Item)
{
	//REMOVE ITEM FROM INVENTORY
	const int32 ItemIndex = Items.Find(Item);
	Items[ItemIndex] = nullptr;

	//SET ITEM STATE
	Item->SetState(EItemState::EIS_Dropped);

	//DETACH ITEM FROM ACTOR
	Item->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
}

//GET ITEM DEPENDING ON THE SELECTED SLOT
AItem* UCharacterInventoryComponent::GetItemWithIndex(int32 index)
{
	if (index > MaxItems || index > Items.Num() || index < 1) return nullptr;
	
	return Items[index - 1];
}

