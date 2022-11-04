# This is how Clawn compiler compiles input code.

+ Phaze 1: Parse input and build AST.
+ Phaze 2: Apply plugins to AST.
    + solve names
+ Phaze 3: Convert AST to HIR.
+ Phaze 4: Apply plugins to HIR.
    + monomorphize generics
+ Phaze 5: Verify HIR by checking types and constraints.
+ Phaze 6: Convert HIR to MIR.
+ Phaze 7: Apply plugins to MIR.
    + optimize MIR
    + insert malloc/free
+ Phaze 8: Convert MIR to LLVM IR.
+ Phaze 9: Apply Plugin(LLVM Path) to LLVM IR.
+ Phaze 10: Generate output from LLVM IR.