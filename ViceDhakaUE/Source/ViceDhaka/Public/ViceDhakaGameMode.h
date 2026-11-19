#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ViceDhakaGameMode.generated.h"

UCLASS()
class VICEDHAKA_API AViceDhakaGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AViceDhakaGameMode();
    virtual void StartPlay() override;
};
