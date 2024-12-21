We are required to build a mini shell (command-line interpreter) that mimics the bash, hence the name it wouldn’t be doing all the work that bash does, but the basic functionality:

- The shell will work only in interactive mode (no scripts, i.e. the executable takes no arguments)
- Run simple commands with absolute, relative path (e.g. `/bin/ls`, `../bin/ls`)
- Run simple commands without a path (e.g. `ls`, `cat`, `grep`, etc…)
- Have a working history (you can navigate through commands with up/down arrows)
- Implement pipes (`|`)
- Implement redirections (`<`, `>`, `>>`)
- Implement the here-doc (`<<`)
- Handle double quotes (`""`) and single quotes (`''`), which should escape special characters, beside `$` for double quotes.
- Handle environment variables (`$` followed by a sequence of characters).
- Handle signals like in bash (`ctrl + C`, `ctrl + \`, `ctrl + D`).
- Implement the following built-ins:  
    — echo (option `-n` only)  
    — exit  
    — env (with no options or arguments)  
    — export (with no options)  
    — unset (with no options)  
    — cd  
    — pwd
- **And for the bonus part (optional, but i did it, because it’s cool!)  
    **— handle `&&` and `||` with the parenthesis `()` for priority.  
    — handle `*` wildcards for the current working directory.
    
	![[Pasted image 20241221202720.png]]
Links: [[Tokens]], [[Parsing]], [[Execution]], 