// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ProjectRE : ModuleRules
{
	public ProjectRE(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate",
			"Niagara",
			"PhysicsCore",
			"NavigationSystem", 
			"GameplayTasks",
			"MotionWarping",
			"GeometryCollectionEngine",
			"FieldSystemEngine",
			"GameplayTags"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"ProjectRE",
			"ProjectRE/Variant_Platforming",
			"ProjectRE/Variant_Platforming/Animation",
			"ProjectRE/Variant_Combat",
			"ProjectRE/Variant_Combat/AI",
			"ProjectRE/Variant_Combat/Animation",
			"ProjectRE/Variant_Combat/Gameplay",
			"ProjectRE/Variant_Combat/Interfaces",
			"ProjectRE/Variant_Combat/UI",
			"ProjectRE/Variant_SideScrolling",
			"ProjectRE/Variant_SideScrolling/AI",
			"ProjectRE/Variant_SideScrolling/Gameplay",
			"ProjectRE/Variant_SideScrolling/Interfaces",
			"ProjectRE/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
