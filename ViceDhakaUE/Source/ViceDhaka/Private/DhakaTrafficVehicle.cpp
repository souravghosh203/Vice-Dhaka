#include "DhakaTrafficVehicle.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

ADhakaTrafficVehicle::ADhakaTrafficVehicle()
{
    PrimaryActorTick.bCanEverTick = true;
    CollisionBody = CreateDefaultSubobject<UBoxComponent>(TEXT("TrafficCollision"));
    SetRootComponent(CollisionBody);
    CollisionBody->SetBoxExtent(FVector(220.0f, 105.0f, 78.0f));
    CollisionBody->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    CollisionBody->SetCollisionObjectType(ECC_PhysicsBody);
    CollisionBody->SetCollisionResponseToAllChannels(ECR_Block);
    CollisionBody->SetSimulatePhysics(true);
    CollisionBody->SetMassOverrideInKg(NAME_None, 1050.0f, true);
    CollisionBody->SetLinearDamping(0.18f);
    CollisionBody->SetAngularDamping(2.5f);
    CollisionBody->SetEnableGravity(true);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> Cube(TEXT("/Engine/BasicShapes/Cube.Cube"));
    VehicleVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TrafficBodyVisual"));
    VehicleVisual->SetupAttachment(CollisionBody);
    VehicleVisual->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    if (Cube.Succeeded())
    {
        VehicleVisual->SetStaticMesh(Cube.Object);
        VehicleVisual->SetRelativeScale3D(FVector(2.1f, 0.96f, 0.62f));
        VehicleVisual->SetRelativeLocation(FVector(0.0f, 0.0f, 22.0f));
    }
}

void ADhakaTrafficVehicle::ConfigureRoute(bool bInAlongX, float LaneCoordinate, float Direction, float StartCoordinate)
{
    bAlongX = bInAlongX;
    Lane = LaneCoordinate;
    TravelDirection = Direction < 0.0f ? -1.0f : 1.0f;
    FVector Position = GetActorLocation();
    if (bAlongX)
    {
        Position.X = StartCoordinate;
        Position.Y = Lane;
        SetActorRotation(FRotator(0.0f, TravelDirection > 0.0f ? 0.0f : 180.0f, 0.0f));
    }
    else
    {
        Position.X = Lane;
        Position.Y = StartCoordinate;
        SetActorRotation(FRotator(0.0f, TravelDirection > 0.0f ? 90.0f : -90.0f, 0.0f));
    }
    SetActorLocation(Position, false, nullptr, ETeleportType::TeleportPhysics);
}

void ADhakaTrafficVehicle::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    if (!CollisionBody->IsSimulatingPhysics()) return;

    FVector Forward = bAlongX ? FVector(TravelDirection, 0.0f, 0.0f) : FVector(0.0f, TravelDirection, 0.0f);
    const FVector Velocity = CollisionBody->GetPhysicsLinearVelocity();
    const float ForwardSpeed = FVector::DotProduct(Velocity, Forward);
    const FVector LateralVelocity = Velocity - Forward * ForwardSpeed;
    CollisionBody->AddForce(Forward * DriveForce - LateralVelocity * 550.0f);

    const FVector CurrentForward = GetActorForwardVector();
    const float HeadingError = FMath::Atan2(FVector::CrossProduct(CurrentForward, Forward).Z,
        FVector::DotProduct(CurrentForward, Forward));
    const float YawRate = CollisionBody->GetPhysicsAngularVelocityInRadians().Z;
    CollisionBody->AddTorqueInRadians(FVector(0.0f, 0.0f, FMath::Clamp(HeadingError * 150000.0f - YawRate * 42000.0f, -320000.0f, 320000.0f)));

    if (ForwardSpeed > MaxSpeed)
    {
        CollisionBody->SetPhysicsLinearVelocity(Velocity + Forward * (MaxSpeed - ForwardSpeed));
    }

    FVector Position = CollisionBody->GetComponentLocation();
    float& Coordinate = bAlongX ? Position.X : Position.Y;
    if (Coordinate > WorldHalfExtent + 500.0f)
    {
        Coordinate = -WorldHalfExtent;
        Position.Z = 110.0f;
        SetActorLocation(Position, false, nullptr, ETeleportType::TeleportPhysics);
    }
    else if (Coordinate < -WorldHalfExtent - 500.0f)
    {
        Coordinate = WorldHalfExtent;
        Position.Z = 110.0f;
        SetActorLocation(Position, false, nullptr, ETeleportType::TeleportPhysics);
    }
}
