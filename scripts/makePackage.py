import os, sys, datetime, glob, shutil

from distutils.dir_util import copy_tree

platform_folders = ["x64"]
package_folders = ["DEV", "PROD_DEBUG", "PROD_PROFILE","PROD"]
debug_packages = ["DEV", "PROD_DEBUG"]
resource_folders = ["Tetarango/res"]
resource_exclude_files = [".xcf"]
dll_folders = ["Tetarango"]

version_file_location = "Tetarango/src/config/GameConfig.h"
package_prefix = "TETARANGO_"
output_folder = "packages"

def getAppVersion():
    with open(version_file_location, "r") as f:
        for line in f.readlines():
            if "Param::version" in line:
                return line.split("\"")[1]

def main():
    if len(sys.argv) == 1:
        print("Expected useage: python makePackage.py <root-directory>")
        return

    version = getAppVersion()
    if version is None or len(version) == 0:
        print("Could not find version file on path=" + version_file_location)
        return

    opDir = (os.getcwd() + "\\" + sys.argv[1]).replace("\\", "/").replace("/.", "/")
    print("Making packages base dir=" + os.getcwd() + "\\" + sys.argv[1])

    packageFolder = opDir + output_folder
    if not os.path.exists(packageFolder):
        os.mkdir(packageFolder)

    versionRootDir = packageFolder + "/" + package_prefix + version + datetime.datetime.now().strftime("_%m-%d-%y-%H.%M.%S")
    print("Making package for version=" + version)

    if not os.path.exists(versionRootDir):
        os.mkdir(versionRootDir)

    for platform in platform_folders:
        platformFolder = opDir + platform
        if os.path.exists(platformFolder):
            for package in package_folders:
                platformBuildFolder = platformFolder + "/" + package
                if os.path.exists(platformBuildFolder):
                    print("  Combining package for build=" + package)
                    packageRootDir = versionRootDir + "/" + package
                    os.mkdir(packageRootDir)

                    if package in debug_packages:
                        print("    Packing resource folder")
                        for resource in resource_folders:
                            copy_tree(opDir + resource, packageRootDir + "/res")
                    else:
                        print("    Packing resource bundle")
                        for file in glob.glob(r'*.pack'):
                            shutil.copy(file, packageRootDir)

                    print("    Packing SDL/GLEW dlls")
                    for dll in dll_folders:
                        for file in glob.glob(opDir + dll +  r'/*.dll'):
                            shutil.copy(file, packageRootDir)

                    print("    Packing executable")
                    for file in glob.glob(platformBuildFolder + r'/*.exe'):
                        shutil.copy(file, packageRootDir)

if __name__ == "__main__":
    main()
