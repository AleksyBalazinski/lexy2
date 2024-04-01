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

def get_expected(dir):
    filepath = os.path.join(dir, "expected.txt")
    with open(filepath, "r") as file:
        return file.read()

def get_output(source_location, show=False):
    lexy2_process = subprocess.run(lexy2 + " " + source_location, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    lexy2_stderr = lexy2_process.stderr
    if(len(lexy2_stderr) > 0 and show):
         print(lexy2_stderr)
        
    clang_process = subprocess.run(clang + " " + source_location[:-3] + ".ll", stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    clang_stderr = clang_process.stderr
    if(len(clang_stderr and show) > 0):
        print(clang_stderr)
        
    exe_process = subprocess.run(exe, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    exe_stdout = exe_process.stdout
    if(show):
        print(exe_stdout)
    return exe_stdout


red_code = "\033[91m"
green_code = "\033[92m"
reset_code = "\033[0m"

for example_dir in os.listdir(examples_dir):
    cur_dir = os.path.join(examples_dir, example_dir)
    l2_files = find_files(cur_dir, "l2")
    out = get_output(l2_files[0])
    expected = get_expected(cur_dir)
    print(example_dir, end=" ")
    if(out.strip() == expected.strip()):
        print(green_code + "OK" + reset_code)
    else:
        print(red_code + "Failing" + reset_code, end=": ")
        print("expected '" + expected + "'")
        print("but got '" + out + "'")