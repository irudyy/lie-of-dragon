// Fill out your copyright notice in the Description page of Project Settings.

#include "C_BP_Princess.h"

// Sets default values
AC_BP_Princess::AC_BP_Princess()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AC_BP_Princess::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AC_BP_Princess::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AC_BP_Princess::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

