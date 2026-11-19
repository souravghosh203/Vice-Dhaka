#include "DhakaWeatherController.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/ExponentialHeightFogComponent.h"
#include "Components/SceneComponent.h"
#include "Components/SkyLightComponent.h"

ADhakaWeatherController::ADhakaWeatherController()
{
    PrimaryActorTick.bCanEverTick = true;
    SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
    SetRootComponent(SceneRoot);

    SunLight = CreateDefaultSubobject<UDirectionalLightComponent>(TEXT("SunLight"));
    SunLight->SetupAttachment(SceneRoot);
    SunLight->SetRelativeRotation(FRotator(-42.0f, -32.0f, 0.0f));
    SunLight->SetIntensity(CurrentSunIntensity);
    SunLight->SetLightColor(FLinearColor(1.0f, 0.77f, 0.55f));

    SkyLight = CreateDefaultSubobject<USkyLightComponent>(TEXT("SkyLight"));
    SkyLight->SetupAttachment(SceneRoot);
    SkyLight->SetMobility(EComponentMobility::Movable);
    SkyLight->SetIntensity(1.1f);

    Fog = CreateDefaultSubobject<UExponentialHeightFogComponent>(TEXT("AtmosphereFog"));
    Fog->SetupAttachment(SceneRoot);
    Fog->FogDensity = CurrentFogDensity;
    Fog->FogInscatteringColor = CurrentFogColor;
}

void ADhakaWeatherController::ToggleRain()
{
    SetRainy(!bRainy);
}

void ADhakaWeatherController::SetRainy(bool bInRainy)
{
    bRainy = bInRainy;
    TargetFogDensity = bRainy ? 0.0015f : 0.0007f;
    TargetSunIntensity = bRainy ? 2.0f : 7.0f;
    TargetFogColor = bRainy
        ? FLinearColor(0.22f, 0.24f, 0.34f, 1.0f)
        : FLinearColor(0.42f, 0.32f, 0.55f, 1.0f);
}

void ADhakaWeatherController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    const float Alpha = FMath::Clamp(DeltaSeconds * 0.35f, 0.0f, 1.0f);
    CurrentFogDensity = FMath::Lerp(CurrentFogDensity, TargetFogDensity, Alpha);
    CurrentSunIntensity = FMath::FInterpTo(CurrentSunIntensity, TargetSunIntensity, DeltaSeconds, 0.35f);
    CurrentFogColor = FLinearColor::LerpUsingHSV(CurrentFogColor, TargetFogColor, Alpha);
    Fog->FogDensity = CurrentFogDensity;
    Fog->FogInscatteringColor = CurrentFogColor;
    SunLight->SetIntensity(CurrentSunIntensity);
    SkyLight->SetIntensity(bRainy ? 0.35f : 1.1f);
}
