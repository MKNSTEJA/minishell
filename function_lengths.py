import os
import re

RED = "\033[91m"
RESET = "\033[0m"

def find_functions_in_file(file_path):
    with open(file_path, 'r', errors='ignore') as file:
        lines = file.readlines()
    
    functions = []
    buffer = []
    current_function = None
    start_line = 0
    scope_depth = 0
    
    for line_num, line in enumerate(lines, 1):
        stripped_line = line.strip()
        new_depth = scope_depth + line.count('{') - line.count('}')
        
        # Collect lines while in global scope
        if scope_depth == 0:
            buffer.append((line_num, stripped_line))
        # Check for entering new top-level scope
        if new_depth > scope_depth and scope_depth == 0:
            function_name = None
            start = 0
            
            # Check buffer in reverse for function signature
            for buf_ln, buf_li in reversed(buffer):
                if not buf_li:
                    continue
                
                # Check for function pattern
                match = re.search(
                    r'^(\w+\s+)?\b(\w+)\s*\([^)]*\)\s*(\{)?$',
                    buf_li
                )
                if match and not re.search(r'^(if|while|for|switch)\b', buf_li):
                    function_name = match.group(2)
                    start = buf_ln
                    break
            
            if function_name:
                current_function = function_name
                start_line = start
            else:
                current_function = None
            
            buffer = []
        
        # Check for exiting function scope
        if scope_depth > 0 and new_depth == 0 and current_function:
            end_line = line_num
            line_count = end_line - start_line + 1
            functions.append((current_function, line_count, start_line))
            current_function = None
        
        scope_depth = new_depth
    
    return functions

def main():
    function_list = []
    
    for root, dirs, files in os.walk('.'):
        for file in files:
            if file.endswith('.c'):
                file_path = os.path.join(root, file)
                functions = find_functions_in_file(file_path)
                for func in functions:
                    function_list.append((func[1], func[0], file_path, func[2]))
    
    # Sort by line count descending
    function_list.sort(reverse=True)
    
    # Print results
    print("Functions sorted by length (longest to shortest):\n")
    print(f"{'Function':<30} {'Lines':<6} {'File':<60} {'Start Line'}")
    print("-" * 110)
    for func in function_list:
        if (func[0] > 25):
            print(f"{RED}{func[1]:<30} {func[0]:<6} {func[2]:<60} {func[3]}{RESET}")
        else:
            print(f"{func[1]:<30} {func[0]:<6} {func[2]:<60} {func[3]}")

if __name__ == "__main__":
    main()