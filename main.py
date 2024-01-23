from openai import OpenAI
import logging
import re
import uuid

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
    code_block_pattern_cpp = r"```cpp(.*?)```"
    code_block_pattern_c = r"```c(.*?)```"

    # Use regex to find all code blocks
    code_blocks = re.findall(code_block_pattern_cpp, response, re.DOTALL)
    code_blocks.extend(re.findall(code_block_pattern_c, response, re.DOTALL))

    # Clean up the code blocks by stripping unnecessary whitespace
    cleaned_code_blocks = [block.strip() for block in code_blocks]

    cleaned_code_blocks = [
        "\n".join(block.split("\n")[1:]) for block in cleaned_code_blocks
    ]

    return cleaned_code_blocks


if __name__ == "__main__":
    logging.basicConfig(level=logging.DEBUG)
    logging.debug("Hello world!")

    # print(extract_code_from_chatgpt_response(test))
    # abort

    file_to_test = "./examples/simple.cpp"
    a = aup.Parser(file_to_test)
    a.run()

    func_s = a.functions

    # test first function
    func = func_s[0]
    prompts = pgen.generate(func, "")  # FIXME: separate signature and function body

    # use LLM to generate tests
    client = OpenAI()

    # generate initial chats
    messages_s = []
    for prompt in prompts:
        messages = [
            {
                "role": "system",
                "content": "You are a professional tester of C++ programs. When I ask you to write a test, you will answer only in code without any explanatory text. Response should not contain tested code.",
            },
            {
                "role": "user",
                "content": prompt.generate(),
            },
        ]
        messages_s.append(messages)

    for prompt in messages_s:
        logging.info(f"Prompt: {prompt}")
        completion = client.chat.completions.create(
            model="gpt-3.5-turbo", messages=prompt
        )
        logging.info(f"Response: {completion}")
        prompt.append(
            {"role": "assistant", "content": completion.choices[0].message.content}
        )

    # NOTE: assume that there is only once code section in a response
    tests = []
    for compl in messages_s:
        test = extract_code_from_chatgpt_response(compl[-1]["content"])
        if len(compl) <= 2:
            logging.warn(f"Skip ")
            continue
        if len(test) == 0:
            tests.append(compl[-1]["content"])
        else:
            tests.append(test[0])

    logging.info(f"Tests:")
    for test in tests:
        test_src = "./build/" + str(uuid.uuid4()) + ".cpp"
        with open(test_src, "w") as cpp:
            code = compiler.fixErrors(test)
            print(code, file=cpp)
        logging.info(f"--------------------------------------------------")
        logging.info(f"  Test:\n{test}")
        logging.info(f"Compile:")
        stat = compiler.Compiler(test_src).run("./test.out")
        logging.info(f"Compiler result: {stat}")
