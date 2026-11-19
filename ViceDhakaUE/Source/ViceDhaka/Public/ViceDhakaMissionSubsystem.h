#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "TimerManager.h"
#include "ViceDhakaMissionSubsystem.generated.h"

UENUM(BlueprintType)
enum class EViceDhakaMissionType : uint8
{
    PassengerPickup,
    BiryaniDelivery,
    ParcelDelivery,
    PhotoTour,
    CheckpointSprint
};

UENUM(BlueprintType)
enum class EViceDhakaMissionPhase : uint8
{
    Inactive,
    Pickup,
    Destination,
    Complete,
    Failed
};

USTRUCT(BlueprintType)
struct FViceDhakaMission
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EViceDhakaMissionType Type = EViceDhakaMissionType::PassengerPickup;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText Title;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector Pickup = FVector::ZeroVector;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector Destination = FVector(12000.0f, 12000.0f, 0.0f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 RewardTaka = 150;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float TimeLimitSeconds = 180.0f;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FViceDhakaMissionChanged, const FViceDhakaMission&, Mission, EViceDhakaMissionPhase, Phase);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FViceDhakaWalletChanged, int32, NewBalance);

UCLASS()
class VICEDHAKA_API UViceDhakaMissionSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category="Vice Dhaka|Missions")
    bool StartMission(const FViceDhakaMission& Mission);

    UFUNCTION(BlueprintCallable, Category="Vice Dhaka|Missions")
    void ReachObjective();

    UFUNCTION(BlueprintCallable, Category="Vice Dhaka|Missions")
    void FailMission();

    UFUNCTION(BlueprintPure, Category="Vice Dhaka|Missions")
    FViceDhakaMission GetCurrentMission() const { return CurrentMission; }

    UFUNCTION(BlueprintPure, Category="Vice Dhaka|Missions")
    EViceDhakaMissionPhase GetCurrentPhase() const { return CurrentPhase; }

    UFUNCTION(BlueprintPure, Category="Vice Dhaka|Missions")
    float GetTimeRemaining() const;

    UFUNCTION(BlueprintPure, Category="Vice Dhaka|Missions")
    int32 GetTakaBalance() const { return TakaBalance; }

    UPROPERTY(BlueprintAssignable, Category="Vice Dhaka|Missions")
    FViceDhakaMissionChanged OnMissionChanged;

    UPROPERTY(BlueprintAssignable, Category="Vice Dhaka|Missions")
    FViceDhakaWalletChanged OnWalletChanged;

private:
    void OnMissionDeadline();
    void BroadcastMissionState();

    UPROPERTY()
    FViceDhakaMission CurrentMission;

    UPROPERTY()
    EViceDhakaMissionPhase CurrentPhase = EViceDhakaMissionPhase::Inactive;

    FTimerHandle DeadlineTimer;
    float DeadlineAtSeconds = 0.0f;
    int32 TakaBalance = 0;
};
