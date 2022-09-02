// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MJ_Project : ModuleRules
{
	public MJ_Project(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay","AIModule", "GameplayTasks","NavigationSystem", "Niagara", "UMG"});
	}
}
