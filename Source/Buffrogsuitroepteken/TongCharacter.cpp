// Fill out your copyright notice in the Description page of Project Settings.


#include "TongCharacter.h"
#include "CableComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"

// Sets default values
ATongCharacter::ATongCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MainBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("body"));
	SetRootComponent(MainBody);

	// construct sphere component
	tongue = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("tongue"));
	tongue->SetupAttachment(MainBody);

	PhysicsConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("PhysicsConstraint"));
	PhysicsConstraint->SetupAttachment(MainBody);
	PhysicsConstraint->SetConstrainedComponents(MainBody, "", tongue, "");
	PhysicsConstraint->SetRelativeLocation(MouthPoint);
	PhysicsConstraint->SetLinearXLimit(LCM_Limited, 100.0f);
	PhysicsConstraint->SetLinearYLimit(LCM_Limited, 100.0f);
	PhysicsConstraint->SetLinearZLimit(LCM_Limited, 100.0f);

	// construct rope
	cable = CreateDefaultSubobject<UCableComponent>(TEXT("Cable"));
	cable->SetupAttachment(MainBody);
	cable->SetAttachEndToComponent(tongue);
	cable->SetRelativeLocation(MouthPoint);

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
void ATongCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATongCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATongCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

