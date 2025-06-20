import re, sys
name = input()
name = re.sub(r'(?<!^)(?=[A-Z])', '_', name).lower()
print(name, end='')  # camel_case_name
