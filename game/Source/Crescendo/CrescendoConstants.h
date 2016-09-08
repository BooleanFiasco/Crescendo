#pragma once

UENUM(BlueprintType)
namespace ESwipeDirection
{
	enum Type
	{
		Up,
		UpRight,
		Right,
		DownRight,
		Down,
		DownLeft,
		Left,
		UpLeft,
		None
	};
}

UENUM(BlueprintType)
namespace ENavDirection
{
	enum Type
	{
		Up,
		Right,
		Down,
		Left,
		Forward,
		Back,
		Max
	};
}
