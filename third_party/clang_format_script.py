import subprocess

subprocess.run("clang-format -i ../*.cpp", shell=True)
subprocess.run("clang-format -i ../*.h", shell=True)

subprocess.run("clang-format -i ../isa/*.h", shell=True)