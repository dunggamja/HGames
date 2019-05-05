// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.
using System;
using System.IO;
using UnrealBuildTool;

public class Client : ModuleRules
{
    private string ModulePath
    {
        get { return ModuleDirectory; }
    }

    private string ThirdPartyPath
    {
        get { return Path.GetFullPath(Path.Combine(ModulePath, "../../ThirdParty/")); }
    }


    public Client(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
              "Core"
            , "CoreUObject"
            , "Engine"
            , "InputCore"
            , "UMG"
            , "HeadMountedDisplay"
            , "GameplayTasks"
        });

        PrivateDependencyModuleNames.AddRange(new string[] { });

        if ((Target.Platform == UnrealTargetPlatform.Win64) || (Target.Platform == UnrealTargetPlatform.Win32))
        {

            string ThirdPartyPath = Path.GetFullPath(Path.Combine(ModuleDirectory, "../../ThirdParty/"));
            string IncludePath = ThirdPartyPath + "Collision/include/";
            string LibraryName = "Physics.lib";
            string LibraryPath = ThirdPartyPath + "Collision/";

            if (Target.Platform == UnrealTargetPlatform.Win64)
                LibraryPath += "bin_x64/";
            else
                LibraryPath += "bin/";

            PublicLibraryPaths.Add(LibraryPath);
            PublicIncludePaths.Add(IncludePath);
            PublicAdditionalLibraries.Add(Path.Combine(LibraryPath, LibraryName));
        }
    }
}
