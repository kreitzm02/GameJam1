// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class GameJam1 : ModuleRules
{
	public GameJam1(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
    
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "AIModule", "NavigationSystem", "Niagara" });

        PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Enemies"));
        PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Enemies", "FSM"));
    }
}
