// Fill out your copyright notice in the Description page of Project Settings.


#include "C_BP_LAVA.h"
#include "BP_C_Player.h"


// Sets default values
AC_BP_LAVA::AC_BP_LAVA()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// Set size for collision capsule
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	RootComponent = BoxComp;
	BoxComp->SetBoxExtent(FVector(1000.f, 1000.f, 100.f));
	
	BoxComp->SetGenerateOverlapEvents(true);
	BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxComp->SetCollisionResponseToAllChannels(ECR_Overlap);
	

}

// Called when the game starts or when spawned
void AC_BP_LAVA::BeginPlay()
{
	Super::BeginPlay();
	
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AC_BP_LAVA::OnOverlapBegin);
    
}

// Called every frame
void AC_BP_LAVA::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AC_BP_LAVA::OnOverlapBegin(UPrimitiveComponent* OverlappedComp,AActor* OtherActor,UPrimitiveComponent* OtherComp,int32 OtherBodyIndex,bool bFromSweep,const FHitResult& SweepResult)
{
	ABP_C_Player* Player = Cast<ABP_C_Player>(OtherActor);
	if (!IsValid(Player)) return;
	Player->LavaDamage(); 
	
}

