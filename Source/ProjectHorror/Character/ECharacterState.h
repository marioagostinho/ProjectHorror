#pragma once

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	ECS_Normal UMETA(DisplayName = "Normal"),
	ECS_Immobilized UMETA(DisplayName = "Immobilized"),
	ECS_Inspecting UMETA(DisplayName = "Inspecting")
};