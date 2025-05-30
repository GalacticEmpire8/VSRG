#pragma once

#include "CoreMinimal.h"
#include "WeaponData.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct VSRG_API FWeaponData : public FTableRowBase
{
	GENERATED_BODY()
public:
    // Level values
    UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Level1", MakeStructureDefaultValue = "0"))
    int32 Level1;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Level2", MakeStructureDefaultValue = "0"))
    int32 Level2;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Level3", MakeStructureDefaultValue = "0"))
    int32 Level3;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Level4", MakeStructureDefaultValue = "0"))
    int32 Level4;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Level5", MakeStructureDefaultValue = "0"))
    int32 Level5;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Level6", MakeStructureDefaultValue = "0"))
    int32 Level6;
};
