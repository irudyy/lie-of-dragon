// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

class WBP_QTECombat_C;


#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "Blueprint/UserWidget.h"
#include "BP_C_Player.generated.h"



class USpringArmComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;


UENUM(BlueprintType)                    // ← Обязательно для Blueprint
enum class E_QTEDirection : uint8
{
	Up          UMETA(DisplayName = "Up"),
	Down        UMETA(DisplayName = "Down"),
	Left        UMETA(DisplayName = "Left"),
	Right       UMETA(DisplayName = "Right"),
};


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

	void PlayerJump();
	void PlayerMove(const FInputActionValue& ActionValue);
	void PlayerStartDash(const FInputActionValue& ActionValue);
	void PlayerEndDash(const FInputActionValue& ActionValue);
	//void PlayerLook(const FInputActionValue& ActionValue);----------------------
	void PlayerSecond();
	
	
	UPROPERTY(EditAnywhere, Category = "Interact")
	float InteractRadius = 200.0f;
		
	UPROPERTY(EditAnywhere, Category = "Interact")
	float InteractDebugDuration = 1.0f;
	
	UPROPERTY(EditAnywhere, Category = "Stammina")
	float StamminaProcent = 100.0f;
	
	UPROPERTY(EditAnywhere, Category = "Timer")
	int TickCounter = 0;
	
	///-------------------------------------------
	///Variables
	///------------------------------------------
	
		/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	bool C_bInsideBatZone;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	bool C_bQTEActive;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	TArray<E_QTEDirection> C_QTESequence;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	int C_QTECurrentIndex;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	int C_QTESequenceLength;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	double C_QTETimeLimit;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	double C_QTERemainingTime;

	
	
	/** Please add a variable description --------------------------------------------------------- */
	//UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default", meta=(EditInline="true"))
	//WBP_QTECombat_C* C_QTEWidgetRef;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> MyWidgetClass; // Сюда в редакторе выберете нужный виджет

	UPROPERTY()
	TObjectPtr<UUserWidget> MyWidgetInstance; // Ссылка на созданный экземпляр
	
	
	
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	double C_MaxHealth =100;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	double C_CurrentHealth=100;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	bool C_bIsDead;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	double C_DeathRestartDelay;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	bool C_Animation_Run_Stay;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	bool C_Animation_Attack;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	double C_multiplication;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	bool C_Is_Dash_On_Player;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	double C_CurrentStammina=100;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	double C_MaxStammina=100;

	/*
	 * Camera
	 */
	UPROPERTY(EditAnywhere, Category = "Camera")
	UCameraComponent* PlayerCamera;
	
	UPROPERTY(EditAnywhere, Category = "Camera")
	USpringArmComponent* SpringArm;
	
	
	
	///-------------------------------------------
	///UFunctions
	///------------------------------------------
	
	UFUNCTION(BlueprintCallable, Category = "QTE")
	void C_StartQTERound();
	UFUNCTION(BlueprintCallable, Category = "QTE")
	void C_FaliRound();
	UFUNCTION(BlueprintCallable, Category = "QTE")
	void C_SuccessRound();
	UFUNCTION(BlueprintCallable, Category = "QTE")
	void C_EnterBatZone();
	UFUNCTION(BlueprintCallable, Category = "QTE")
	void C_ExitBatZone();
	UFUNCTION(BlueprintCallable, Category = "QTE")
	void C_TakeDamageFromEnemy();
	UFUNCTION(BlueprintCallable, Category = "Default")
	void C_HPBuff();
	UFUNCTION(BlueprintCallable, Category = "Default")
	void C_LavaDamage();
	UFUNCTION(BlueprintCallable, Category = "Default")
	void C_HealthBarSettings();
	UFUNCTION(BlueprintCallable, Category = "Default")
	void C_UpdPlayerHealth();
	UFUNCTION(BlueprintCallable, Category = "Default")
	void C_UpdPlayerStammina();
};
