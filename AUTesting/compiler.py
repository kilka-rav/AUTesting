import os
import subprocess
import logging
import shlex


class Compiler(Exception):
    def __init__(self, file_code, include_file="", link_libraries=[], using_compiler = "gcc"):
        self.file_code = file_code
        self.include_file = include_file
        self.link_libraries = link_libraries
        self.using_compiler = using_compiler

    def check_files(self):
        if self.file_code is None:
            raise Compiler(self.file_code + " isn't exist")
        if os.path.isfile(self.file_code) == False:
            raise Compiler(self.file_code + " isn't exist")
        if os.path.isfile(self.include_file) == False and self.include_file != "":
            raise Compiler(self.include_file + " isn't exist")
        return True

    def start(self, srcs: str, out_file: str):
        command_line = (
            self.using_compiler + " "
            + self.file_code
            + f" {srcs} "
            + " -fprofile-arcs -ftest-coverage " # for code coverage
            + " -o "
            + out_file
        )
        if self.include_file is not "":
            command_line += " -I ./ -I " + self.include_file
        command_line = shlex.split(command_line)
        logging.info(f"Compile: {command_line}")
        s = subprocess.run(command_line, capture_output=True, text=True)
        return s

    def run(self, srcs: str, out_file):
        self.check_files()
        need_refine = self.start(srcs, out_file)
        return need_refine


def fixErrors(code: str):
    # add include assert.h
    code = "\n#include <assert.h>\n" + code
    code = "\n#include <stdbool.h>\n" + code
    code = "\n#include <string.h>\n" + code
    code = '\n#include "./examples/RBTree/RBTree.h"\n' + code
    code = code.replace('"RBTree.h"', '"./examples/RBTree/RBTree.h"')
    code = code.replace('"Operation.h"', '"./examples/operation/Operation.h"')
    code = code.replace("<cassert>", "<assert.h>")
    code = code.replace("<cstdlib>", "<stdlib.h>")
    code = code.replace("nullptr", "NULL")

    return code
