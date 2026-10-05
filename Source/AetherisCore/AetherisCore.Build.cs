// Copyright © 2026 AETHERIS. All rights reserved.

using UnrealBuildTool;

public class AetherisCore : ModuleRules
{
	public AetherisCore(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore"
		});
	
		PrivateDependencyModuleNames.AddRange(new string[] { });
	}
}
