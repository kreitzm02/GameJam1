// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GameJam1 : ModuleRules
{
	public GameJam1(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
