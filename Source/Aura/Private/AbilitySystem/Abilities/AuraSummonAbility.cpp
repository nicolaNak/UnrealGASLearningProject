// Copyright WetaWorkshop Ltd


#include "AbilitySystem/Abilities/AuraSummonAbility.h"

#include "Character/AuraEnemy.h"
#include "Kismet/KismetMathLibrary.h"


TArray<FVector> UAuraSummonAbility::GetSpawnLocations()
{
	const FVector Forward =  GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();

	const float DeltaSpread = SpawnSpread / NumMinions;

	//From source POV
	const FVector RightOfSpread = Forward.RotateAngleAxis(SpawnSpread/2.f,FVector::UpVector);
	const FVector LeftOfSpread = Forward.RotateAngleAxis(-SpawnSpread/2.f,FVector::UpVector);

	TArray<FVector> SpawnLocations;

	for (int32 i = 0; i < NumMinions; i++)
	{
		const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread*i + DeltaSpread/2.f, FVector::UpVector);
		FVector ChosenSpawnLocation = Location + Direction * FMath::FRandRange(MinSpawnDistance, MaxSpawnDistance);

		FHitResult Hit;
		GetWorld()->LineTraceSingleByChannel(Hit, ChosenSpawnLocation + FVector(0.f, 0.f, 400.f), ChosenSpawnLocation - FVector(0.f, 0.f, 400.f), ECC_Visibility);
		if (Hit.bBlockingHit)
		{
			ChosenSpawnLocation = Hit.ImpactPoint;
		}
		SpawnLocations.Add(ChosenSpawnLocation);
	}
	
	return SpawnLocations;
}

void UAuraSummonAbility::SpawnMinionsAtLocationsWithRandomisedDelay(const TArray<FVector> SpawnLocations, const float Delay)
{
	for (int32 i = 0; i < SpawnLocations.Num(); i++)
	{
		FTimerHandle TimerHandle;
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindWeakLambda(this,[&, SpawnLocations, i]
		{
			SpawnMinion(SpawnLocations[i]);
		});
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, Delay + Delay * i, false);
		Timers.Add(TimerHandle, TimerDelegate);
	}
}

TSubclassOf<APawn> UAuraSummonAbility::GetRandomMinionClass()
{
	int32 Selection = FMath::RandRange(0, MinionClasses.Num() - 1);
	return MinionClasses[Selection];
}

void UAuraSummonAbility::SpawnMinion(const FVector& Location)
{
	TSubclassOf<APawn> MinionClassToSpawn = GetRandomMinionClass();
	FVector AdjustedLocation = Location + FVector(0.f, 0.f, 35.f);
	FRotator FacingTargetRotation = UKismetMathLibrary::FindLookAtRotation(GetAvatarActorFromActorInfo()->GetActorLocation(), Location);
	FActorSpawnParameters SpawnParameters = FActorSpawnParameters();
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	AActor* SpawnedMinion = GetWorld()->SpawnActor(MinionClassToSpawn, &AdjustedLocation, &FacingTargetRotation, SpawnParameters);
	AAuraEnemy* MinionEnemy = Cast<AAuraEnemy>(SpawnedMinion);
	MinionEnemy->SpawnController();
	
	
	MinionSpawnedCount++;
	ICombatInterface* SourceInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	SourceInterface->Execute_SubscribeToMinionDeath(GetAvatarActorFromActorInfo(), SpawnedMinion);
	SourceInterface->Execute_ChangeMinionCount(GetAvatarActorFromActorInfo(), 1);
	if (MinionSpawnedCount >= NumMinions)
	{
		SummonComplete();
	}
}

void UAuraSummonAbility::SummonComplete()
{
	//As we store the pairs in a TMap we need to clear/invalidate them on completion before Emptying the map
	for (auto& TimerPair : Timers)
	{
		TimerPair.Key.Invalidate();
		TimerPair.Value.Unbind();
	}
	Timers.Empty();
	MinionSpawnsCompleteDelegate.Broadcast();
}
