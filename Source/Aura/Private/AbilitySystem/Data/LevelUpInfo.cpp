

#include "AbilitySystem/Data/LevelUpInfo.h"

int32 ULevelUpInfo::FindLevelForXP(int32 XP) const
{
	int32 Level = 1;
	bool bSearching = true;
	while (bSearching)
	{
		//Level up info at [1] == Level 1 information, [0] is meaningless
		if (LevelUpInfo.Num() - 1 <= Level) return Level;

		if (XP >= LevelUpInfo[Level].LevelUpRequirement)
		{
			Level++;
		}
		else
		{
			bSearching = false;
		}
	}
	return Level;
}
