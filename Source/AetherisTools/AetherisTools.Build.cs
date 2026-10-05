// Copyright © 2026 AETHERIS. All rights reserved.

using UnrealBuildTool;

public class AetherisTools : ModuleRules
{
	public AetherisTools(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"AetherisCore",
			"AetherisSimulation",
			"AetherisGameplay",
			"UMG",
			"Slate",
			"SlateCore"
		});
	
		PrivateDependencyModuleNames.AddRange(new string[] { });
	}
}
