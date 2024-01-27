// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterPawn.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/SphereComponent.h"

// Sets default values
ACharacterPawn::ACharacterPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create the sphere component and attach it to the root
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SetRootComponent(SphereComponent);
	SphereComponent->InitSphereRadius(32.0f);

	mainBody = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("body"));
	mainBody->SetupAttachment(SphereComponent);
	mainBody->SetSimulatePhysics(true);
	mainBody->SetRelativeLocation(FVector(0.0f, 0.0f, -50.0f));

	mouthHolder = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("mouth"));
	mouthHolder->SetupAttachment(mainBody);

	// construct sphere component
	tongue = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("tongue"));
	tongue->SetupAttachment(mainBody);

	PhysicsConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("PhysicsConstraint"));
	PhysicsConstraint->SetupAttachment(mouthHolder);
	PhysicsConstraint->SetConstrainedComponents(mouthHolder, "", tongue, "");
	PhysicsConstraint->SetLinearXLimit(LCM_Limited, 100.0f);
	PhysicsConstraint->SetLinearYLimit(LCM_Limited, 100.0f);
	PhysicsConstraint->SetLinearZLimit(LCM_Limited, 100.0f);

	// construct rope
	//cable = CreateDefaultSubobject<UCableComponent>(TEXT("Cable"));
	//cable->SetupAttachment(mouthHolder);
	//cable->SetAttachEndToComponent(tongue);
	//cable->EndLocation = tongue->GetComponentLocation();
	//cable->EndLocation = FVector().Zero();

	// make limit scalable
	// on input -> set limit, and shoot tongue
	// physics constraint

	//// Add projectile movement component
	//ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	//ProjectileMovementComponent->SetUpdatedComponent(tong);
	//ProjectileMovementComponent->InitialSpeed = 0;
	//ProjectileMovementComponent->MaxSpeed = TongEjectSpeed;
	//ProjectileMovementComponent->bRotationFollowsVelocity = true;
	//ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
}

// Called when the game starts or when spawned
void ACharacterPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACharacterPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Update de positie van de Pawn
	if (!CurrentVelocity.IsZero())
	{
		// Normalize input
		FVector2D InputVector(CurrentVelocity.X, CurrentVelocity.Y);
		InputVector = InputVector.GetSafeNormal();
		FVector MovementDirection = FVector(InputVector.X, InputVector.Y, 0);

		// Apply movement
		FVector NewLocation = GetActorLocation() + FVector(
			InputVector.X * MovementSpeed * DeltaTime,
			InputVector.Y * MovementSpeed * DeltaTime,
			0
		);
		SetActorLocation(NewLocation);

		// Rotate with velocity
		FRotator NewRotation = MovementDirection.Rotation();
		FRotator CurrentRotation = GetActorRotation();
		FRotator SmoothRotation = FMath::RInterpTo(CurrentRotation, NewRotation, DeltaTime, RotateSpeed);
		SetActorRotation(SmoothRotation);
	}
}

// Called to bind functionality to input
void ACharacterPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Bind movement actions
	PlayerInputComponent->BindAxis("MoveHorizontal", this, &ACharacterPawn::MoveHorizontal);
	PlayerInputComponent->BindAxis("MoveVertical", this, &ACharacterPawn::MoveVertical);
}

void ACharacterPawn::MoveHorizontal(float Value)
{
	CurrentVelocity.X = Value;
}

void ACharacterPawn::MoveVertical(float Value)
{
	CurrentVelocity.Y = Value;
}

