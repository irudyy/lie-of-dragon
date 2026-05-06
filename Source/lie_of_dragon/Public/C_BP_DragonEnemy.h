// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/StateTreeComponent.h"
#include "InputActionValue.h"

#include "C_BP_DragonEnemy.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
class USphereComponent;
class USkeletalMeshComponent;
class UC_BP_EnemyWidget;
class UUserWidget;
	




UCLASS()
class LIE_OF_DRAGON_API AC_BP_DragonEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AC_BP_DragonEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* DragonInputComponent) override;

	
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* DragonMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* DragonMoveAction;
	
	void DragonMove(const FInputActionValue& ActionValue);
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Components")
	UStateTreeComponent* DragonStateTree;

	/** Please add a variable description 
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Default")
	USphereComponent* QTEZoneTEMP;

	 Please add a variable description 
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Components")
	USkeletalMeshComponent* DradonMesh;*/
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Variables")
	double C_MaxHealth = 100;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Variables")
	double C_CurrentHealth = 100;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Variables")
	double C_DamagePerSuccess = 9;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Variables")
	double C_DamageToPlayerOnFail = 51;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Variables")
	double C_AttackOnFailDelay = 0.2;
	
};
