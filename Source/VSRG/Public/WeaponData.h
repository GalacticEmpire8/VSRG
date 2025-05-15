#pragma once

#include "CoreMinimal.h"
#include "WeaponData.generated.h"

/** Please add a struct description */
USTRUCT(Blueprintable, BlueprintType)
struct VSRG_API FWeaponData : public FTableRowBase
{
	GENERATED_BODY()
public:
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Level1", MakeStructureDefaultValue = "0"))
	int32 Level1;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Level2", MakeStructureDefaultValue = "0"))
	int32 Level2;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Level3", MakeStructureDefaultValue = "0"))
	int32 Level3;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Level4", MakeStructureDefaultValue = "0"))
	int32 Level4;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Level5", MakeStructureDefaultValue = "0"))
	int32 Level5;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Level6", MakeStructureDefaultValue = "0"))
	int32 Level6;
};
