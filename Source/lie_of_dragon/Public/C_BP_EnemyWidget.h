#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "C_BP_EnemyWidget.generated.h"

UCLASS()
class LIE_OF_DRAGON_API UC_BP_EnemyWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category="Enemy UI")
	void UpdateHealth(float Percent);
};