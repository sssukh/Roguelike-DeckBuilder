
using UnrealBuildTool;
using System.Collections.Generic;

public class CrownOfSinTarget : TargetRules
{
	public CrownOfSinTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;

		ExtraModuleNames.AddRange( new string[] { "CrownOfSin" } );
	}
}
