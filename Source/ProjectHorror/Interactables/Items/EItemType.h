#pragma once

UENUM(BlueprintType)
enum class EItemType : uint8
{
	EIT_Individual UMETA(DisplayName = "Individual"),
	EIS_Shared UMETA(DisplayName = "Shared")
};