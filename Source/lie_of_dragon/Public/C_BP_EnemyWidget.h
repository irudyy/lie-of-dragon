// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "C_BP_EnemyWidget.generated.h"

/**
 * 
 */
UCLASS()
class LIE_OF_DRAGON_API UC_BP_EnemyWidget : public UUserWidget
{
	GENERATED_BODY()
	
	
public:

	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> MyWidgetClass; // Сюда в редакторе выберете нужный виджет

	UPROPERTY()
	TObjectPtr<UUserWidget> MyWidgetInstance; // Ссылка на созданный экземпляр
};
