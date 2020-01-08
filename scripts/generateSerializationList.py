import os, sys, datetime, glob, shutil, io, subprocess

from distutils.dir_util import copy_tree

resource_folders = ["Tetarango/src"]
resource_include_files = [".cpp"]
namespaces_to_use = ["AWGame"]

output_file = "Tetarango/serialization-list.generated.h"
output_file_template = """#pragma warning(push)
#pragma warning(disable : 4834)
#pragma once

// GENERATED FILE - CHANGES WILL BE LOST

{{INCLUDES}}

{{NAMESPACES}}
namespace
{
    bool hasPrimed = false;
}

namespace AW
{
    class GeneratedPrimeList
    {
        friend class Application;
        void prime()
        {
            if (hasPrimed) return;
            hasPrimed = true;

{{MAKE_SHARED}}
        }
    };
}
#pragma warning(pop)
"""
include_template = "#include \"{{SOURCE_FILE}}\""
namespace_template = "using namespace {{NAMESPACE}};"
make_shared_template = "            std::make_unique<{{TYPE}}>();"
make_shared_load_template = "            std::make_unique<{{TYPE}}>()->onLoadResources();"

def main():
    if len(sys.argv) == 1:
        print("Expected useage: python generateSerializationList.py <root-directory>")
        return

    opDir = (os.getcwd() + "\\" + sys.argv[1]).replace("\\", "/").replace("/.", "/")
    print("Making serialization list base dir=" + os.getcwd() + "\\" + sys.argv[1])

    resourceFiles = []
    for root, dirs, files in os.walk(opDir + resource_folders[0]):
        for f in files:
            if f[-4:] in resource_include_files or f[-5:] in resource_include_files:
                resourceFiles.append((root + "/" + f).replace("\\", "/"))

    if os.path.exists(output_file):
        subprocess.check_output(["rm", output_file])

    objectsToAdd = []
    for rf in resourceFiles:
        hasResources = False;
        hasInserted = False
        for line in open(rf):
            if "::onLoadResources" in line:
                if hasInserted:
                    objectsToAdd[-1][2] = True
                hasResources = True
            if "GORegister(" in line:
                hasInserted = True
                pathInfo = rf.split("src/game") if "src/game" in rf else rf.split("src/core")
                objectsToAdd.append([pathInfo[1][1:].replace(".cpp", ".h"), line.split("(")[1].split(")")[0], hasResources])

    namespaces = []
    for namespace in namespaces_to_use:
        namespaces.append(namespace_template.replace("{{NAMESPACE}}", namespace))

    includes = []
    make_shareds = []
    for obj in objectsToAdd:
        includes.append(include_template.replace("{{SOURCE_FILE}}", obj[0]))
        make_shareds.append((make_shared_load_template if obj[2] else make_shared_template).replace("{{TYPE}}", obj[1].strip()))

    source = output_file_template.replace("{{INCLUDES}}", "\n".join(includes)).replace("{{NAMESPACES}}", "\n".join(namespaces)).replace("{{MAKE_SHARED}}", "\n".join(make_shareds))

    with open(output_file, "w") as o:
        o.write(source)

    print("Done making serialization list" )


if __name__ == "__main__":
    main()
