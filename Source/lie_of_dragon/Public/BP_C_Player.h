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
class ABP_C_MainEnemy; //добавил 

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
	
	//dobavil
	UPROPERTY(EditAnywhere, Category = "Input|QTE")
	UInputAction* QTEUpAction;

	UPROPERTY(EditAnywhere, Category = "Input|QTE")
	UInputAction* QTEDownAction;

	UPROPERTY(EditAnywhere, Category = "Input|QTE")
	UInputAction* QTELeftAction;

	UPROPERTY(EditAnywhere, Category = "Input|QTE")
	UInputAction* QTERightAction;
	//dobavil
	
	void PlayerJump();
	void PlayerMove(const FInputActionValue& ActionValue);
	void PlayerStartDash(const FInputActionValue& ActionValue);
	void PlayerEndDash(const FInputActionValue& ActionValue);
	
	//dobavil
	void QTEInputUp(const FInputActionValue& ActionValue);
	void QTEInputDown(const FInputActionValue& ActionValue);
	void QTEInputLeft(const FInputActionValue& ActionValue);
	void QTEInputRight(const FInputActionValue& ActionValue);
	//dobavil
	
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
	//pomenyal i dobavil
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="QTE")
	bool C_bInsideBatZone = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="QTE")
	bool C_bQTEActive = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="QTE")
	TArray<E_QTEDirection> C_QTESequence;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="QTE")
	int32 C_QTECurrentIndex = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="QTE")
	int32 C_QTESequenceLength = 4;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="QTE")
	double C_QTETimeLimit = 5.0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="QTE")
	double C_QTERemainingTime = 0.0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="QTE")
	TObjectPtr<ABP_C_MainEnemy> C_CurrentBatEnemy = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="QTE")
	bool C_bAutoStartQTEOnEnterZone = true;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="QTE")
	bool C_bBlockMovementDuringQTE = true;

	FTimerHandle C_QTENextRoundTimerHandle;
	//pomenyal i dobavil
	
	/** Please add a variable description --------------------------------------------------------- */
	//UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default", meta=(EditInline="true"))
	//WBP_QTECombat_C* C_QTEWidgetRef;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> MyWidgetClass; // Сюда в редакторе выберете нужный виджет

	UPROPERTY()
	TObjectPtr<UUserWidget> MyWidgetInstance; // Ссылка на созданный экземпляр
	
	
	
	//dobavil
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI|QTE")
	TSubclassOf<UUserWidget> QTEWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = "UI|QTE")
	TObjectPtr<UUserWidget> QTEWidgetInstance;
	//dobavil
	
	
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
	
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	float damage=25.f;
	
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	float heal=25.f;
	

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
	
	//dobavil
	UFUNCTION(BlueprintCallable, Category = "QTE")
	void C_StartQTERound();

	UFUNCTION(BlueprintCallable, Category = "QTE", meta=(DisplayName="HandleQTEInput"))
	void C_HandleQTEInput(E_QTEDirection PressedDirection);

	UFUNCTION(BlueprintCallable, Category = "QTE")
	void C_FaliRound();

	UFUNCTION(BlueprintCallable, Category = "QTE")
	void C_SuccessRound();

	UFUNCTION(BlueprintCallable, Category = "QTE")
	void C_EnterBatZone(ABP_C_MainEnemy* BatEnemy);

	UFUNCTION(BlueprintCallable, Category = "QTE")
	void C_ExitBatZone(ABP_C_MainEnemy* BatEnemy);

	UFUNCTION(BlueprintCallable, Category = "QTE")
	void C_TakeDamageFromEnemy();

	UFUNCTION(BlueprintCallable, Category = "QTE")
	bool C_IsQTEActive() const;

	UFUNCTION(BlueprintImplementableEvent, Category = "QTE|UI")
	void BP_QTE_SetVisible(bool bVisible);

	UFUNCTION(BlueprintImplementableEvent, Category = "QTE|UI")
	void BP_QTE_SetupSequence(const TArray<E_QTEDirection>& Sequence);

	UFUNCTION(BlueprintImplementableEvent, Category = "QTE|UI")
	void BP_QTE_HighlightArrow(int32 ArrowIndex);

	UFUNCTION(BlueprintImplementableEvent, Category = "QTE|UI")
	void BP_QTE_ShowResult(bool bSuccess);
	//dobavil
	
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
