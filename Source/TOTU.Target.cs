// Copyright xTear Studios
/*-------------------------------------------------------------------------*/
using UnrealBuildTool;
using System.Collections.Generic;

public class TOTUTarget : TargetRules
{
	public TOTUTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V4;

		ExtraModuleNames.AddRange( new string[] { "TOTU" } );
	}
}
/*-------------------------------------------------------------------------*/