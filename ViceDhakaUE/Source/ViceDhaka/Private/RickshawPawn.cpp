#include "RickshawPawn.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/InputComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/CollisionProfile.h"
#include "EngineUtils.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputCoreTypes.h"
#include "DhakaWeatherController.h"
#include "ViceDhakaMissionSubsystem.h"
#include "Engine/GameInstance.h"
#include "UObject/ConstructorHelpers.h"

ARickshawPawn::ARickshawPawn()
{
    PrimaryActorTick.bCanEverTick = true;
    AutoPossessPlayer = EAutoReceiveInput::Player0;

    CollisionBody = CreateDefaultSubobject<UBoxComponent>(TEXT("ChassisCollision"));
    SetRootComponent(CollisionBody);
    CollisionBody->SetBoxExtent(FVector(165.0f, 88.0f, 76.0f));
    CollisionBody->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    CollisionBody->SetCollisionObjectType(ECC_Pawn);
    CollisionBody->SetCollisionResponseToAllChannels(ECR_Block);
    CollisionBody->SetSimulatePhysics(true);
    CollisionBody->SetMassOverrideInKg(NAME_None, 340.0f, true);
    CollisionBody->SetLinearDamping(0.22f);
    CollisionBody->SetAngularDamping(2.8f);
    CollisionBody->SetEnableGravity(true);
    CollisionBody->SetNotifyRigidBodyCollision(true);
    CollisionBody->OnComponentHit.AddDynamic(this, &ARickshawPawn::OnBodyHit);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> Cube(TEXT("/Engine/BasicShapes/Cube.Cube"));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> Sphere(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> Cylinder(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));

    RickshawVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RickshawBodyVisual"));
    RickshawVisual->SetupAttachment(CollisionBody);
    RickshawVisual->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    if (Cube.Succeeded())
    {
        RickshawVisual->SetStaticMesh(Cube.Object);
        RickshawVisual->SetRelativeScale3D(FVector(1.58f, 0.88f, 0.62f));
        RickshawVisual->SetRelativeLocation(FVector(-10.0f, 0.0f, 28.0f));
    }

    DriverVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RafiDriverVisual"));
    DriverVisual->SetupAttachment(CollisionBody);
    DriverVisual->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    if (Cube.Succeeded())
    {
        DriverVisual->SetStaticMesh(Cube.Object);
        DriverVisual->SetRelativeScale3D(FVector(0.42f, 0.34f, 0.64f));
        DriverVisual->SetRelativeLocation(FVector(88.0f, 0.0f, 115.0f));
    }

    UStaticMeshComponent* Head = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DriverHeadVisual"));
    Head->SetupAttachment(CollisionBody);
    Head->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    if (Sphere.Succeeded())
    {
        Head->SetStaticMesh(Sphere.Object);
        Head->SetRelativeScale3D(FVector(0.3f, 0.3f, 0.34f));
        Head->SetRelativeLocation(FVector(103.0f, 0.0f, 173.0f));
    }

    const FVector WheelLocations[] = {
        FVector(112.0f, 0.0f, -70.0f), FVector(-100.0f, -76.0f, -70.0f), FVector(-100.0f, 76.0f, -70.0f)
    };
    for (int32 Index = 0; Index < UE_ARRAY_COUNT(WheelLocations); ++Index)
    {
        UStaticMeshComponent* Wheel = CreateDefaultSubobject<UStaticMeshComponent>(*FString::Printf(TEXT("Wheel_%d"), Index));
        Wheel->SetupAttachment(CollisionBody);
        Wheel->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        if (Cylinder.Succeeded())
        {
            Wheel->SetStaticMesh(Cylinder.Object);
            Wheel->SetRelativeScale3D(FVector(0.62f, 0.2f, 0.62f));
            Wheel->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
            Wheel->SetRelativeLocation(WheelLocations[Index]);
        }
        Wheels.Add(Wheel);
    }

    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("ThirdPersonCameraBoom"));
    CameraBoom->SetupAttachment(CollisionBody);
    CameraBoom->TargetArmLength = 780.0f;
    CameraBoom->SocketOffset = FVector(0.0f, 115.0f, 145.0f);
    CameraBoom->SetRelativeRotation(FRotator(-11.0f, 0.0f, 0.0f));
    CameraBoom->bEnableCameraLag = true;
    CameraBoom->CameraLagSpeed = 7.0f;
    CameraBoom->bUsePawnControlRotation = false;

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->FieldOfView = 78.0f;
    FollowCamera->bUsePawnControlRotation = false;
}

void ARickshawPawn::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    if (!CollisionBody->IsSimulatingPhysics()) return;

    const FVector Forward = GetActorForwardVector();
    const FVector Velocity = CollisionBody->GetPhysicsLinearVelocity();
    const float ForwardSpeed = FVector::DotProduct(Velocity, Forward);
    const float SpeedFactor = FMath::Clamp(FMath::Abs(ForwardSpeed) / 450.0f, 0.12f, 1.0f);

    CollisionBody->AddForce(Forward * (DriveInput * 95000.0f));
    if (BrakeInput > 0.0f)
    {
        CollisionBody->AddForce(-Velocity * (BrakeInput * 950.0f));
    }

    // Left input is intentionally reversed: pressing left turns the rickshaw right.
    CollisionBody->AddTorqueInRadians(FVector(0.0f, 0.0f, -SteeringInput * 115000.0f * SpeedFactor));

    constexpr float MaxForwardSpeed = 1450.0f;
    constexpr float MaxReverseSpeed = 500.0f;
    if (ForwardSpeed > MaxForwardSpeed)
    {
        CollisionBody->SetPhysicsLinearVelocity(Velocity + Forward * (MaxForwardSpeed - ForwardSpeed));
    }
    else if (ForwardSpeed < -MaxReverseSpeed)
    {
        CollisionBody->SetPhysicsLinearVelocity(Velocity + Forward * (-MaxReverseSpeed - ForwardSpeed));
    }

    WheelSpin += ForwardSpeed * DeltaSeconds * 0.22f;
    for (UStaticMeshComponent* Wheel : Wheels)
    {
        Wheel->SetRelativeRotation(FRotator(90.0f, 0.0f, WheelSpin));
    }

    if (UGameInstance* GameInstance = GetGameInstance())
    {
        if (UViceDhakaMissionSubsystem* Missions = GameInstance->GetSubsystem<UViceDhakaMissionSubsystem>())
        {
            const EViceDhakaMissionPhase Phase = Missions->GetCurrentPhase();
            if (Phase == EViceDhakaMissionPhase::Pickup || Phase == EViceDhakaMissionPhase::Destination)
            {
                const FViceDhakaMission Mission = Missions->GetCurrentMission();
                const FVector Objective = Phase == EViceDhakaMissionPhase::Pickup ? Mission.Pickup : Mission.Destination;
                if (FVector::Dist2D(CollisionBody->GetComponentLocation(), Objective) < 450.0f)
                {
                    Missions->ReachObjective();
                }
            }
        }
    }
}

void ARickshawPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    PlayerInputComponent->BindAxis(TEXT("Drive"), this, &ARickshawPawn::SetDrive);
    PlayerInputComponent->BindAxis(TEXT("Steer"), this, &ARickshawPawn::SetSteering);
    PlayerInputComponent->BindAxis(TEXT("Brake"), this, &ARickshawPawn::SetBrake);
    PlayerInputComponent->BindKey(EKeys::C, IE_Pressed, this, &ARickshawPawn::ToggleCamera);
    PlayerInputComponent->BindKey(EKeys::R, IE_Pressed, this, &ARickshawPawn::ToggleWeather);
}

void ARickshawPawn::SetDrive(float Value)
{
    DriveInput = FMath::Clamp(Value, -1.0f, 1.0f);
}

void ARickshawPawn::SetSteering(float Value)
{
    SteeringInput = FMath::Clamp(Value, -1.0f, 1.0f);
}

void ARickshawPawn::SetBrake(float Value)
{
    BrakeInput = FMath::Clamp(Value, 0.0f, 1.0f);
}

void ARickshawPawn::ToggleCamera()
{
    bDriverCamera = !bDriverCamera;
    CameraBoom->TargetArmLength = bDriverCamera ? 155.0f : 780.0f;
    CameraBoom->SocketOffset = bDriverCamera ? FVector(30.0f, 0.0f, 95.0f) : FVector(0.0f, 115.0f, 145.0f);
}

void ARickshawPawn::ToggleWeather()
{
    for (TActorIterator<ADhakaWeatherController> It(GetWorld()); It; ++It)
    {
        It->ToggleRain();
        break;
    }
}

void ARickshawPawn::OnBodyHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
    if (!OtherActor || OtherActor == this) return;
    // Chaos resolves blocking contacts; this hook is reserved for damage and mission feedback.
}
