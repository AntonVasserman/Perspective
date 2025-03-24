// Copyright Anton Vasserman, All Rights Reserved.

using UnrealBuildTool;

public class Perspective : ModuleRules
{
	public Perspective(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "AVUtilities", "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "MotionWarping" });
	}
}
