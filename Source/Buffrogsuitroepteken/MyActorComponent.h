// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MyActorComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUFFROGSUITROEPTEKEN_API UMyActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMyActorComponent();

	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* tong;
	UPROPERTY(VisibleAnywhere)
	class UProjectileMovementComponent* ProjectileMovementComponent;
	UPROPERTY(EditAnywhere)
	class UCableComponent* cable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "tong parameters")
	float TongEjectSpeed = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "tong parameters")
	float TongRetractSpeed = 10.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "tong parameters")
	float TongMaxLength = 100.0f;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
