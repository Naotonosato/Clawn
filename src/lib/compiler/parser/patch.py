#use http://www.comrite.com/wp/c11-stdshared_ptr-as-semantic-value-in-bison-c-mode/
#as reference. thanks!

import getpass
try:
    with open(f"/Users/{getpass.getuser()}/Desktop/Clawn/build/parser.hpp") as file:
        source = file.read()
        source = source.replace(
            "return *new (yyas_<T> ()) T (t);",
            "return *new (yyas_<T> ()) T (std::move((T&)t));"
            )
    with open(f"/Users/{getpass.getuser()}/Desktop/Clawn/build/parser.hpp","wt") as file:
        file.write(source)
except:pass