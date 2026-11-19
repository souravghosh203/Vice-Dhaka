#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DhakaWeatherController.generated.h"

class UDirectionalLightComponent;
class UExponentialHeightFogComponent;
class USceneComponent;
class USkyLightComponent;

UCLASS()
class VICEDHAKA_API ADhakaWeatherController : public AActor
{
    GENERATED_BODY()

public:
    ADhakaWeatherController();
    virtual void Tick(float DeltaSeconds) override;

    UFUNCTION(BlueprintCallable, Category="Vice Dhaka|Weather")
    void ToggleRain();

    UFUNCTION(BlueprintCallable, Category="Vice Dhaka|Weather")
    void SetRainy(bool bInRainy);

    UFUNCTION(BlueprintPure, Category="Vice Dhaka|Weather")
    bool IsRainy() const { return bRainy; }

private:
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<USceneComponent> SceneRoot;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UDirectionalLightComponent> SunLight;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<USkyLightComponent> SkyLight;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UExponentialHeightFogComponent> Fog;

    bool bRainy = false;
    float CurrentFogDensity = 0.0007f;
    float TargetFogDensity = 0.0007f;
    float CurrentSunIntensity = 7.0f;
    float TargetSunIntensity = 7.0f;
    FLinearColor CurrentFogColor = FLinearColor(0.42f, 0.32f, 0.55f, 1.0f);
    FLinearColor TargetFogColor = FLinearColor(0.42f, 0.32f, 0.55f, 1.0f);
};
