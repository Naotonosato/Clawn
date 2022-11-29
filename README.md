# Clawn
Clawn is a programming language developed with the goal of making it easy to write high-quality, safe programs.

![demo](https://user-images.githubusercontent.com/33174108/199721317-3729b791-fa9b-49a6-af59-a36d5cf9e8a0.gif)

Currently, Clawn has the following features
+ Simple syntax
+ Statically typing
+ Higher rank polymorphism
+ Safe "any"
+ elegant C FFI
+ Automatic memory management (WIP)

I value a gentle learning curve, and Clawn is designed to be easy to learn. For example, Clawn's function definitions have no type annotations and are defined as polymorphic functions, with the goal of reducing the amount of knowledge you have to be aware of from the beginning like declaring types and entry points (int main(), etc.) in introductory programs like defining and calling simple functions.
There are various other features as described below.

## Installation


At first,download [Dockerfile](https://github.com/Naotonosato/Clawn/blob/main/Dockerfile) and execute the following commands.
```shell
docker image build -t clawn-playground-image:0.1 .
docker run --name clawn-playground -it clawn-playground-image:0.1 /bin/bash
```
and
```
git clone https://github.com/Naotonosato/Clawn
cd Clawn
mkdir build
cd build
cmake ..
make
```
Then, you can try clawn.
```shell
./clawn ../sample/hello_world.clawn 
```


## Syntax

Clawn has simple syntax similar to natural language, and is designed with the utmost importance placed on understandability.

+ Variable definitions/assignments.

```c++
a = 10
a = 100
```

Clawn sets the global scope of the input file to the entry point, so there is no need to declare int main(), etc., and you can start writing your program immediately.

+ Function calls.

```c++
print("Hello, World!”)
```

Output:
```
Hello, World!
``` 
+ block expression

```c++
result = {
	print("Block expression”)
	=>0
	print("End of Block expression”)
}
print("Result is " + int_to_str(result))
```

Output:
```
Block expression
End of Block expression
Result is 0
```

A block consisting of one or more expressions in curly braces returns the value of `expr` as the value of the block if there is a statement `=> expr` in the middle of the block. This is useful to organize the processing at a good granularity and to make the program easier to understand. If `=>` does not appear in the block, the block will not return a value.

+ if expression

```c++
x = 100
message = if x == 100
{
	print("x is 100.”)
	=>”OK.”
} else
{
	=>"Something went wrong.”
}
print(message)
````

Output:

```shell
x is 100.
OK.
```

In Clawn, if is an expression, not a statement, and returns a value, as does block.



+ repeat expression

```python
result = repeat count:count<5
    {
        print("count: " +int_to_str(count))
        =>count
    }
print(int_to_str(result))
```

Output:
```
count: 0
count: 1
count: 2
count: 3
count: 3
count: 4
```

A repeat expression is one that increments the counter variable while the `condition` is satisfied by setting `repeat `name of the counter variable`:`condition`, and repeats the following expression, returning a value in the same way as if and block.

+ See also.

```c++
value = “value”
reference = refer value
print(access reference)
```

Output:
```
value
```

You can use ``refer`` to get a reference and ``access`` to get the value of the reference.

+ function

```c++
function f(x)
{
	return x
}

f(0)
print(f("abc”))
a = f
print(float_to_str(a(0.1)))
```

Output:
```
abc
0.100000
```



The function is defined as a polymorphic function in the form function `function name` (`argument1`,`argument2`...) The function is defined as a polymorphic function in the form ``(`argument1`,`argument2`...)''. A `requirement` described below may be specified for the argument to restrict the type of the argument passed, although a type annotation is not required. Also, functions in Clawn are first-class objects and can be assigned to variables or passed as arguments to functions.

+ struct

```c++
structure S
{
	x
	y
}

instance = S{x:10.0,y:0.0}
print(float_to_str(instance.x))
```

Output:
```
10.000000
```

The structure is

structure `structure name`.

{

``member name``…

}

and can be instantiated in the form `structure name` {`member name`:`expr`...} and can be instantiated in the form ``structure name`{`member name`:`expr`…}.

Like functions, the `requirements` described below can be applied to members.

+ direct sum type

```c++
union U
{
	A
	B
}

u = U#A(0)
u = U#B(“abc”)

str = match u
{
	A=>{
        =>int_to_str(u.A)
    }
	B=>{
        =>u.B
    }
}
print(str)
````

output:

```
abc
```

The direct sum type is 

Union `name`

{

`tag`…

}

and can be instantiated with the form `type_name`#`tag_name`(`expr`).

It is also possible to switch processing depending on the value held by the match expression.

+ Requirement.

```` hasWidth
requirement HasWidth
{
	has: width: RealNumber
}

function f(x:HasWidth)
{
	return x.width
}

f(0)
````

Output:

````powershell
Error: requirement 'HasWidth(has member named 'width': RealNumber)' is set at line 7 but not satisfied with type ‘Integer’.
````

The requirement can be a function argument, a structure member, or a tag of a direct sum type, and is set to

+ type name
+ requirement clause
+ requirement clause

The requirement clause is a function argument, a member of a structure, or a tag of a direct sum type. Requirement clauses are

+ `value`, which requires the type to be a value type
+ value:X`, which requires it to be a value type satisfying requirement `X`, or
+ `reference`, which requires it to be a reference type
+ `reference:X` requiring it to be a reference type to a type satisfying requirement `X`.
+ `has: name`, which requires it to have a member named `name`.
+ `has:name:X`, which requires the type to have a member `name` that satisfies requirement `X`.

and that it can be called from now on with the arguments `arg1,arg2... In the future, we plan to implement `callable_with`, which requires that the function be callable with `arg1,arg2...` arguments, and `returns:X`, which requires that the return type satisfy the requirement `X`.

Also, the requirement name should be　

requirement `requirement_name`.

{

`requirement`…

}


This feature prevents unintended program execution.

## C FFI

Clawn can easily call C functions.

```python
import “source.c”

c_function()
```

Thus, you can call a function in a C program by simply importing `path/to/source.c`.

## Memory Management(WIP)

Clawn has a memory management mechanism that statically verifies the dependencies of values in the program and automatically inserts code to allocate and release memory at appropriate times, so programmers do not need to be aware of memory.

However, this feature is experimental and will be further stabilized and optimized in the future.

## Roadmap
Clawn is still a work in progress, but will be actively developed after 2023 (after the author's college entrance exam).
+ Module system
+ Support for OOP
+ Add pure function
+ Improve compile speed
+ Improve error messages
+ Improve memory management mechanism
+ Enhanced optimization
+ Support for iOS, Android, Windows, Linux, Web output
+ Standard libraries
+ Build system
+ Package manager

## Theoretical Aspects(Japanese)
https://github.com/Naotonosato/Clawn/blob/main/docs/design.md

## Projects using Clawn
+ https://github.com/taisukef/Geo3x3/