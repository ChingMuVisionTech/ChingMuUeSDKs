// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

using Path = System.IO.Path;
using UnrealBuildTool;

namespace UnrealBuildTool.Rules
{
	public class MotionCapture : ModuleRules
	{
		public MotionCapture(ReadOnlyTargetRules Target) : base(Target)
        {
            PrivatePCHHeaderFile = "Private/MotionCapturePrivatePCH.h";

            PublicIncludePaths.AddRange(
                new string[] {
                    "Runtime/HeadMountedDisplay/Public",
                    "Runtime/HeadMountedDisplay/Private",
                    "../Intermediate/Build/Win64/UE4Editor/Inc/SequenceRecorder",
                }
            );

            PrivateIncludePaths.AddRange(
				new string[] {
                    "MotionCapture/Public",
                    "MotionCapture/Private",
                    "MotionCapture/Model",
                    "MotionCapture/Interface",
                    "MotionCapture/Interface/ChingmuHumanCap",
                    "MotionCapture/Interface/ChingmuIQFace",
                    //"MotionCapture/Interface/SpringVRHandCap",
                }
            );

			PublicDependencyModuleNames.AddRange(
				new string[]
				{
					"Core",
					"CoreUObject",
					"InputCore",
					"Engine",
                    "InputCore",
                    "Renderer",
                    "RenderCore",
                    "Sockets",
                    "Networking",
                    "Projects",
                   // "MotionCapture",
                    "HeadMountedDisplay",
                }
		    );

			PrivateDependencyModuleNames.AddRange(
				new string[]
				{
					// ... add private dependencies that you statically link with here ...
				}
			);

			DynamicallyLoadedModuleNames.AddRange(
				new string[]
				{
					// ... add any modules that your module loads dynamically here ...
				}
			);
				
			// add MotionCapture libraries
            string BaseDirectory = Path.GetFullPath(Path.Combine(ModuleDirectory, "..", "..", ".."));
            string MotionCaptureDirectory = Path.Combine(BaseDirectory, "MotionCapture", "ThirdParty", Target.Platform.ToString());

             if (Target.Platform == UnrealTargetPlatform.Win32 || Target.Platform == UnrealTargetPlatform.Win64)
             {
                 RuntimeDependencies.Add(Path.Combine(MotionCaptureDirectory, "CMVrpn.dll"));//CMUnity//Developer
             }
		}
	}
}