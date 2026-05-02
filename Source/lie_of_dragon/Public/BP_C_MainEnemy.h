// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "Components/StateTreeComponent.h"
#include "Components/WidgetComponent.h"


#include "GameFramework/Character.h"
#include "Components/WidgetComponent.h"



#include "BP_C_MainEnemy.generated.h"





class USpringArmComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
class UWidgetComponent;
class USphereComponent;
class USkeletalMeshComponent;

	

	


UCLASS()



class ABP_C_MainEnemy : public ACharacter

{
	GENERATED_BODY()

public:
	
	// Sets default values for this character's properties
	ABP_C_MainEnemy();

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
	
	
	void EnemyMove(const FInputActionValue& ActionValue);
	
	/** Please add a variable description */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Default")
	UWidgetComponent* WidgetHealthBar;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Default")
	UStateTreeComponent* StateTree;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Default")
	USphereComponent* QTEZoneTEMP;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Default")
	USkeletalMeshComponent* BatMesh;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Variables")
	double C_MaxHealth = 100;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Variables")
	double C_CurrentHealth = 100;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Variables")
	double C_DamagePerSuccess = 35;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Variables")
	double C_DamageToPlayerOnFail = 25;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Variables")
	double C_AttackOnFailDelay = 0.2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> MyEnemyWidgetClass; // Сюда в редакторе выберете нужный виджет

	UPROPERTY()
	TObjectPtr<UUserWidget> MyEnemyWidgetInstance; // Ссылка на созданный экземпляр


	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	//UC_BP_EnemyWidget* EnemyHealthBar ;




};


