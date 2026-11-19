#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ViceDhakaHUD.generated.h"

UCLASS()
class VICEDHAKA_API AViceDhakaHUD : public AHUD
{
    GENERATED_BODY()

public:
    virtual void DrawHUD() override;
};
