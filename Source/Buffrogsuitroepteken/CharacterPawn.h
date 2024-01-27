// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CharacterPawn.generated.h"

UCLASS()
class BUFFROGSUITROEPTEKEN_API ACharacterPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACharacterPawn();

	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* mainBody;
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* mouthHolder;
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* tongue;
	//UPROPERTY(EditAnywhere)
	//class UProjectileMovementComponent* ProjectileMovementComponent;
	//UPROPERTY(EditAnywhere)
	//class UCableComponent* cable;
	UPROPERTY(EditAnywhere)
	class UPhysicsConstraintComponent* PhysicsConstraint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "tong parameters")
	float TongEjectSpeed = 5000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "tong parameters")
	float TongRetractSpeed = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "tong parameters")
	float TongMaxLength = 500.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "movement parameters")
	float MovementSpeed = 100.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	// Movement methods
	void MoveHorizontal(float Value);
	void MoveVertical(float Value);

	// Current velocity
	FVector2D CurrentVelocity;
};
