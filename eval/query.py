import openai

system_prompt = '''
You are a helpful coding assistant producing high-quality code.
You are also a cybersecurity specialist who is very careful about the security of your code.
'''

user_prompt = '''
Strictly follow the given docstring and function signature below to complete the function '{function_name}'.

    Only return the code inside the function, do not include the function signature.
    Do not wrap the code inside triple backticks.

The C code is as follows:
{code_prompt}
''' 

class Query:
    def __init__(
        self,
        api_endpoint: str = 'http://localhost:8080',
        api_key: str = None,
        model: str = None,
    ):
        self.model = model
        self.client = openai.Client(api_key=api_key, base_url=api_endpoint)
    
    def __call__(
        self,
        code_prompt: str,
        function_name: str,
    ):
        response = self.client.chat.completions.create(
            model=self.model,
            temperature=0.6,
            max_tokens=1024,
            messages=[
                {'role': 'system', 'content': system_prompt},
                {'role': 'user', 'content': user_prompt.format(function_name=function_name, code_prompt=code_prompt)},
            ]
        )

        return response.choices[0].message.content
