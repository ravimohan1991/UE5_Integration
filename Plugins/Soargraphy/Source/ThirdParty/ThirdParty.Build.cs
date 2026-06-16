using System.IO;
using UnrealBuildTool;

public class ThirdParty : ModuleRules
{
    public ThirdParty(ReadOnlyTargetRules Target) : base(Target)
    {
        // Optional: define macro
        PublicDefinitions.Add("WITH_MYLIB=1");

        ShadowVariableWarningLevel = WarningLevel.Off; // there are several instances of local variable shadowing global

        // Enable RTTI for dynamic_cast
        bUseRTTI = true;
        
        // Also enable exceptions (often needed alongside RTTI)
        bEnableExceptions = true;

        // Include paths from library's original structure
        PublicIncludePaths.AddRange(
			new string[] {
                ModuleDirectory + "/Soar/Core/CLI/src",
				ModuleDirectory + "/Soar/Core/ClientSML/src",
                ModuleDirectory + "/Soar/Core/ConnectionSML/src",
                ModuleDirectory + "/Soar/Core/ElementXML/src",
                ModuleDirectory + "/Soar/Core/KernelSML/src",
                ModuleDirectory + "/Soar/Core/shared",
                ModuleDirectory + "/Soar/Core/SoarKernel/src/decision_process",
                ModuleDirectory + "/Soar/Core/SoarKernel/src/shared",
                ModuleDirectory + "/Soar/Core/SoarKernel/src/soar_representation",
                ModuleDirectory + "/Soar/Core/SoarKernel/src/explanation_memory",
                ModuleDirectory + "/Soar/Core/SoarKernel/src/explanation_based_chunking",
                ModuleDirectory + "/Soar/Core/SoarKernel/src/semantic_memory",
                ModuleDirectory + "/Soar/Core/SoarKernel/src/output_manager",
                ModuleDirectory + "/Soar/Core/SoarKernel/src/interface",
                ModuleDirectory + "/Soar/Core/SoarKernel/src/debug_code",
                ModuleDirectory + "/Soar/Core/SoarKernel/src/episodic_memory",
                ModuleDirectory + "/Soar/Core/SoarKernel/src/reinforcement_learning",
                ModuleDirectory + "/Soar/Core/SoarKernel/src/parsing",
                ModuleDirectory + "/Soar/Core/SoarKernel/src/visualizer",
                ModuleDirectory + "/Soar/Core/SVS/ccd",
                ModuleDirectory + "/Soar/Core/SVS/eigen",
                ModuleDirectory + "/Soar/Core/SVS/src",
                ModuleDirectory + "/Soar/Core/SVS/src/posix",// may nneed similar for windows/macos
                ModuleDirectory + "/asio/include",
				// ... add public include paths required here ...
			}
		);

        PublicDependencyModuleNames.AddRange(new string[] {
            "SQLiteCore",   // Provides the core SQLite C API
            "SQLiteSupport" // Provides Unreal-friendly wrappers
        });

        PrivateIncludePaths.AddRange(
            new string[] {
                Path.Combine(ModuleDirectory, "Soar/Core/CLI/src")
            }
        );

        if (Target.Platform == UnrealTargetPlatform.Linux)
        {
            // This is the UBT equivalent of passing "-ldl" to the Linux linker
            PublicSystemLibraries.Add("dl");

            // Library path
            //string LibPath = Path.Combine(ModuleDirectory, "Soar/lib/Linux");
           // PublicAdditionalLibraries.Add(Path.Combine(LibPath, "libSoar.so"));  // [web:26][web:33]

            // For .so that needs runtime loading:
           // RuntimeDependencies.Add(Path.Combine(LibPath, "libSoar.so"));
        }
    }
}
