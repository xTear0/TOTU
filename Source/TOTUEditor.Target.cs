// Copyright xTear Studios
/*-------------------------------------------------------------------------*/
using UnrealBuildTool;
using System.Collections.Generic;

public class TOTUEditorTarget : TargetRules
{
	public TOTUEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V4;

		ExtraModuleNames.AddRange( new string[] { "TOTU" } );
	}
}
/*-------------------------------------------------------------------------*/