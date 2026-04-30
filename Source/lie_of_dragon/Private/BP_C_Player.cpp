// Fill out your copyright notice in the Description page of Project Settings.


#include "BP_C_Player.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GeometryCollection/GeometryCollectionDebugDrawActor.h"

// Sets default values
ABP_C_Player::ABP_C_Player()
{
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
	
if	(APlayerController* PlayerController = Cast<APlayerController>(GetController()))
{
	if (ULocalPlayer * LocalPlayer = PlayerController->GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
		{
			Subsystem->AddMappingContext(MappingContext, 0);
		}
	}
}
	
}

// Called every frame
void ABP_C_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABP_C_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if 	(UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent -> BindAction(JumpAction, ETriggerEvent::Triggered, this, &ABP_C_Player::PlayerJump);
		EnhancedInputComponent -> BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABP_C_Player::PlayerMove);
		EnhancedInputComponent -> BindAction(ShootAction, ETriggerEvent::Triggered, this, &ABP_C_Player::PlayerShoot);
		EnhancedInputComponent -> BindAction(LookAction, ETriggerEvent::Triggered, this, &ABP_C_Player::PlayerLook);
	}
	
}

void ABP_C_Player::PlayerJump()
{
	Jump();
}

void ABP_C_Player::PlayerMove(const FInputActionValue& ActionValue)
{
	FVector2D ActionVector = ActionValue.Get<FVector2D>();
	
	GetActorForwardVector().X;
	
	AddMovementInput(GetActorForwardVector(), ActionVector.Y);
	AddMovementInput(GetActorRightVector(), ActionVector.X);
	
	// GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Cyan, TEXT("MOVING!"));
}

void ABP_C_Player::PlayerShoot()
{
}

void ABP_C_Player::PlayerLook(const FInputActionValue& ActionValue)
{
	FVector2D ActionRotation = ActionValue.Get<FVector2D>();
	AddControllerYawInput(ActionRotation.X);
	AddControllerPitchInput(ActionRotation.Y);
}

