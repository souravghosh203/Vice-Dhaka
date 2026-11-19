#include "ViceDhakaGameMode.h"
#include "DhakaCityBlockout.h"
#include "RickshawPawn.h"
#include "Engine/World.h"
#include "ViceDhakaMissionSubsystem.h"
#include "DhakaWeatherController.h"
#include "Engine/GameInstance.h"
#include "ViceDhakaHUD.h"

AViceDhakaGameMode::AViceDhakaGameMode()
{
    DefaultPawnClass = ARickshawPawn::StaticClass();
    HUDClass = AViceDhakaHUD::StaticClass();
}

void AViceDhakaGameMode::StartPlay()
{
    Super::StartPlay();
    if (UWorld* World = GetWorld())
    {
        FActorSpawnParameters Params;
        Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        World->SpawnActor<ADhakaCityBlockout>(ADhakaCityBlockout::StaticClass(), FTransform::Identity, Params);
        World->SpawnActor<ADhakaWeatherController>(ADhakaWeatherController::StaticClass(), FTransform::Identity, Params);
    }

    if (UGameInstance* GameInstance = GetGameInstance())
    {
        if (UViceDhakaMissionSubsystem* Missions = GameInstance->GetSubsystem<UViceDhakaMissionSubsystem>())
        {
            FViceDhakaMission FirstJob;
            FirstJob.Type = EViceDhakaMissionType::BiryaniDelivery;
            FirstJob.Title = FText::FromString(TEXT("Biryani Dash"));
            FirstJob.Pickup = FVector(12000.0f, 0.0f, 0.0f);
            FirstJob.Destination = FVector(30000.0f, 18000.0f, 0.0f);
            FirstJob.RewardTaka = 220;
            FirstJob.TimeLimitSeconds = 240.0f;
            Missions->StartMission(FirstJob);
        }
    }
}
