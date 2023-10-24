#pragma once

UENUM(BlueprintType)
enum class EItemState : uint8
{
	EIS_Dropped UMETA(DisplayName = "Dropped"),
	EIS_Unpicked UMETA(DisplayName = "Unpicked"),
	EIS_Equipped UMETA(DisplayName = "Equipped"),
	ETS_Unequipped UMETA(DisplayName = "Unequipped"),
	ETS_Inspected UMETA(DisplayName = "Inspected"),
	ETS_Destroyed UMETA(DisplayName = "Destroyed")
};
