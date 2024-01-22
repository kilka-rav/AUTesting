from openai import OpenAI
import logging
import re

import AUTesting.PGenerator as pgen
import AUTesting.parser as aup
import AUTesting.compiler as compiler


def extract_code_from_chatgpt_response(response):
    """
    Extracts code blocks from a ChatGPT response.

    :param response: A string containing the ChatGPT response.
    :return: A list of code blocks extracted from the response.
    """
    # Define the regex pattern for code blocks
    code_block_pattern = r"```(.*?)```"

    # Use regex to find all code blocks
    code_blocks = re.findall(code_block_pattern, response, re.DOTALL)

    # Clean up the code blocks by stripping unnecessary whitespace
    cleaned_code_blocks = [block.strip() for block in code_blocks]

    return cleaned_code_blocks


if __name__ == "__main__":
    logging.basicConfig(level=logging.DEBUG)
    logging.debug("Hello world!")

    a = aup.Parser("./examples/simple.cpp")
    a.run()

    func_s = a.functions

    # test first function
    func = func_s[0]
    prompts = pgen.generate(func, "")  # FIXME: separate signature and function body

    # use LLM to generate tests
    client = OpenAI()

    completions = []
    for prompt in prompts:
        logging.info(f"Prompt: {prompt}")
        completion = client.chat.completions.create(
            model="gpt-3.5-turbo-1106",
            messages=[
                {
                    "role": "system",
                    "content": "You are a professional tester of C++ programs. When I ask you to write a test, you will answer only in code without any explanatory text. Response should not contain tested code code.",
                },
                {
                    "role": "user",
                    "content": prompt.generate(),
                },
            ],
        )
        completions.append(completion)
        logging.info(f"Response: {completion}")
        break

    # NOTE: assume that there is only once code section in a response
    tests = []
    for compl in completions:
        for choice in compl.choices:
            tests.append(extract_code_from_chatgpt_response(choice.message.content)[0])

    logging.info(f"Tests:")
    for test in tests:
        logging.info(f"--------------------------------------------------")
        logging.info(f"  Test:\n{test}")
