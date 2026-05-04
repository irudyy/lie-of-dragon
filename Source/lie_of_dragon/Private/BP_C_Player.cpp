// Fill out your copyright notice in the Description page of Project Settings.



/*
// Basic message
UE_LOG(LogTemp, Warning, TEXT("Hello from C++!"));

// Message with variables (using FString::Printf style formatting)
FString PlayerName = "John";
int32 Score = 100;
UE_LOG(LogTemp, Display, TEXT("Player %s has a score of: %d"), *PlayerName, Score);











*/

#include "BP_C_Player.h"
#include "BP_C_MainEnemy.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputCoreTypes.h"
#include "GameFramework/GameSession.h"
#include "GeometryCollection/GeometryCollectionDebugDrawActor.h"
//#include "Blueprint/UserWidget.h"
#include "TimerManager.h"
#include "C_WBP_MainUI.h"
#include "Components/CapsuleComponent.h"


// Sets default values
ABP_C_Player::ABP_C_Player()
{
	GetCapsuleComponent()->InitCapsuleSize(99.f, 99.0f);
	
	
	
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArm->SetupAttachment(GetRootComponent());
	
	PlayerCamera = CreateDefaultSubobject<UCameraComponent>("Camera");
	PlayerCamera->SetupAttachment(SpringArm);
	
	GetMesh()->SetRelativeRotation(FRotator {0.f, -90.f, 0.f});
	

}

// Called when the game starts or when spawned
void ABP_C_Player::BeginPlay()
{
	Super::BeginPlay();

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

// Called every frame

//dobavil
void ABP_C_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (C_bQTEActive)
	{
		C_QTERemainingTime -= DeltaTime;

		if (C_QTERemainingTime <= 0.0)
		{
			C_QTERemainingTime = 0.0;
			C_FaliRound();
		}
	}

	TickCounter += 1;

	if (TickCounter == 100)
	{
		TickCounter = 0;
		PlayerSecond();
	}
}
//dobavil

// Called to bind functionality to input
void ABP_C_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if 	(UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent -> BindAction(JumpAction, ETriggerEvent::Triggered, this, &ABP_C_Player::PlayerJump);
		EnhancedInputComponent -> BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABP_C_Player::PlayerMove);
		//EnhancedInputComponent -> BindAction(DashAction, ETriggerEvent::Triggered, this, &ABP_C_Player::PlayerDash);
		//EnhancedInputComponent -> BindAction(LookAction, ETriggerEvent::Triggered, this, &ABP_C_Player::PlayerLook);
		
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Started, this, &ABP_C_Player::PlayerStartDash);
		//EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Canceled, this, &ABP_C_Player::PlayerStopDash);
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Completed, this, &ABP_C_Player::PlayerEndDash);
		
		//dobavil
		if (QTEUpAction)
		{
			EnhancedInputComponent->BindAction(QTEUpAction, ETriggerEvent::Started, this, &ABP_C_Player::QTEInputUp);
		}

		if (QTEDownAction)
		{
			EnhancedInputComponent->BindAction(QTEDownAction, ETriggerEvent::Started, this, &ABP_C_Player::QTEInputDown);
		}

		if (QTELeftAction)
		{
			EnhancedInputComponent->BindAction(QTELeftAction, ETriggerEvent::Started, this, &ABP_C_Player::QTEInputLeft);
		}

		if (QTERightAction)
		{
			EnhancedInputComponent->BindAction(QTERightAction, ETriggerEvent::Started, this, &ABP_C_Player::QTEInputRight);
		}
		//dobavil
	}
	
}

void ABP_C_Player::PlayerJump()
{
	Jump();
}

//izmenil
void ABP_C_Player::PlayerMove(const FInputActionValue& ActionValue)
{
	if (C_bBlockMovementDuringQTE && C_bQTEActive)
	{
		return;
	}

	FVector2D ActionVector = ActionValue.Get<FVector2D>();

	AddMovementInput(GetActorForwardVector(), ActionVector.Y);
	AddMovementInput(GetActorRightVector(), ActionVector.X);
	
	// GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Cyan, TEXT("MOVING!"));
}
//izmenil

void ABP_C_Player::PlayerStartDash(const FInputActionValue& ActionValue)
{
	
	if (StamminaProcent <= 0)
	{
		GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = 1500.0f;
		
	}
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("SPRINT ON"));
	}
}
void ABP_C_Player::PlayerEndDash(const FInputActionValue& ActionValue)
{
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("SPRINT OFF"));
	}
}

//dobavil
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
//dobavil


/*void ABP_C_Player::PlayerLook(const FInputActionValue& ActionValue)
{
	FVector2D ActionRotation = ActionValue.Get<FVector2D>();
	AddControllerYawInput(ActionRotation.X);
	AddControllerPitchInput(ActionRotation.Y);
}
*/

void ABP_C_Player::PlayerSecond()
{
	if (StamminaProcent <= 0)
	{
		GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	}
	if (GetCharacterMovement()->MaxWalkSpeed == 1500.0f && StamminaProcent != 0)
	{
		StamminaProcent = StamminaProcent - 40;
	}
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT("Second"));
	}
	if (StamminaProcent >= 100)
	{
	}
	else
	{
		StamminaProcent = StamminaProcent + 20;
	}
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString::SanitizeFloat(StamminaProcent));
}

//dobavil i izmenil
void ABP_C_Player::C_StartQTERound()
{
	if (!C_bInsideBatZone)
	{
		return;
	}

	if (!IsValid(C_CurrentBatEnemy))
	{
		C_bQTEActive = false;
		return;
	}

	if (C_QTESequenceLength <= 0)
	{
		C_QTESequenceLength = 4;
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

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Cyan, TEXT("QTE STARTED"));
	}
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

	BP_QTE_HighlightArrow(C_QTECurrentIndex);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.75f, FColor::Green, TEXT("QTE CORRECT INPUT"));
	}
}

void ABP_C_Player::C_SuccessRound()
{
	if (!C_bQTEActive)
	{
		return;
	}

	C_bQTEActive = false;

	BP_QTE_ShowResult(true);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("QTE SUCCESS"));
	}

	if (IsValid(C_CurrentBatEnemy))
	{
		C_CurrentBatEnemy->C_CurrentHealth = FMath::Max(
			0.0,
			C_CurrentBatEnemy->C_CurrentHealth - C_CurrentBatEnemy->C_DamagePerSuccess
		);

		if (C_CurrentBatEnemy->C_CurrentHealth <= 0.0)
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Green, TEXT("BAT DEAD"));
			}

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
			0.35f,
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

	C_bQTEActive = false;

	BP_QTE_ShowResult(false);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("QTE FAIL"));
	}

	C_TakeDamageFromEnemy();

	const float RestartDelay = IsValid(C_CurrentBatEnemy)
		? static_cast<float>(C_CurrentBatEnemy->C_AttackOnFailDelay)
		: 0.35f;

	if (C_bInsideBatZone && IsValid(C_CurrentBatEnemy) && !C_bIsDead)
	{
		GetWorldTimerManager().SetTimer(
			C_QTENextRoundTimerHandle,
			this,
			&ABP_C_Player::C_StartQTERound,
			FMath::Max(RestartDelay, 0.2f),
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

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Cyan, TEXT("ENTER BAT ZONE"));
	}

	if (C_bAutoStartQTEOnEnterZone && !C_bQTEActive)
	{
		C_StartQTERound();
	}
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

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT("EXIT BAT ZONE"));
	}
}

void ABP_C_Player::C_TakeDamageFromEnemy()
{
	if (C_bIsDead)
	{
		return;
	}

	const double Damage = IsValid(C_CurrentBatEnemy)
		? C_CurrentBatEnemy->C_DamageToPlayerOnFail
		: damage;

	C_CurrentHealth = C_CurrentHealth - Damage;
	C_CurrentHealth = FMath::Clamp(C_CurrentHealth, 0.0, C_MaxHealth);

	if (GEngine)
	{
		const FString Message = FString::Printf(TEXT("PLAYER HP: %.0f"), C_CurrentHealth);
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, Message);
	}

	if (C_CurrentHealth <= 0.0)
	{
		C_bIsDead = true;
		C_bQTEActive = false;

		if (QTEWidgetInstance)
		{
			QTEWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
		}

		BP_QTE_SetVisible(false);

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("PLAYER DEAD"));
		}
	}
}

bool ABP_C_Player::C_IsQTEActive() const
{
	return C_bQTEActive;
}
//dobavil i izmenil

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


// IVAN UPDATE FOR LAVA===============================
void ABP_C_Player::LavaDamage_Implementation()
{
	if (C_bIsDead) return;

	C_CurrentHealth -= 200.f;
	C_CurrentHealth = FMath::Clamp(C_CurrentHealth, 0.0, C_MaxHealth); // ======= MAYBEE DELETEEE=====

	if (GEngine)
	{
		const FString Message = FString::Printf(TEXT("LAVA DAMAGE! HP: %.0f"), C_CurrentHealth);
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Orange, Message);
	}

	if (C_CurrentHealth <= 0.0)
	{
		C_bIsDead = true;
		C_bQTEActive = false; // ======= MAYBEE DELETEEE=====

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("PLAYER DEAD FROM LAVA"));
			UE_LOG(LogTemp, Warning, TEXT("PLAYER DEAD FROM LAVA"));
		}
	}
}
