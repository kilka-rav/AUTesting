import os
import re


class Parser(Exception):
    def __init__(self, path_to_file):
        self.path = path_to_file
        self.functions = []
        self.signatures = []
        self.includes = []

    def isExist(self):
        return os.path.isfile(self.path)

    def get_body(self, declarations, file):
        inFunction = False
        numBracket = 0
        cur_func = ""
        lines = file.split("\n")
        for line in lines:
            if "#include" in line:
                self.includes.append(line + "\n")
            if inFunction:
                numBracket = numBracket + line.count("{") - line.count("}")
                cur_func = cur_func + line + "\n"
                if numBracket == 0:
                    self.functions.append(cur_func)
                    inFunction = False
                    cur_func = ""
            else:
                for dec in declarations:
                    if dec in line and len(dec) > 0:
                        if "}" in line:
                            self.functions.append(line)
                            self.signatures.append(dec)
                        elif "{" in line or ";" not in line:
                            inFunction = True
                            cur_func = line + "\n"
                            numBracket = line.count("{")
                            self.signatures.append(dec)

    def clear_bracket(self, a):
        lst = []
        for el in a:
            el = el.replace(" {", "")
            el = el.replace("{", "")
            el = el.replace(") ", ")")
            el = el.replace(")\n", ")")
            el = el.replace(") ", ")")
            lst.append(el)
        return lst

    def find_function(self):
        pattern_next = r"\b\w+\s+\w+\s*\([^)]*\)\s*(?:\n\s*)?\{"
        pattern_c = r"\b\w+\s+\w+\s*\([^)]*\)\s*"
        pattern_method = r"\w+\s+\w+::\w+\([^)]*\)\s*{"

        with open(self.path, "r") as file:
            data = " ".join(file.readlines())
            a = re.findall(pattern_next, data)
            b = re.findall(pattern_c, data)
            c = re.findall(pattern_method, data)
            a = self.clear_bracket(a)
            b = self.clear_bracket(b)
            c = self.clear_bracket(c)

            result = list(set(a) | set(b) | set(c))
            self.get_body(result, data)

    def run(self):
        if not self.isExist():
            raise Parser(self.path + " isn't exist")
        self.find_function()


def extract_code_from_chatgpt_response(response):
    """
    Extracts code blocks from a ChatGPT response.

    :param response: A string containing the ChatGPT response.
    :return: A list of code blocks extracted from the response.
    """
    # Define the regex pattern for code blocks
    code_block_pattern_cpp = r"(```.*?```)"

    # Use regex to find all code blocks
    code_blocks = re.findall(code_block_pattern_cpp, response, re.DOTALL)

    # Clean up the code blocks by stripping unnecessary whitespace
    cleaned_code_blocks = [block.strip() for block in code_blocks]

    cleaned_code_blocks = [
        "\n".join(block.split("\n")[1:-1]) for block in cleaned_code_blocks
    ]

    return cleaned_code_blocks
