import os
import re

class Parser(Exception):
    def __init__(self, path_to_file):
        self.path = path_to_file
        self.functions = []
    
    def isExist(self):
        return os.path.isfile(self.path)

    def find_function(self):
        pattern = r'\b[a-zA-Z_]\w*\s+[a-zA-Z_]\w*\s*\([^)]*\)\s*\{[^}]*\}'
       
        with open(self.path, "r") as file:
            data = " ".join(file.readlines())
            a = re.findall(pattern, data)
            self.functions = a

    def run(self):
        if not self.isExist():
            raise Parser(self.path + " isn't exist")
        self.find_function()
        print(self.functions)
a = Parser("./../examples/simple.cpp")
a.run()