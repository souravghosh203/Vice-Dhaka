#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "RickshawPawn.generated.h"

class UBoxComponent;
class UCameraComponent;
class USpringArmComponent;
class UStaticMeshComponent;

UCLASS()
class VICEDHAKA_API ARickshawPawn : public APawn
{
    GENERATED_BODY()

public:
    ARickshawPawn();
    virtual void Tick(float DeltaSeconds) override;
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

private:
    void SetDrive(float Value);
    void SetSteering(float Value);
    void SetBrake(float Value);
    void ToggleCamera();
    void ToggleWeather();

    UFUNCTION()
    void OnBodyHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UBoxComponent> CollisionBody;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UStaticMeshComponent> RickshawVisual;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UStaticMeshComponent> DriverVisual;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<USpringArmComponent> CameraBoom;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UCameraComponent> FollowCamera;

    UPROPERTY(VisibleAnywhere)
    TArray<TObjectPtr<UStaticMeshComponent>> Wheels;

    float DriveInput = 0.0f;
    float SteeringInput = 0.0f;
    float BrakeInput = 0.0f;
    float WheelSpin = 0.0f;
    bool bDriverCamera = false;
};
