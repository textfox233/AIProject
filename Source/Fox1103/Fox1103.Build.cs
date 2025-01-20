// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Fox1103 : ModuleRules
{
	public Fox1103(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
