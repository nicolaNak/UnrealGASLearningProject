// Copyright WetaWorkshop Ltd


#include "Input/AuraInputConfig.h"

const UInputAction* UAuraInputConfig::FindAbilityInputAction(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	for (const FAuraInputAction& Action : AbilityInputActions)
	{
		if (Action.InputAction && Action.InputTag.IsValid() && InputTag == Action.InputTag)
		{
			return Action.InputAction;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find ability action for input tag [%s], on input config [%s]"), *InputTag.ToString(), *GetNameSafe(this));
	}
	return nullptr;
}
