- Parsing a command goes through two phases, the lexical analysis (lexing) which produces “lexems” and then the syntax analysis (parsing the lexems).

- **Lexical analysis / tokenization:** taking the input from the user and processing it char by char into “[[Tokens]]”.

- **Syntax analysis / Parsing:** scanning the stream of tokens according to a specific grammar and then deciding what to do with them (e.g. generating an [AST — Abstract Syntax Tree](https://en.wikipedia.org/wiki/Abstract_syntax_tree)). 
	More about [[AST]]. 

- Bash does parsing by using a combination of techniques and mainly [The Recursive Descent](https://en.wikipedia.org/wiki/Recursive_descent_parser)
	More about [[The Recursive Descent]]
- Before I got a bit familiar with the Recursive Descent, I had to understand first what a grammar is and what a language is, etc…  
	So what is a grammar?
	
	There are several kinds of grammars, but the type of grammar that is used for this project is called [CFG (Context Free Grammar)](https://en.wikipedia.org/wiki/Context-free_grammar).
		More about it [[CFG (Context Free Grammar)]]
	A grammar is basically a way of telling how a language can be made, back to the example above… We can create a grammar that tells us how an arithmetic expression can be formed, for example:

	```
	//The following grammar is written in Backus-nauer form  
	  
	<Expression>  :=  <Number>  
	              |   <Number> ‘+’ <Expression>  
	  
	<Number>      :=  One or more Digits
	```
	
	We are saying in this grammar, that an Expression can be a Number or a Number followed by a `+` sign that is followed by another Expression, and a Number can be one or more digits.

*"After doing some research I found another technique that helped me to get my parser together, and it is like a newer improved version of the Recursive Descent, it is called Precedence Climbing , which you can read more about in [**this article**](https://eli.thegreenplace.net/2012/08/02/parsing-expressions-by-precedence-climbing) that helped me a lot in understanding it."*
	More about it: [[Precedence Climbing Technique]].
	

**Here-doc initialization:**

Here-doc initialization is where here-doc gets initialized by storing the input in a [pipe](https://www.geeksforgeeks.org/pipe-system-call/), which will be used later by the execution — **_though we have to make sure to close the corresponding file descriptors, else the program may freeze somewhere._**

After finishing initializing the AST, comes the time for the [[Execution]].
