// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimMontage.h"
#include "C_WBP_MainUI.h"
#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "Blueprint/UserWidget.h"
#include "BP_C_Player.generated.h"


class USpringArmComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
class ABP_C_MainEnemy;

UENUM(BlueprintType)
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
	ABP_C_Player();

protected:

	virtual void BeginPlay() override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

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
	UInputAction* PauseAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI|Pause")
	TSubclassOf<UUserWidget> PauseMenuClass;

	UPROPERTY(BlueprintReadOnly, Category = "UI|Pause")
	TObjectPtr<UUserWidget> PauseMenuInstance;
	
	UPROPERTY(EditAnywhere, Category = "Input|QTE")
	UInputAction* QTEUpAction;

	UPROPERTY(EditAnywhere, Category = "Input|QTE")
	UInputAction* QTEDownAction;

	UPROPERTY(EditAnywhere, Category = "Input|QTE")
	UInputAction* QTELeftAction;

	UPROPERTY(EditAnywhere, Category = "Input|QTE")
	UInputAction* QTERightAction;
	
	void PlayerJump();
	
	void PlayerMove(const FInputActionValue& ActionValue);
	
	void PlayerStopMove(const FInputActionValue& ActionValue);
	
	void PlayerStartDash(const FInputActionValue& ActionValue);
	
	void PlayerEndDash(const FInputActionValue& ActionValue);
	
	void PlayerPause();
	
	void QTEInputUp(const FInputActionValue& ActionValue);
	
	void QTEInputDown(const FInputActionValue& ActionValue);
	
	void QTEInputLeft(const FInputActionValue& ActionValue);
	
	void QTEInputRight(const FInputActionValue& ActionValue);
	
	void PlayerLook(const FInputActionValue& ActionValue);
	
	void PlayerSecond();
	
	
	UPROPERTY(EditAnywhere, Category = "Interact")
	float InteractRadius = 200.0f;
		
	UPROPERTY(EditAnywhere, Category = "Interact")
	float InteractDebugDuration = 1.0f;
	
	UPROPERTY(EditAnywhere, Category = "Timer")
	int TickCounter = 0;
	
	///-------------------------------------------
	///Variables
	///------------------------------------------
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="QTE")
	bool C_bInsideBatZone = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="QTE")
	bool C_bQTEActive = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="QTE")
	TArray<E_QTEDirection> C_QTESequence;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="QTE")
	int32 C_QTECurrentIndex = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="QTE")
	int32 C_QTESequenceLength = 5;

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> MyWidgetClass; // Сюда в редакторе выберете нужный виджет

	UPROPERTY()
	TObjectPtr<UUserWidget> MyWidgetInstance; // Ссылка на созданный экземпляр
	
	//Death or Win Timer (DoW Timer)
	FTimerHandle DoWTimerHandle;
	//1 second timer
	FTimerHandle SecTimerHandle;
	//enemy animation Delay
	FTimerHandle EnemyAnimationDelay;
	
	//default animation Delay
	FTimerHandle DefaultAnimationDelay;
	
	//dobavil
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI|QTE")
	TSubclassOf<UUserWidget> QTEWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = "UI|QTE")
	TObjectPtr<UUserWidget> QTEWidgetInstance;

	
	
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
	
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default") bool playerIsDead=false;
	
	UPROPERTY(BlueprintReadWrite, Category = "UI")
	UC_WBP_MainUI* MainUII;

	///-------------------------------------------
	/// Camera
	///------------------------------------------
	UPROPERTY(EditAnywhere, Category = "Camera")
	UCameraComponent* PlayerCamera;
	
	UPROPERTY(EditAnywhere, Category = "Camera")
	USpringArmComponent* SpringArm;
	
	///-------------------------------------------
	///Animations
	///------------------------------------------

	UFUNCTION(BlueprintCallable, Category = "Default")
	void C_UpdPlayerAnimation(int numAnim, bool loopAnim);
	
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TArray<UAnimSequence*> Animations; // instead UAnimMontage*
	
	UPROPERTY()
	int32 CurrentAnimIndex = -1; // -1 = nothing plays
	
	///-------------------------------------------
	///UFunctions
	///------------------------------------------

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
	
	
	
	
	
	UFUNCTION(BlueprintNativeEvent)
	void LavaDamage();
	virtual void LavaDamage_Implementation();
};
