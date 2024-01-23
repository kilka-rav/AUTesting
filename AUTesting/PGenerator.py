from dataclasses import dataclass
import copy
from typing import Optional


@dataclass
class Prompt:
    signature: str
    example: Optional[str] = None
    doc: Optional[str] = None
    body: Optional[str] = None
    # should include prev test and error message
    error: Optional[str] = None

    def refineWithExample(self, example: str) -> "Prompt":
        newPrompt = copy.deepcopy(self)
        newPrompt.example = example
        return newPrompt

    def refineWithDoc(self, doc: str) -> "Prompt":
        newPrompt = copy.deepcopy(self)
        newPrompt.doc = doc
        return newPrompt

    def refineWithBody(self, body: str) -> "Prompt":
        newPrompt = copy.deepcopy(self)
        newPrompt.body = body
        return newPrompt

    def refineFromError(self, error: str) -> "Prompt":
        newPrompt = copy.deepcopy(self)
        newPrompt.error = error
        return newPrompt

    def generate(self) -> str:
        prompt = """
Your task is to generate a runnable test case code for the provided code. Please ensure that the test case covers all possible scenarios and edge cases, and that the code is easy to read and understand. Your response should only include the runnable code. Do not return any original code. Additionally, please make sure that the test case is well-organized and follows best practices for testing.
"""
        prompt += f"""
Function for test: 
{self.signature}
"""
        if self.body:
            prompt += f"""
{self.doc}
"""

        if self.example:
            prompt += f"""
Example os usage:
{self.example}
"""

        if self.doc:
            prompt += f"""
Doc comment:
{self.doc}
"""

        if self.error:
            prompt += f"""
{self.error}
Fixed test:
"""
        return prompt

    def __str__(self) -> str:
        return self.generate()


def generate(
    f_signature: str,
    f_body: Optional[str] = None,
    f_doc: Optional[str] = None,
    usage_example: Optional[str] = None,
) -> list[Prompt]:
    # create base prompt
    prompts = [Prompt(f_signature)]

    # each next prompt has more information
    if usage_example:
        prompts.append(prompts[-1].refineWithExample(usage_example))
    if f_doc:
        prompts.append(prompts[-1].refineWithDoc(f_doc))
    if f_body:
        prompts.append(prompts[-1].refineWithBody(f_body))

    return prompts
