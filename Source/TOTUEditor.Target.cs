// Copyright xTear Studios
/*-------------------------------------------------------------------------*/
using UnrealBuildTool;
using System.Collections.Generic;

public class TOTUEditorTarget : TargetRules
{
	public TOTUEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V6;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_7;
		
		ExtraModuleNames.AddRange( new string[] { "TOTU" } );
	}
}
/*-------------------------------------------------------------------------*/