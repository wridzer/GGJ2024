// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TongCharacter.generated.h"

UCLASS()
class BUFFROGSUITROEPTEKEN_API ATongCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATongCharacter();

	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* MainBody;
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* tongue;
	//UPROPERTY(EditAnywhere)
	//class UProjectileMovementComponent* ProjectileMovementComponent;
	UPROPERTY(EditAnywhere)
	class UCableComponent* cable;
	UPROPERTY(EditAnywhere)
	class UPhysicsConstraintComponent* PhysicsConstraint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "tong parameters")
	float TongEjectSpeed = 5000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "tong parameters")
	float TongRetractSpeed = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "tong parameters")
	float TongMaxLength = 500.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "tong parameters")
	FVector MouthPoint = FVector(0.0f, 0.0f, 159.0f);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
