import os
import subprocess

class Compiler(Exception):
    def __init__(self, file_code, include_file = "", link_libraries = []):
        self.file_code = file_code
        self.include_file = include_file
        self.link_libraries = link_libraries

    def check_files(self):
        if self.file_code is None:
            raise Compiler(self.file_code + " isn't exist")
        if os.path.isfile(self.file_code) == False:
            raise Compiler(self.file_code + " isn't exist")
        if os.path.isfile(self.include_file) == False and self.include_file != "":
            raise Compiler(self.include_file + " isn't exist")
        return True

    def start(self):
        command_line = "g++ " + self.file_code + " -o " + "./../build/testOut"
        if self.include_file is not "":
            command_line = command_line + " -I" + self.include_file
        s = subprocess.check_call(command_line, shell=True) 
        return s
    def run(self):
        self.check_files()
        need_refine = self.start()
        return need_refine

a = Compiler("./../examples/main.cpp")
isCompile = a.run()
print(isCompile)