
#pragma once

#include <Atomic/Core/Object.h>

using namespace Atomic;

namespace ToolCore
{

// the tool environment contains paths for various
// binaries, data paths, example folder, etc

// it is either built from the cli, AtomicEditor, environment variables,
// or a json config file, this avoids needing to symlink folders, etc

class ToolEnvironment : public Object
{
    OBJECT(ToolEnvironment)

public:

    ToolEnvironment(Context* context);
    virtual ~ToolEnvironment();

    // dev build init env from json
    bool InitFromJSON();

    void SetRootSourceDir(const String& sourceDir);
    void SetRootBuildDir(const String& buildDir, bool setBinaryPaths = false);

    const String& GetRootSourceDir() { return rootSourceDir_; }
    const String& GetRootBuildDir() { return rootBuildDir_; }

    const String& GetEditorBinary() { return editorBinary_; }
    const String& GetPlayerBinary() { return playerBinary_; }
    const String& GetToolBinary() { return toolBinary_; }

    const String& GetCoreDataDir() { return resourceCoreDataDir_; }
    const String& GetPlayerDataDir() { return resourcePlayerDataDir_; }
    const String& GetEditorDataDir() { return resourceEditorDataDir_; }

    const String& GetDeploymentDataDir() { return toolBinary_; }

    const String& GetMacPlayerDeploymentBinary();

    const String& GetExamplesDir() { return examplesDir_; }

    const String& GetDevConfigFilename();

private:

    // root source directory (for development builds)
    String rootSourceDir_;

    // root build directory (for development builds)
    String rootBuildDir_;

    // path to the Atomic Editor binary
    String editorBinary_;

    // path to Atomic player binary used when running content from the editor or cli
    String playerBinary_;

    // path to the AtomicTool command line binary
    String toolBinary_;

    // examples directory
    String examplesDir_;

    // resources
    String resourceCoreDataDir_;
    String resourcePlayerDataDir_;
    String resourceEditorDataDir_;

    // deployment

    // static deployment data directory
    String deploymentDataDir_;

    // whether to use individual build folders, or the deployment data dir for binaries
    bool useBuildDirs_;

    String macBuildDir_;
    String windowsBuildDir_;
    String linuxBuildDir_;

    String androidBuildDir_;
    String iosBuildDir_;
    String webBuildDir_;

    String devConfigFilename_;
};

}
