#include "DhakaCityBlockout.h"
#include "DhakaTrafficVehicle.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"

namespace
{
    constexpr int32 CityBlocks = 6;
    constexpr float BlockSize = 18000.0f;
    constexpr float RoadWidth = 2600.0f;
    constexpr float CitySize = CityBlocks * BlockSize;
}

ADhakaCityBlockout::ADhakaCityBlockout()
{
    PrimaryActorTick.bCanEverTick = false;
    SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
    SetRootComponent(SceneRoot);

    Ground = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Ground"));
    Roads = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Roads"));
    Buildings = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Buildings"));
    FacadeDetails = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("FacadeDetails"));
    WaterTanks = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("RooftopWaterTanks"));

    for (UInstancedStaticMeshComponent* Component : {Ground.Get(), Roads.Get(), Buildings.Get(), FacadeDetails.Get(), WaterTanks.Get()})
    {
        Component->SetupAttachment(SceneRoot);
        Component->SetMobility(EComponentMobility::Static);
    }

    static ConstructorHelpers::FObjectFinder<UStaticMesh> Cube(TEXT("/Engine/BasicShapes/Cube.Cube"));
    if (Cube.Succeeded())
    {
        Ground->SetStaticMesh(Cube.Object);
        Roads->SetStaticMesh(Cube.Object);
        Buildings->SetStaticMesh(Cube.Object);
        FacadeDetails->SetStaticMesh(Cube.Object);
    }

    static ConstructorHelpers::FObjectFinder<UStaticMesh> Cylinder(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
    if (Cylinder.Succeeded())
    {
        WaterTanks->SetStaticMesh(Cylinder.Object);
    }

    Ground->SetCollisionProfileName(TEXT("BlockAll"));
    Buildings->SetCollisionProfileName(TEXT("BlockAll"));
    Roads->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    FacadeDetails->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    WaterTanks->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ADhakaCityBlockout::BeginPlay()
{
    Super::BeginPlay();
    BuildCity();
}

void ADhakaCityBlockout::BuildCity()
{
    const float HalfCity = CitySize * 0.5f;
    Ground->AddInstance(FTransform(FRotator::ZeroRotator, FVector(0.0f, 0.0f, -60.0f), FVector(CitySize / 100.0f, CitySize / 100.0f, 1.0f)));

    for (int32 RoadIndex = 0; RoadIndex <= CityBlocks; ++RoadIndex)
    {
        const float Position = -HalfCity + RoadIndex * BlockSize;
        Roads->AddInstance(FTransform(FRotator::ZeroRotator, FVector(Position, 0.0f, -4.0f), FVector(RoadWidth / 100.0f, CitySize / 100.0f, 0.08f)));
        Roads->AddInstance(FTransform(FRotator::ZeroRotator, FVector(0.0f, Position, -4.0f), FVector(CitySize / 100.0f, RoadWidth / 100.0f, 0.08f)));
    }

    FRandomStream Random(4803);
    const float CellSize = BlockSize / 3.0f;
    for (int32 BlockX = 0; BlockX < CityBlocks; ++BlockX)
    {
        for (int32 BlockY = 0; BlockY < CityBlocks; ++BlockY)
        {
            const float CenterX = -HalfCity + (BlockX + 0.5f) * BlockSize;
            const float CenterY = -HalfCity + (BlockY + 0.5f) * BlockSize;
            for (int32 CellX = 0; CellX < 3; ++CellX)
            {
                for (int32 CellY = 0; CellY < 3; ++CellY)
                {
                    const float X = CenterX + (CellX - 1) * CellSize;
                    const float Y = CenterY + (CellY - 1) * CellSize;
                    const float Width = CellSize - Random.FRandRange(900.0f, 1700.0f);
                    const float Depth = CellSize - Random.FRandRange(900.0f, 1700.0f);
                    float Height = Random.FRandRange(1800.0f, 7200.0f);
                    if (BlockX >= 3 && BlockX <= 4)
                    {
                        Height = Random.FRandRange(5200.0f, 10500.0f);
                    }

                    Buildings->AddInstance(FTransform(FRotator::ZeroRotator, FVector(X, Y, Height * 0.5f), FVector(Width / 100.0f, Depth / 100.0f, Height / 100.0f)));

                    const bool bFacesRoadX = CellX == 0 || CellX == 2;
                    const bool bFacesRoadY = CellY == 0 || CellY == 2;
                    const int32 FloorCount = FMath::Min(12, FMath::FloorToInt(Height / 700.0f));
                    for (int32 Floor = 1; Floor < FloorCount; Floor += 2)
                    {
                        const float Z = Floor * 600.0f + 360.0f;
                        if (bFacesRoadY)
                        {
                            const float Side = CellY == 0 ? -1.0f : 1.0f;
                            const float EdgeY = Y + Side * (Depth * 0.5f + 55.0f);
                            FacadeDetails->AddInstance(FTransform(FRotator::ZeroRotator, FVector(X, EdgeY, Z), FVector(Width * 0.0072f, 1.25f, 0.28f)));
                            FacadeDetails->AddInstance(FTransform(FRotator::ZeroRotator, FVector(X, EdgeY + Side * 62.0f, Z + 65.0f), FVector(Width * 0.0072f, 0.12f, 0.85f)));
                        }
                        if (bFacesRoadX)
                        {
                            const float Side = CellX == 0 ? -1.0f : 1.0f;
                            const float EdgeX = X + Side * (Width * 0.5f + 55.0f);
                            FacadeDetails->AddInstance(FTransform(FRotator::ZeroRotator, FVector(EdgeX, Y, Z), FVector(1.25f, Depth * 0.0072f, 0.28f)));
                            FacadeDetails->AddInstance(FTransform(FRotator::ZeroRotator, FVector(EdgeX + Side * 62.0f, Y, Z + 65.0f), FVector(0.12f, Depth * 0.0072f, 0.85f)));
                        }
                    }

                    if (bFacesRoadY)
                    {
                        const float Side = CellY == 0 ? -1.0f : 1.0f;
                        FacadeDetails->AddInstance(FTransform(FRotator::ZeroRotator, FVector(X, Y + Side * (Depth * 0.5f + 90.0f), 520.0f), FVector(Width * 0.008f, 1.8f, 0.55f)));
                    }
                    if (bFacesRoadX)
                    {
                        const float Side = CellX == 0 ? -1.0f : 1.0f;
                        FacadeDetails->AddInstance(FTransform(FRotator::ZeroRotator, FVector(X + Side * (Width * 0.5f + 90.0f), Y, 520.0f), FVector(1.8f, Depth * 0.008f, 0.55f)));
                    }

                    if (Random.RandRange(0, 2) == 0)
                    {
                        WaterTanks->AddInstance(FTransform(FRotator::ZeroRotator, FVector(X + Width * 0.2f, Y - Depth * 0.15f, Height + 170.0f), FVector(2.1f, 2.1f, 1.7f)));
                    }
                }
            }
        }
    }

    // Two lanes on each street direction. Traffic uses physics bodies, so impacts
    // separate vehicles through Chaos instead of teleporting them sideways.
    UWorld* World = GetWorld();
    if (!World) return;

    for (int32 RoadIndex = 0; RoadIndex <= CityBlocks; ++RoadIndex)
    {
        const float RoadCoordinate = -HalfCity + RoadIndex * BlockSize;
        for (const float Direction : {-1.0f, 1.0f})
        {
            const float StartX = Random.FRandRange(-HalfCity + 4000.0f, HalfCity - 4000.0f);
            const float LaneY = RoadCoordinate + (Direction > 0.0f ? -460.0f : 460.0f);
            const FRotator XRotation(0.0f, Direction > 0.0f ? 0.0f : 180.0f, 0.0f);
            if (ADhakaTrafficVehicle* Vehicle = World->SpawnActor<ADhakaTrafficVehicle>(ADhakaTrafficVehicle::StaticClass(), FVector(StartX, LaneY, 100.0f), XRotation))
            {
                Vehicle->ConfigureRoute(true, LaneY, Direction, StartX);
            }

            const float StartY = Random.FRandRange(-HalfCity + 4000.0f, HalfCity - 4000.0f);
            const float LaneX = RoadCoordinate + (Direction > 0.0f ? 460.0f : -460.0f);
            const FRotator YRotation(0.0f, Direction > 0.0f ? 90.0f : -90.0f, 0.0f);
            if (ADhakaTrafficVehicle* Vehicle = World->SpawnActor<ADhakaTrafficVehicle>(ADhakaTrafficVehicle::StaticClass(), FVector(LaneX, StartY, 100.0f), YRotation))
            {
                Vehicle->ConfigureRoute(false, LaneX, Direction, StartY);
            }
        }
    }
}
