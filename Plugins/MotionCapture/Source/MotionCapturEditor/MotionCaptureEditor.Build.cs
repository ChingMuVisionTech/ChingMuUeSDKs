// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

namespace UnrealBuildTool.Rules
{
	public class MotionCaptureEditor : ModuleRules
	{
		public MotionCaptureEditor(ReadOnlyTargetRules Target) : base(Target)
        {
            PrivatePCHHeaderFile = "Private/MotionCaptureEditorPCH.h";

            PrivateIncludePaths.AddRange(
                new string[] {
                    "MotionCapture/Private",
                    "MotionCapture/Public",
                    "MotionCapturEditor/Private",
                }
            );

            PublicDependencyModuleNames.AddRange(
				new string[]
				{
					"Core",
					"CoreUObject",
                    "UnrealEd",
                    "Engine",
                    "Projects",
                    "DetailCustomizations",

                    "WorkspaceMenuStructure",
                    "EditorStyle",
                    "SlateCore",
                    "Slate",
                    "InputCore",

                    //"Messaging",
                    //"LiveLinkInterface",
					//"LiveLinkMessageBusFramework",
                    "BlueprintGraph",
					//"LiveLink",
                    "MotionCapture",
                    "AnimGraph",
                    "Persona"
                }
			);

            PrivateDependencyModuleNames.AddRange(
                new string[]
                {
                    // ... add private dependencies that you statically link with here ...
                }
            );
        }
	}
}
