// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class VisualStudioTest : ModuleRules
{
	public VisualStudioTest(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "UMG" });
	}
}
