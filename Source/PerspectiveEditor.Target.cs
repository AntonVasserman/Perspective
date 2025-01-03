// Copyright Anton Vasserman, All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class PerspectiveEditorTarget : TargetRules
{
	public PerspectiveEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_5;
		ExtraModuleNames.Add("Perspective");
	}
}
