// Fill out your copyright notice in the Description page of Project Settings.

#include "BP_C_Player.h"
#include "BP_C_MainEnemy.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputCoreTypes.h"
#include "Kismet/GameplayStatics.h" 
#include "TimerManager.h"
#include "C_WBP_MainUI.h"
#include "Components/CapsuleComponent.h"

ABP_C_Player::ABP_C_Player()
{
	GetCapsuleComponent()->InitCapsuleSize(99.f, 99.0f);
	PrimaryActorTick.bCanEverTick = true;
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArm->SetupAttachment(GetRootComponent());
	
	PlayerCamera = CreateDefaultSubobject<UCameraComponent>("Camera");
	PlayerCamera->SetupAttachment(SpringArm);
}

void ABP_C_Player::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(SecTimerHandle, [this]()
			{
				PlayerSecond();
			}, 1.0f, false);
	
	MainUII = Cast<UC_WBP_MainUI>(MyWidgetInstance);
	
	C_UpdPlayerAnimation(1, true);
	
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer())
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
			{
				Subsystem->AddMappingContext(MappingContext, 0);
			}
		}
	}

	if (MyWidgetClass)
	{
		APlayerController* PC = GetWorld()->GetFirstPlayerController();
		MyWidgetInstance = CreateWidget<UC_WBP_MainUI>(PC, MyWidgetClass);
		if (MyWidgetInstance)
		{
			MyWidgetInstance->AddToViewport();
		}
	}

	if (QTEWidgetClass)
	{
		APlayerController* PC = GetWorld()->GetFirstPlayerController();
		QTEWidgetInstance = CreateWidget<UUserWidget>(PC, QTEWidgetClass);
		if (QTEWidgetInstance)
		{
			QTEWidgetInstance->AddToViewport();
			QTEWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void ABP_C_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (C_CurrentHealth > C_MaxHealth)
	{
		C_CurrentHealth = C_MaxHealth;
	}
	
	if (C_bQTEActive)
	{
		C_QTERemainingTime -= DeltaTime;
		if (C_QTERemainingTime <= 0.0)
		{
			C_QTERemainingTime = 0.0;
			C_FaliRound();
		}
	}
}

// Called to bind functionality to input
void ABP_C_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if 	(UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent -> BindAction(JumpAction, ETriggerEvent::Triggered, this, &ABP_C_Player::PlayerJump);
		EnhancedInputComponent -> BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABP_C_Player::PlayerMove);
		EnhancedInputComponent -> BindAction(MoveAction, ETriggerEvent::Completed, this, &ABP_C_Player::PlayerStopMove);
		EnhancedInputComponent -> BindAction(LookAction, ETriggerEvent::Triggered, this, &ABP_C_Player::PlayerLook);
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Started, this, &ABP_C_Player::PlayerStartDash);
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Completed, this, &ABP_C_Player::PlayerEndDash);
		
		if (PauseAction)	EnhancedInputComponent->BindAction(PauseAction, 	ETriggerEvent::Started, this, &ABP_C_Player::PlayerPause);
		if (QTEUpAction)	EnhancedInputComponent->BindAction(QTEUpAction,		ETriggerEvent::Started, this, &ABP_C_Player::QTEInputUp);
		if (QTEDownAction)	EnhancedInputComponent->BindAction(QTEDownAction, 	ETriggerEvent::Started, this, &ABP_C_Player::QTEInputDown);
		if (QTELeftAction)	EnhancedInputComponent->BindAction(QTELeftAction, 	ETriggerEvent::Started, this, &ABP_C_Player::QTEInputLeft);
		if (QTERightAction)	EnhancedInputComponent->BindAction(QTERightAction, 	ETriggerEvent::Started, this, &ABP_C_Player::QTEInputRight);
	}
}

void ABP_C_Player::PlayerPause()
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	
	if (!PC) PC = UGameplayStatics::GetPlayerController(this, 0);
	if (!PC) return;
	
	const bool bNewPaused = !UGameplayStatics::IsGamePaused(this);
	UGameplayStatics::SetGamePaused(this, bNewPaused);

	if (bNewPaused)
	{
		if (!PauseMenuInstance && PauseMenuClass)	PauseMenuInstance = CreateWidget<UUserWidget>(PC, PauseMenuClass);
		if (PauseMenuInstance && !PauseMenuInstance->IsInViewport())	PauseMenuInstance->AddToViewport(100); 
		
		PC->SetShowMouseCursor(true);
		FInputModeGameAndUI InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		InputMode.SetHideCursorDuringCapture(false);
		if (PauseMenuInstance) InputMode.SetWidgetToFocus(PauseMenuInstance->TakeWidget());
		PC->SetInputMode(InputMode);
	}
	
	else
	{
		if (PauseMenuInstance && PauseMenuInstance->IsInViewport())
		{
			PauseMenuInstance->RemoveFromParent();
		}
		PC->SetShowMouseCursor(false);
		PC->SetInputMode(FInputModeGameOnly());
	}
}

void ABP_C_Player::PlayerJump()
{
	Jump();
}

void ABP_C_Player::PlayerMove(const FInputActionValue& ActionValue)
{
	FVector2D ActionVector = ActionValue.Get<FVector2D>();
	
	float Angle = FMath::RadiansToDegrees(FMath::Atan2(ActionVector.X, ActionVector.Y));

	GetMesh()->SetRelativeRotation(FRotator(0.f, Angle-90.f, 0.f));
	
	AddMovementInput(GetActorForwardVector(), ActionVector.Y);
	
	AddMovementInput(GetActorRightVector(), ActionVector.X);
	
	C_UpdPlayerAnimation(2, true);
}

void ABP_C_Player::PlayerStopMove(const FInputActionValue& ActionValue)
{
	C_UpdPlayerAnimation(1, false);
}

void ABP_C_Player::PlayerStartDash(const FInputActionValue& ActionValue)
{
	if (C_CurrentStammina <= 0)
	{
		GetCharacterMovement()->MaxWalkSpeed = 400.0f;
		C_UpdPlayerAnimation(2, true);
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = 800.0f;
		C_UpdPlayerAnimation(5, true);
	}
}

void ABP_C_Player::PlayerEndDash(const FInputActionValue& ActionValue)
{
	GetCharacterMovement()->MaxWalkSpeed = 400.0f;
}

void ABP_C_Player::QTEInputUp(const FInputActionValue& ActionValue)
{
	C_HandleQTEInput(E_QTEDirection::Up);
}
void ABP_C_Player::QTEInputDown(const FInputActionValue& ActionValue)
{
	C_HandleQTEInput(E_QTEDirection::Down);
}
void ABP_C_Player::QTEInputLeft(const FInputActionValue& ActionValue)
{
	C_HandleQTEInput(E_QTEDirection::Left);
}
void ABP_C_Player::QTEInputRight(const FInputActionValue& ActionValue)
{
	C_HandleQTEInput(E_QTEDirection::Right);
}

void ABP_C_Player::PlayerLook(const FInputActionValue& ActionValue)
{
	FVector2D ActionRotation = ActionValue.Get<FVector2D>();
	AddControllerYawInput(ActionRotation.X);
	AddControllerPitchInput(ActionRotation.Y);
}

void ABP_C_Player::PlayerSecond()
{
	if (!IsValid(this) || playerIsDead) return;
	
	if (C_CurrentHealth <= 0 && playerIsDead == false)
	{
		playerIsDead = true;
		GetWorldTimerManager().ClearTimer(SecTimerHandle);
		
		GetWorldTimerManager().ClearTimer(EnemyAnimationDelay);
		
		GetWorldTimerManager().ClearTimer(C_QTENextRoundTimerHandle);
		
		FTimerDelegate TimerDel;
		TimerDel.BindLambda([this]()
		{
			UGameplayStatics::OpenLevel(this, FName("MenuLevel"));
		});
		GetWorldTimerManager().SetTimer(DoWTimerHandle, TimerDel, 5.0f, false);
		
		GetWorldTimerManager().UnPauseTimer(DoWTimerHandle);

		return;
	}

	if (C_CurrentStammina <= 0)		GetCharacterMovement()->MaxWalkSpeed = 400.0f;
	if (GetCharacterMovement()->MaxWalkSpeed == 800.0f && C_CurrentStammina != 0)		C_CurrentStammina = C_CurrentStammina - 40;
	if (C_CurrentStammina < 100)	C_CurrentStammina = C_CurrentStammina + 20;

	UC_WBP_MainUI* MainUI = Cast<UC_WBP_MainUI>(MyWidgetInstance);
	
	if (!IsValid(MainUI)) return;
	
	MainUI->UpdateStammFromPlayer(C_CurrentStammina / C_MaxStammina);
	
	MainUI->UpdateHPFromPlayer(C_CurrentHealth / C_MaxHealth);

	GetWorldTimerManager().SetTimer(SecTimerHandle, [this]()
	{
		PlayerSecond();
	}, 1.0f, false);
}

void ABP_C_Player::C_StartQTERound()
{
	if (C_bQTEActive)
	{
		return;
	}

	if (!C_bInsideBatZone)
	{
		return;
	}

	if (!IsValid(C_CurrentBatEnemy))
	{
		C_bQTEActive = false;
		return;
	}

	GetWorldTimerManager().ClearTimer(C_QTENextRoundTimerHandle);
	
	if (C_QTESequenceLength <= 0)
	{
		C_QTESequenceLength = 5;
	}

	if (C_QTETimeLimit <= 0.0)
	{
		C_QTETimeLimit = 5.0;
	}

	C_QTESequence.Empty();
	
	C_QTECurrentIndex = 0;
	
	C_QTERemainingTime = C_QTETimeLimit;
	
	C_bQTEActive = true;

	for (int32 i = 0; i < C_QTESequenceLength; i++)
	{
		const int32 RandomDirection = FMath::RandRange(0, 3);

		switch (RandomDirection)
		{
		case 0:
			C_QTESequence.Add(E_QTEDirection::Up);
			break;
		case 1:
			C_QTESequence.Add(E_QTEDirection::Down);
			break;
		case 2:
			C_QTESequence.Add(E_QTEDirection::Left);
			break;
		case 3:
			C_QTESequence.Add(E_QTEDirection::Right);
			break;
		default:
			C_QTESequence.Add(E_QTEDirection::Up);
			break;
		}
	}

	if (QTEWidgetInstance)
	{
		QTEWidgetInstance->SetVisibility(ESlateVisibility::Visible);
	}
	BP_QTE_SetVisible(true);
	
	BP_QTE_SetupSequence(C_QTESequence);
	
	BP_QTE_HighlightArrow(C_QTECurrentIndex);
}

void ABP_C_Player::C_HandleQTEInput(E_QTEDirection PressedDirection)
{
	if (!C_bQTEActive)
	{
		return;
	}

	if (!C_bInsideBatZone)
	{
		C_FaliRound();
		return;
	}

	if (!IsValid(C_CurrentBatEnemy))
	{
		C_FaliRound();
		return;
	}

	if (!C_QTESequence.IsValidIndex(C_QTECurrentIndex))
	{
		C_FaliRound();
		return;
	}

	const E_QTEDirection ExpectedDirection = C_QTESequence[C_QTECurrentIndex];

	if (PressedDirection != ExpectedDirection)
	{
		C_FaliRound();
		return;
	}
	C_QTECurrentIndex++;
	
	if (C_QTECurrentIndex >= C_QTESequence.Num())
	{
		C_SuccessRound();
		return;
	}
	C_QTERemainingTime = C_QTETimeLimit;
	
	BP_QTE_HighlightArrow(C_QTECurrentIndex);
}

void ABP_C_Player::C_SuccessRound()
{
	ABP_C_MainEnemy* enemy = Cast<ABP_C_MainEnemy>(C_CurrentBatEnemy);
	
	if (!IsValid(enemy)) return;
	
	enemy->C_UpdEnemyAnimation(3);
	
	GetWorldTimerManager().SetTimer(EnemyAnimationDelay, [this]()
		{
			ABP_C_MainEnemy* eenemy = Cast<ABP_C_MainEnemy>(C_CurrentBatEnemy);
			if (!IsValid(eenemy)) return;
			eenemy->C_UpdEnemyAnimation(1);

		},0.76f, false);
	
	C_UpdPlayerAnimation(3,false );
	
	GetWorldTimerManager().SetTimer(DefaultAnimationDelay, [this]()
			{
			C_UpdPlayerAnimation(1,false );
			},1.0f, false);
	
    if (!C_bQTEActive)
    {
        return;
    }
	
    GetWorldTimerManager().ClearTimer(C_QTENextRoundTimerHandle);
	
    UC_WBP_MainUI* MainUI = Cast<UC_WBP_MainUI>(MyWidgetInstance);
	
    if (MainUI)
    {
	    float baseScoreFromSuccesRound = 10.f;
	    MainUI->UpdateScoreFromCoin(baseScoreFromSuccesRound * C_QTERemainingTime);
    }
	
    C_bQTEActive = false;
	
    C_QTERemainingTime = 0.0;
	
    C_QTECurrentIndex = 0;
	
    C_QTESequence.Empty();
	
    if (QTEWidgetInstance)
    {
        QTEWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
    }
    BP_QTE_SetVisible(false);
	
    BP_QTE_ShowResult(true);
	
    if (IsValid(C_CurrentBatEnemy))
    {
        C_CurrentBatEnemy->C_CurrentHealth = FMath::Max(
            0.0,
            C_CurrentBatEnemy->C_CurrentHealth - C_CurrentBatEnemy->C_DamagePerSuccess
        );
        if (C_CurrentBatEnemy->C_CurrentHealth <= 0.0)
        {
            C_CurrentBatEnemy->Destroy();
        	
            C_CurrentBatEnemy = nullptr;
        	
            C_bInsideBatZone = false;
        	
            if (QTEWidgetInstance)
            {
                QTEWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
            }
            BP_QTE_SetVisible(false);
            return;
        }
    }

    if (C_bInsideBatZone && IsValid(C_CurrentBatEnemy))
    {
        GetWorldTimerManager().SetTimer(
            C_QTENextRoundTimerHandle,
            this,
            &ABP_C_Player::C_StartQTERound,
            1.0f,
            false
        );
    }
}

void ABP_C_Player::C_FaliRound()
{
	if (!C_bQTEActive)
	{
		return;
	}

	GetWorldTimerManager().ClearTimer(C_QTENextRoundTimerHandle);

	C_bQTEActive = false;
	
	C_QTERemainingTime = 0.0;
	
	C_QTECurrentIndex = 0;
	
	C_QTESequence.Empty();

	if (QTEWidgetInstance)
	{
		QTEWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
	}
	
	BP_QTE_SetVisible(false);
	
	BP_QTE_ShowResult(false);
	
	C_TakeDamageFromEnemy();

	const float RestartDelay = IsValid(C_CurrentBatEnemy)
		? static_cast<float>(C_CurrentBatEnemy->C_AttackOnFailDelay)
		: 1.0f;
	
	if (C_bInsideBatZone && IsValid(C_CurrentBatEnemy) && !C_bIsDead)
	{
		GetWorldTimerManager().SetTimer(
			C_QTENextRoundTimerHandle,
			this,
			&ABP_C_Player::C_StartQTERound,
			FMath::Max(RestartDelay, 1.0f),
			false
		);
	}
}

void ABP_C_Player::C_EnterBatZone(ABP_C_MainEnemy* BatEnemy)
{
	if (!IsValid(BatEnemy))
	{
		return;
	}

	C_CurrentBatEnemy = BatEnemy;
	
	C_bInsideBatZone = true;

	if (!C_bAutoStartQTEOnEnterZone)
	{
		return;
	}

	if (C_bQTEActive)
	{
		return;
	}

	if (GetWorldTimerManager().IsTimerActive(C_QTENextRoundTimerHandle))
	{
		return;
	}

	C_StartQTERound();
}

void ABP_C_Player::C_ExitBatZone(ABP_C_MainEnemy* BatEnemy)
{
	if (IsValid(BatEnemy) && BatEnemy != C_CurrentBatEnemy)
	{
		return;
	}
	
	GetWorldTimerManager().ClearTimer(C_QTENextRoundTimerHandle);
	
	C_bInsideBatZone = false;
	
	C_bQTEActive = false;
	
	C_CurrentBatEnemy = nullptr;
	
	C_QTESequence.Empty();
	
	C_QTECurrentIndex = 0;
	
	C_QTERemainingTime = 0.0;
	
	if (QTEWidgetInstance)
	{
		QTEWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
	}
	BP_QTE_SetVisible(false);
}

void ABP_C_Player::C_TakeDamageFromEnemy()
{
	if (C_bIsDead)
	{
		return;
	}
	C_UpdPlayerAnimation(4, false );
	
	GetWorldTimerManager().SetTimer(DefaultAnimationDelay, [this]()
			{
			C_UpdPlayerAnimation(1,false );
			},1.0f, false);
	
	ABP_C_MainEnemy* enemy = Cast<ABP_C_MainEnemy>(C_CurrentBatEnemy);
	
	if (!IsValid(enemy)) return;
	
	enemy->C_UpdEnemyAnimation(2);
	
	GetWorldTimerManager().SetTimer(EnemyAnimationDelay, [this]()
		{
			ABP_C_MainEnemy* eenemy = Cast<ABP_C_MainEnemy>(C_CurrentBatEnemy);
			if (!IsValid(eenemy)) return;
			eenemy->C_UpdEnemyAnimation(1);
		},1.33f, false);
	
	double Damage = IsValid(C_CurrentBatEnemy)
		? C_CurrentBatEnemy->C_DamageToPlayerOnFail
		: damage;
	C_CurrentHealth = C_CurrentHealth - Damage;
	
	C_CurrentHealth = FMath::Clamp(C_CurrentHealth, 0.0, C_MaxHealth);
	
	UC_WBP_MainUI* MainUI = Cast<UC_WBP_MainUI>(MyWidgetInstance);
	
	if (!IsValid(MainUI)) return;
	
	MainUI->UpdateHPFromPlayer(C_CurrentHealth/C_MaxHealth);
	
	if (C_CurrentHealth <= 0.0)
	{
		C_bIsDead = true;
		
		C_bQTEActive = false;
		
		if (QTEWidgetInstance)
		{
			QTEWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
		}
		BP_QTE_SetVisible(false);
	}
}

bool ABP_C_Player::C_IsQTEActive() const
{
	return C_bQTEActive;
}

void ABP_C_Player::C_HPBuff()
{
	if (!C_bIsDead)
	{
		C_CurrentHealth = C_CurrentHealth + heal;
		C_CurrentHealth = FMath::Clamp(C_CurrentHealth, 0.0f, C_MaxHealth);
	}
}

void ABP_C_Player::C_LavaDamage()
{
}

void ABP_C_Player::C_HealthBarSettings()
{
}

void ABP_C_Player::C_UpdPlayerHealth()
{
}

void ABP_C_Player::C_UpdPlayerStammina()
{
}

void ABP_C_Player::C_UpdPlayerAnimation(int numAnim, bool loopAnim)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	
	if (!AnimInstance) return;
	
	int32 Index = numAnim - 1;
	
	if (!Animations.IsValidIndex(Index) || !Animations[Index]) return;
	
	if (loopAnim && CurrentAnimIndex == Index) return;
	
	CurrentAnimIndex = Index;
	
	GetMesh()->PlayAnimation(Animations[Index], loopAnim);
}

void ABP_C_Player::LavaDamage_Implementation()
{
	if (C_bIsDead) return;
	
	C_CurrentHealth -= 200.f;
	
	C_CurrentHealth = FMath::Clamp(C_CurrentHealth, 0.0, C_MaxHealth); 
	
	if (C_CurrentHealth <= 0.0)
	{
		C_bIsDead = true;
		C_bQTEActive = false; 
	}
}
