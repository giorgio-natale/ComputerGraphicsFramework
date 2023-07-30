import os
import subprocess

def compile_shader(subdirectory, file_info, target):
    result = subprocess.run(["glslc.exe", f"{os.path.join(subdirectory, file_info[0])}{file_info[1]}", "-o", f"{os.path.join(target, file_info[0])}.spv"], stdout=subprocess.PIPE)
    print(result.stdout.decode("utf-8"))

def get_extension_and_name(file_name):
    return (os.path.splitext(os.path.basename(file_name)))


def compile_files_in_dir(subdirectory, target):
    for f in os.listdir(os.path.join("", subdirectory)):
        file_info = get_extension_and_name(f)
        if file_info[1] == ".frag" or file_info[1] == ".vert":
            compile_shader(subdirectory, file_info, target)


subdirectories = ["source/vertex", "source/fragment"]

for subdirectory in subdirectories:
    compile_files_in_dir(subdirectory, "compiled")
    