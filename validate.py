import subprocess
import os

lexy2 = "./build/lexy2"
clang = "clang"
exe = "./a"
examples_dir = "./examples"

def find_files(dir, extension):
    files = []
    for file in os.listdir(dir):
        if file.endswith(extension):
            files.append(os.path.join(dir, file))
    return files

def get_output(source_location):
    try:
        print(source_location)
        lexy2_process = subprocess.run(lexy2 + " " + source_location, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
        lexy2_stederr = lexy2_process.stderr
        if(len(lexy2_stederr) > 0):
            print(lexy2_stederr)
        
        clang_process = subprocess.run(clang + " " + source_location[:-3] + ".ll", stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
        clang_stderr = clang_process.stderr
        if(len(clang_stderr) > 0):
            print(clang_stderr)
        
        exe_process = subprocess.run(exe, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
        exe_stdout = exe_process.stdout
        print(exe_stdout)

    except subprocess.CalledProcessError as e:
        print("Error:", e)

for example_dir in os.listdir(examples_dir):
    l2_files = find_files(os.path.join(examples_dir, example_dir), "l2")
    get_output(l2_files[0])
