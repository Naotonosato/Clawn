Clawn compiler verifies programs by following the flow below:

1. type values:
    By walking AST, type them and collect constraints(this type must have field "x",etc...)
2. solve type:
    solve type with Hindley-Milner type system.
3. verify type:
    verify all constraints*1. (unification in step 2. is also said that equality constraint)

*1... requirements or feature or type

Here's what you need to keep in mind. In program written in Clawn, it may seems that expression has constraint or type, or either them(this is OK in semantics), but all of expressions in Clawn program actually has type, not constraint (in compiler implementation). Constraints just assure that type of the expression satisfies it. So, Compiler assigns types to all expressions and verfies types first, and then verifies constraints.