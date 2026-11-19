#include "ViceDhakaMissionSubsystem.h"
#include "Engine/World.h"
#include "TimerManager.h"

bool UViceDhakaMissionSubsystem::StartMission(const FViceDhakaMission& Mission)
{
    if (CurrentPhase == EViceDhakaMissionPhase::Pickup || CurrentPhase == EViceDhakaMissionPhase::Destination)
    {
        return false;
    }

    CurrentMission = Mission;
    CurrentPhase = Mission.Type == EViceDhakaMissionType::CheckpointSprint
        ? EViceDhakaMissionPhase::Destination
        : EViceDhakaMissionPhase::Pickup;

    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().ClearTimer(DeadlineTimer);
        if (Mission.TimeLimitSeconds > 0.0f)
        {
            DeadlineAtSeconds = World->GetTimeSeconds() + Mission.TimeLimitSeconds;
            World->GetTimerManager().SetTimer(DeadlineTimer, this, &UViceDhakaMissionSubsystem::OnMissionDeadline, Mission.TimeLimitSeconds, false);
        }
    }

    BroadcastMissionState();
    return true;
}

void UViceDhakaMissionSubsystem::ReachObjective()
{
    if (CurrentPhase == EViceDhakaMissionPhase::Pickup)
    {
        CurrentPhase = EViceDhakaMissionPhase::Destination;
        BroadcastMissionState();
    }
    else if (CurrentPhase == EViceDhakaMissionPhase::Destination)
    {
        CurrentPhase = EViceDhakaMissionPhase::Complete;
        TakaBalance += FMath::Max(0, CurrentMission.RewardTaka);
        OnWalletChanged.Broadcast(TakaBalance);
        if (UWorld* World = GetWorld())
        {
            World->GetTimerManager().ClearTimer(DeadlineTimer);
        }
        BroadcastMissionState();
    }
}

void UViceDhakaMissionSubsystem::FailMission()
{
    if (CurrentPhase != EViceDhakaMissionPhase::Pickup && CurrentPhase != EViceDhakaMissionPhase::Destination)
    {
        return;
    }
    CurrentPhase = EViceDhakaMissionPhase::Failed;
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().ClearTimer(DeadlineTimer);
    }
    BroadcastMissionState();
}

float UViceDhakaMissionSubsystem::GetTimeRemaining() const
{
    if (const UWorld* World = GetWorld())
    {
        return CurrentPhase == EViceDhakaMissionPhase::Pickup || CurrentPhase == EViceDhakaMissionPhase::Destination
            ? FMath::Max(0.0f, DeadlineAtSeconds - World->GetTimeSeconds())
            : 0.0f;
    }
    return 0.0f;
}

void UViceDhakaMissionSubsystem::OnMissionDeadline()
{
    FailMission();
}

void UViceDhakaMissionSubsystem::BroadcastMissionState()
{
    OnMissionChanged.Broadcast(CurrentMission, CurrentPhase);
}
