import subprocess

subprocess.run("clang-format -i ../src/*.cpp", shell=True)
subprocess.run("clang-format -i ../include/*.h", shell=True)

subprocess.run("clang-format -i ../isa/*.h", shell=True)