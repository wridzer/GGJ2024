// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CharacterPawn.generated.h"

UENUM()
enum TongueState
{
	Retracted,
	Shot,
	Retracting
};

UCLASS()
class BUFFROGSUITROEPTEKEN_API ACharacterPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACharacterPawn();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	class USkeletalMeshComponent* mainBody;
	UPROPERTY(VisibleAnywhere)
	class USphereComponent* SphereComponent;
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* mouthHolder;
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* tongue;
	UPROPERTY(EditAnywhere)
	class UPhysicsConstraintComponent* PhysicsConstraint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "tong parameters")
	float TongEjectSpeed = 5000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "tong parameters")
	float TongRetractSpeed = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "tong parameters")
	float TongMaxLength = 500.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "tong parameters")
	float TongMinLength = 5.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "movement parameters")
	float MovementSpeed = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "movement parameters")
	float RotateSpeed = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "movement parameters")
	float JumpHight = 5.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "movement parameters")
	float JumpGravity = 1.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


private:

	// Input methods
	void MoveHorizontal(float Value);
	void MoveVertical(float Value);
	void jump();
	void ShootTongue();
	void StartGrab();
	void StopGrab();
	void RetractTongue();
	void StopRetracting();

	TongueState TongueState = TongueState::Retracted;

	//UFUNCTION()
	//void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	void ResetTongue();

	FVector CurrentVelocity;
	float tongueLimit = 0.0f;

	float RestLength = TongMaxLength;
	float SpringStiffness = 1;
	float Damping = 20;

	// Grabbing
	AActor* PerformHitCheck();
	UPhysicsConstraintComponent* GrabConstraint;
	AActor* GrabbedObject;
};
