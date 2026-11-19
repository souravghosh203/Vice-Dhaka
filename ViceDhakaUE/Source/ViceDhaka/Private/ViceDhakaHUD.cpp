#include "ViceDhakaHUD.h"
#include "DhakaWeatherController.h"
#include "Engine/Canvas.h"
#include "Engine/Engine.h"
#include "Engine/GameInstance.h"
#include "EngineUtils.h"
#include "RickshawPawn.h"
#include "ViceDhakaMissionSubsystem.h"

void AViceDhakaHUD::DrawHUD()
{
    Super::DrawHUD();
    if (!Canvas || !PlayerOwner || !GEngine) return;

    const float Width = Canvas->SizeX;
    const float Height = Canvas->SizeY;
    const FLinearColor Panel(0.035f, 0.025f, 0.075f, 0.86f);
    const FLinearColor Cyan(0.15f, 0.88f, 1.0f, 1.0f);
    const FLinearColor Pink(1.0f, 0.19f, 0.62f, 1.0f);
    const FLinearColor Gold(1.0f, 0.77f, 0.20f, 1.0f);

    Canvas->DrawRect(Panel, 24.0f, 24.0f, 300.0f, 108.0f);
    Canvas->DrawText(GEngine->GetMediumFont(), TEXT("VICE DHAKA"), 42.0f, 34.0f, 1.0f, 1.0f);
    Canvas->DrawText(GEngine->GetSmallFont(), TEXT("DHAKA DIVISION  /  RICKSHAW RUSH"), 42.0f, 67.0f, 1.0f, 1.0f);

    if (const ARickshawPawn* Rickshaw = Cast<ARickshawPawn>(PlayerOwner->GetPawn()))
    {
        const float SpeedKmh = FMath::Abs(FVector::DotProduct(Rickshaw->GetVelocity(), Rickshaw->GetActorForwardVector())) * 0.036f;
        const FString SpeedText = FString::Printf(TEXT("%.0f KM/H"), SpeedKmh);
        Canvas->DrawRect(Panel, 24.0f, Height - 105.0f, 240.0f, 76.0f);
        Canvas->DrawText(GEngine->GetMediumFont(), SpeedText, 42.0f, Height - 91.0f, 1.1f, 1.1f);
        Canvas->DrawText(GEngine->GetSmallFont(), TEXT("WASD DRIVE   C CAMERA   R WEATHER"), 42.0f, Height - 52.0f, 1.0f, 1.0f);
    }

    if (const UGameInstance* GameInstance = PlayerOwner->GetGameInstance())
    {
        if (const UViceDhakaMissionSubsystem* Missions = GameInstance->GetSubsystem<UViceDhakaMissionSubsystem>())
        {
            const FViceDhakaMission Mission = Missions->GetCurrentMission();
            const FString MissionName = Mission.Title.IsEmpty() ? TEXT("EXPLORE THE CITY") : Mission.Title.ToString();
            const TCHAR* PhaseText = TEXT("READY");
            switch (Missions->GetCurrentPhase())
            {
            case EViceDhakaMissionPhase::Pickup: PhaseText = TEXT("PICKUP"); break;
            case EViceDhakaMissionPhase::Destination: PhaseText = TEXT("DELIVER"); break;
            case EViceDhakaMissionPhase::Complete: PhaseText = TEXT("COMPLETE"); break;
            case EViceDhakaMissionPhase::Failed: PhaseText = TEXT("EXPIRED"); break;
            default: break;
            }

            const float CardWidth = 420.0f;
            const float CardX = (Width - CardWidth) * 0.5f;
            Canvas->DrawRect(Panel, CardX, 24.0f, CardWidth, 108.0f);
            Canvas->DrawText(GEngine->GetMediumFont(), MissionName, CardX + 20.0f, 34.0f, 1.0f, 1.0f);
            Canvas->DrawText(GEngine->GetSmallFont(), FString::Printf(TEXT("%s  /  %s"), PhaseText, *FString::Printf(TEXT("%03.0f SEC"), Missions->GetTimeRemaining())), CardX + 20.0f, 69.0f, 1.0f, 1.0f);
            Canvas->DrawText(GEngine->GetSmallFont(), FString::Printf(TEXT("REWARD  ৳%d"), Mission.RewardTaka), CardX + 20.0f, 94.0f, 1.0f, 1.0f);

            const FString MoneyText = FString::Printf(TEXT("৳ %d"), Missions->GetTakaBalance());
            Canvas->DrawRect(Panel, Width - 190.0f, 24.0f, 166.0f, 62.0f);
            Canvas->DrawText(GEngine->GetMediumFont(), MoneyText, Width - 172.0f, 39.0f, 1.0f, 1.0f);
        }
    }

    FString WeatherText = TEXT("CLEAR");
    for (TActorIterator<ADhakaWeatherController> It(GetWorld()); It; ++It)
    {
        WeatherText = It->IsRainy() ? TEXT("MONSOON") : TEXT("CLEAR");
        break;
    }
    Canvas->DrawText(GEngine->GetSmallFont(), WeatherText, Width - 180.0f, Height - 45.0f, 1.0f, 1.0f);
    Canvas->DrawRect(Cyan, 24.0f, 22.0f, 300.0f, 2.0f);
    Canvas->DrawRect(Pink, (Width - 420.0f) * 0.5f, 22.0f, 420.0f, 2.0f);
    Canvas->DrawRect(Gold, Width - 190.0f, 22.0f, 166.0f, 2.0f);
}
