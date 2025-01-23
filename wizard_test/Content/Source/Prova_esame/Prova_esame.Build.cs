// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Prova_esame : ModuleRules
{
	public Prova_esame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
