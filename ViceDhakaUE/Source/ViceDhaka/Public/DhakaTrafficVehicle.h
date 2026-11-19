#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "DhakaTrafficVehicle.generated.h"

class UBoxComponent;
class UStaticMeshComponent;

UCLASS()
class VICEDHAKA_API ADhakaTrafficVehicle : public APawn
{
    GENERATED_BODY()

public:
    ADhakaTrafficVehicle();
    virtual void Tick(float DeltaSeconds) override;

    void ConfigureRoute(bool bInAlongX, float LaneCoordinate, float Direction, float StartCoordinate);

private:
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UBoxComponent> CollisionBody;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UStaticMeshComponent> VehicleVisual;

    bool bAlongX = true;
    float TravelDirection = 1.0f;
    float Lane = 0.0f;
    float WorldHalfExtent = 54000.0f;
    float MaxSpeed = 1200.0f;
    float DriveForce = 600000.0f;
};
