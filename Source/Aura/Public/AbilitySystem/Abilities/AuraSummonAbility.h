// Copyright WetaWorkshop Ltd

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "AuraSummonAbility.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMinionSpawnsComplete);

UCLASS()
class AURA_API UAuraSummonAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()

	public:

	UFUNCTION(BlueprintCallable)
	TArray<FVector> GetSpawnLocations();

	UFUNCTION(BlueprintCallable)
	void SpawnMinionsAtLocationsWithRandomisedDelay(const TArray<FVector> SpawnLocations,  const float Delay);

	UPROPERTY(BlueprintAssignable)
	FMinionSpawnsComplete MinionSpawnsCompleteDelegate;

	UFUNCTION(BlueprintPure, Category="Summoning")
	TSubclassOf<APawn> GetRandomMinionClass();
	
	UPROPERTY(EditAnywhere, Category="Summoning")
	int32 NumMinions{5};

	UPROPERTY(EditDefaultsOnly, Category="Summoning")
	TArray<TSubclassOf<APawn>> MinionClasses;

	UPROPERTY(EditDefaultsOnly, Category="Summoning")
	float MinSpawnDistance{150.f};

	UPROPERTY(EditDefaultsOnly, Category="Summoning")
	float MaxSpawnDistance{400.f};

	UPROPERTY(EditDefaultsOnly, Category="Summoning")
	float SpawnSpread{90.f};

protected:

	void SpawnMinion(const FVector& Location);

	int32 MinionSpawnedCount{0};

	TMap<FTimerHandle, FTimerDelegate> Timers;

	void SummonComplete();
	
};
