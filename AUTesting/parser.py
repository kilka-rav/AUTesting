import os
import re

class Parser(Exception):
    def __init__(self, path_to_file):
        self.path = path_to_file
        self.functions = []
    
    def isExist(self):
        return os.path.isfile(self.path)

    def get_body(self, declarations, file):
        print(declarations)
        lines = file.split('\n')
        for line in lines:
            for dec in declarations:
                if dec in line and len(dec) > 0:
                    print("find: ", dec)
                    

    def find_function(self):
        pattern_cpp =  r'\b(?:\w+\s+)*(?:\w+\s+)*\w+(?:::\w+)?\s+\w+\s*\([^)]*\)\s*[^;]\{?'
        with open(self.path, "r") as file:
            data = " ".join(file.readlines())
            a = re.findall(pattern_cpp, data)
            self.get_body(a, data)

    def run(self):
        if not self.isExist():
            raise Parser(self.path + " isn't exist")
        self.find_function()
