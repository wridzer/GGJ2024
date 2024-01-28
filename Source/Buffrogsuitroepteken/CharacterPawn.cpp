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
	//mainBody->SetSimulatePhysics(true);
	mainBody->SetRelativeLocation(FVector(0.0f, 0.0f, -50.0f));

	mouthHolder = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("mouth"));
	mouthHolder->SetupAttachment(mainBody);

	// construct sphere component
	tongue = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("tongue"));
	tongue->SetupAttachment(SphereComponent);
	tongue->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	tongue->SetNotifyRigidBodyCollision(true);
	tongue->MoveIgnoreComponents.Add(SphereComponent);
	tongue->OnComponentHit.AddDynamic(this, &ACharacterPawn::OnHit);

	PhysicsConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("PhysicsConstraint"));
	PhysicsConstraint->SetupAttachment(mouthHolder);
	PhysicsConstraint->SetConstrainedComponents(mouthHolder, "", tongue, "");
	PhysicsConstraint->SetLinearXLimit(LCM_Limited, 100.0f);
	PhysicsConstraint->SetLinearYLimit(LCM_Limited, 100.0f);
	PhysicsConstraint->SetLinearZLimit(LCM_Limited, 100.0f);
	PhysicsConstraint->SetLinearPositionDrive(true, true, true);
	PhysicsConstraint->SetLinearVelocityDrive(true, true, true);
	PhysicsConstraint->SetLinearDriveParams(1.0f, 20.0f, 0.f);
}

// Called when the game starts or when spawned
void ACharacterPawn::BeginPlay()
{
	Super::BeginPlay();
	tongueLimit = TongMinLength;
	//tongue->SetRelativeLocation(mouthHolder->GetRelativeLocation()); // + FVector(0, TongMinLength, 0));
	//tongue->SetSimulatePhysics(false);
	//tongue->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PhysicsConstraint->SetLinearXLimit(LCM_Limited, tongueLimit);
	PhysicsConstraint->SetLinearYLimit(LCM_Limited, tongueLimit);
	PhysicsConstraint->SetLinearZLimit(LCM_Limited, tongueLimit);
	//tongue->AttachToComponent(SphereComponent, FAttachmentTransformRules::KeepWorldTransform);
}

// Called every frame
void ACharacterPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//if(TongueState == Shot)
	//	PhysicsConstraintFunc(DeltaTime);

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

	// Retracting
	if (TongueState == Retracting)
	{
		tongue->SetPhysicsLinearVelocity(FVector::ZeroVector);

		FVector NewLocation = FMath::Lerp(
			tongue->GetComponentLocation(),
			mouthHolder->GetComponentLocation(),
			DeltaTime * TongRetractSpeed);
		tongue->SetWorldLocation(NewLocation);

		float tongueDist = FVector::Distance(tongue->GetComponentLocation(), mouthHolder->GetComponentLocation());

		if (tongueDist <= TongMinLength)
		{
			TongueState = Retracted;
			ResetTongue();
		}
		else {
			PhysicsConstraint->SetLinearXLimit(LCM_Limited, tongueDist);
			PhysicsConstraint->SetLinearYLimit(LCM_Limited, tongueDist);
			PhysicsConstraint->SetLinearZLimit(LCM_Limited, tongueDist);
		}
	}
}

// Called to bind functionality to input
void ACharacterPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Bind movement actions
	PlayerInputComponent->BindAxis("MoveHorizontal", this, &ACharacterPawn::MoveHorizontal);
	PlayerInputComponent->BindAxis("MoveVertical", this, &ACharacterPawn::MoveVertical);

	// Bind button actions
	PlayerInputComponent->BindAction("Shoot", IE_Pressed, this, & ACharacterPawn::ShootTongue);
	PlayerInputComponent->BindAction("Grab", IE_Pressed, this, & ACharacterPawn::StartGrab);
	PlayerInputComponent->BindAction("Grab", IE_Released, this, & ACharacterPawn::StopGrab);
	PlayerInputComponent->BindAction("Retract", IE_Pressed, this, & ACharacterPawn::RetractTongue);
	PlayerInputComponent->BindAction("Retract", IE_Released, this, &ACharacterPawn::StopRetracting);
}

void ACharacterPawn::ResetTongue()
{	
	//tongue->SetSimulatePhysics(false);
	//tongue->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//tongue->AttachToComponent(SphereComponent, FAttachmentTransformRules::KeepWorldTransform);

	//tongueLimit = tongueLimit - TongRetractSpeed;
	//PhysicsConstraint->SetLinearXLimit(LCM_Limited, tongueLimit);
	//PhysicsConstraint->SetLinearYLimit(LCM_Limited, tongueLimit);
	//PhysicsConstraint->SetLinearZLimit(LCM_Limited, tongueLimit);
}

void ACharacterPawn::MoveHorizontal(float Value)
{
	CurrentVelocity.X = Value;
}

void ACharacterPawn::MoveVertical(float Value)
{
	CurrentVelocity.Y = Value;
}

void ACharacterPawn::ShootTongue()
{
	if (TongueState == Retracted)
	{
		TongueState = Shot;
		tongue->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);

		tongueLimit = TongMaxLength;
		PhysicsConstraint->SetLinearXLimit(LCM_Limited, TongMaxLength);
		PhysicsConstraint->SetLinearYLimit(LCM_Limited, TongMaxLength);
		PhysicsConstraint->SetLinearZLimit(LCM_Limited, TongMaxLength);

		//tongue->SetSimulatePhysics(true);
		//tongue->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		FVector ForwardDirection = GetActorForwardVector();
		FVector impulse = ForwardDirection * TongEjectSpeed;
		tongue->AddImpulse(impulse);
	}
}

void ACharacterPawn::StartGrab()
{
	// Perform hit check to find the object to grab
	AActor* TargetObject = PerformHitCheck();
	if (TargetObject)
	{
		// Create and configure the physics constraint
		GrabConstraint = NewObject<UPhysicsConstraintComponent>(this);
		GrabConstraint->AttachToComponent(tongue, FAttachmentTransformRules::KeepWorldTransform);
		GrabConstraint->SetConstrainedComponents(tongue, NAME_None, Cast<UPrimitiveComponent>(TargetObject->GetRootComponent()), NAME_None);

		// Additional configuration for the constraint as needed
		// ...

		// Keep a reference to the grabbed object
		GrabbedObject = TargetObject;

		// Activate the constraint
		GrabConstraint->SetActive(true);
	}
}

void ACharacterPawn::StopGrab()
{
	if (GrabbedObject)
	{
		// Disable or destroy the constraint
		GrabConstraint->SetActive(false);
		GrabConstraint->DestroyComponent();

		// Reset the grabbed object reference
		GrabbedObject = nullptr;
	}
}

void ACharacterPawn::RetractTongue()
{
	if(TongueState == Shot)
		TongueState = Retracting;
}

void ACharacterPawn::StopRetracting()
{
	if (TongueState == Retracted)
		return;

	if (FVector::Distance(tongue->GetComponentLocation(), mouthHolder->GetComponentLocation()) <= TongMinLength)
	{
		TongueState = Retracted;
		ResetTongue();
	}
	else
		TongueState = Shot;
}

void ACharacterPawn::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor == GetOwner())
		return;

	tongueLimit = FVector::Distance(tongue->GetComponentLocation(), mouthHolder->GetComponentLocation());
	PhysicsConstraint->SetLinearXLimit(LCM_Limited, tongueLimit);
	PhysicsConstraint->SetLinearYLimit(LCM_Limited, tongueLimit);
	PhysicsConstraint->SetLinearZLimit(LCM_Limited, tongueLimit);
}

AActor* ACharacterPawn::PerformHitCheck()
{
	FVector Start = GetActorLocation(); // or the location of the character's hand/viewpoint
	FVector ForwardVector = GetActorForwardVector();
	FVector End = ((ForwardVector * 10.f) + Start); // 1000.f is the distance of the sweep
	FHitResult HitResult;

	// Collision parameters
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this); // Ignore the character itself

	// Sphere sweep radius
	float SphereRadius = 5.0f; // Adjust the radius as needed

	// Perform the sphere sweep
	bool bHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		Start,
		End,
		FQuat::Identity,
		ECC_Visibility, // or your custom collision channel
		FCollisionShape::MakeSphere(SphereRadius),
		CollisionParams
	);

	if (bHit)
	{
		// Draw a debug sphere - useful for testing
		DrawDebugSphere(GetWorld(), HitResult.Location, SphereRadius, 32, FColor::Green, false, 1, 0, 1);

		// Return the hit actor
		return HitResult.GetActor();
	}
	else
	{
		DrawDebugSphere(GetWorld(), HitResult.Location, SphereRadius, 32, FColor::Red, false, 1, 0, 1);
		// No hit
		return nullptr;
	}
}

