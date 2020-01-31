import os, sys, datetime, glob, shutil, io, subprocess

from distutils.dir_util import copy_tree

resource_folders = ["Tetarango/res"]
resource_include_files = [".png", ".wav", ".ttf", ".mp3", ".glsl", ".lua"]

output_file = "asset.pack"

def main():
    if len(sys.argv) == 1:
        print("Expected useage: python makeAssetPackage.py <root-directory>")
        return

    opDir = (os.getcwd() + "\\" + sys.argv[1]).replace("\\", "/").replace("/.", "/")
    print("Making resource bundle base dir=" + os.getcwd() + "\\" + sys.argv[1])

    resourceFiles = []
    for root, dirs, files in os.walk(opDir + resource_folders[0]):
        for f in files:
            if f[-4:] in resource_include_files or f[-5:] in resource_include_files:
                resourceFiles.append((root + "/" + f).replace("\\", "/"))

    if os.path.exists(opDir + output_file):
        subprocess.check_output(["rm", output_file])

    print("Making resource bundle")
    for rf in resourceFiles:
        print("  Packing resource file on path=" + rf)

    subprocess.check_output(["scripts/AssetPacker.exe"] + resourceFiles + [output_file])


if __name__ == "__main__":
    main()
