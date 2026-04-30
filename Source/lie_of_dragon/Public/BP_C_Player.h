// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "BP_C_Player.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;

UCLASS()
class ABP_C_Player : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABP_C_Player();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
	/*
	 * Controller
	*/
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* MappingContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* DashAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* InteractAction;
	
	void PlayerJump();
	void PlayerMove(const FInputActionValue& ActionValue);
	void PlayerDash(const FInputActionValue& ActionValue);
	void PlayerLook(const FInputActionValue& ActionValue);
	void PlayerInteract();
	
	UPROPERTY(EditAnywhere, Category = "Interact")
	float InteractRadius = 200.f;
		
	UPROPERTY(EditAnywhere, Category = "Interact")
	float InteractDebugDuration = 1.f;
	
	/*
	 * Camera
	 */
	UPROPERTY(EditAnywhere, Category = "Camera")
	UCameraComponent* PlayerCamera;
	
	UPROPERTY(EditAnywhere, Category = "Camera")
	USpringArmComponent* SpringArm;
	
	/*
	 * Weapons 
	 */
	UPROPERTY(EditAnywhere, Category = "Weapon")
	UStaticMeshComponent* WeaponMesh;
	
	UPROPERTY(EditAnywhere, Category = "Weapon")
	UClass* Projectile;
	
	UPROPERTY(EditAnywhere, Category = "Weapon")
	FName HandleSocket;
	
	UPROPERTY(EditAnywhere, Category = "Weapon")
	FName ProjectileMuzzleSocket;
	
};
