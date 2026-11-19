#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DhakaCityBlockout.generated.h"

class UInstancedStaticMeshComponent;
class USceneComponent;

UCLASS()
class VICEDHAKA_API ADhakaCityBlockout : public AActor
{
    GENERATED_BODY()

public:
    ADhakaCityBlockout();
    virtual void BeginPlay() override;

private:
    void BuildCity();

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<USceneComponent> SceneRoot;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UInstancedStaticMeshComponent> Ground;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UInstancedStaticMeshComponent> Roads;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UInstancedStaticMeshComponent> Buildings;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UInstancedStaticMeshComponent> FacadeDetails;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UInstancedStaticMeshComponent> WaterTanks;
};
