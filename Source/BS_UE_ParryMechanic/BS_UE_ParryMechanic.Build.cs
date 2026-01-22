// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class BS_UE_ParryMechanic : ModuleRules
{
	public BS_UE_ParryMechanic(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
		
		PrivateIncludePaths.AddRange(new string[] {"BS_UE_ParryMechanic/private"});
	}
}
