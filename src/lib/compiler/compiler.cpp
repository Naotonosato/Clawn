#include <string>
#include <iostream>
#include <fstream>
#include "include/ast/builder.hpp"
#include "include/compiler/error.hpp"
#include "include/compiler/parser/driver.hpp"
#include "include/hir/hir.hpp"
#include "include/hir/visitor.hpp"
#include "include/mir/mir.hpp"
#include "include/compiler/module/module.hpp"
#include "include/compiler/pipeline.hpp"
//#include <lld/Common/Driver.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/PassManager.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/raw_ostream.h>

#include <unistd.h>
#include <experimental/filesystem>
#include <fstream>
using namespace std::experimental;
// running llc llvmir;clang llvmir.s -o a.out;./a.out; echo \"$?\";
// in build directory makes exe file.

namespace clawn::compiler
{
static std::string compiler_path;

std::string abs(std::string path) { return compiler_path + "/" + path; }

int get_compile_options(int argc, char** argv) { return 0; }

class CompileOption
{
    std::vector<std::string> links;
    std::string filename;

    public:
    int imported_line = 0;
    void link(const std::string& filename) { links.push_back(filename); }
    void set_filename(const std::string& filename_) { filename = filename_; }
    std::string get_filename() { return filename; }
    std::vector<std::string> get_files_to_link() { return links; }
};

bool link(std::vector<std::string> arguments)
{
    std::vector<const char*> arguments_char_array(arguments.size());
    size_t idx = 0;
    for (auto& arg : arguments)
    {
        arguments_char_array[idx] = arg.c_str();
        idx += 1;
    }
    llvm::ArrayRef<const char*> lld_arguments(arguments_char_array);
    // lld::
    // return lld::mach_o::link(lld_arguments, true);
    return false;
}

// link({"-execute", "-lsystem",
//      "/Users/ueharanaoto/Desktop/Clawn/test/cpp.o"});
std::vector<std::string> get_imports(std::string filename)
{
    std::ifstream file(filename);
    std::string line;
    std::string source_code;
    while (getline(file, line)) source_code += line + "\n";
    std::string current_token;
    std::vector<std::string> tokens;
    for (auto& c : source_code)
    {
        if (c == ' ' || c == '\n' || c == '\r')
        {
            tokens.push_back(current_token);
            current_token = "";
            continue;
        }
        current_token += c;
    }
    std::string last_token;
    std::vector<std::string> imports;
    for (auto& token : tokens)
    {
        if (token == "") continue;
        if (last_token == "import" && token.front() == '"' &&
            token.back() == '"')
        {
            auto filename_to_import = token.substr(1, token.size() - 2);

            imports.push_back(filename_to_import);
        }
        last_token = token;
    }
    return imports;
}

std::string get_filename(std::string name)
{
    auto index = name.rfind("/");
    std::string filename_with_ext = name;
    if (index != std::string::npos)
        filename_with_ext = name.substr(index + 1, name.size() - index - 1);
    index = name.rfind('\\');
    if (index != std::string::npos)
        filename_with_ext = name.substr(index + 1, name.size() - index - 1);
    auto ext_index = filename_with_ext.find_last_of(".");
    if (ext_index == std::string::npos) return filename_with_ext;
    return filename_with_ext.substr(0, ext_index);
}

std::string extract_file_name(const std::string& path,
                              bool without_extension = true)
{
    std::string fn;
    std::string::size_type fpos;
    if ((fpos = path.find_last_of("/")) != std::string::npos)
    {
        fn = path.substr(fpos + 1);
    }
    else if ((fpos = path.find_last_of("\\")) != std::string::npos)
    {
        fn = path.substr(fpos + 1);
    }
    else
    {
        fn = path;
    }

    if (without_extension && (fpos = fn.find_last_of(".")) != std::string::npos)
    {
        fn = fn.substr(0, fpos);
    }

    return fn;
}

class CompileOption get_expanded(const std::string& filename)
{
    class CompileOption options;
    char buf[1000];
    system("mkdir tmp");
    readlink("/proc/self/exe", buf, 1000);
    //system("python3 cridge.py builtins.hpp");
    std::string expanded_filename =
        abs("tmp/" + get_filename(filename) + ".clawn");
    options.set_filename(expanded_filename);
    std::ofstream expand_file(expanded_filename);
    std::ifstream file("builtins.cridge");
    std::string import_code;
    unsigned int imported_line = 0;
    while (getline(file, import_code))
    {
        expand_file << import_code << std::endl;
        imported_line += 1;
    }

    auto imports = get_imports(filename);
    for (auto& name : imports)
    {
        std::ifstream file(abs(name));
        if (!file)
        {
            file = std::ifstream(name);
        }
        if (!file)
        {
            throw Error::cannot_open_file(name);
        }
        std::string ext = strrchr(name.c_str(), '.');
        if (ext == ".c" || ext == ".cpp")
        {
            system(("python3 cridge.py " + name).c_str());
            system(("gcc -c -Wno-everything " + name + " -o " + "tmp/" +
                    get_filename(name) + ".o")
                       .c_str());
            options.link("tmp/" + get_filename(name) + ".o");
            file = std::ifstream(extract_file_name(name) + ".cridge");
            if (!file)
            {
                throw Error::cannot_open_file(extract_file_name(name) +
                                              ".cridge");
            }
        }
        std::string source_code;
        while (getline(file, source_code))
        {
            expand_file << source_code << std::endl;
            imported_line += 1;
        }
    }

    std::ifstream source_file(filename);
    std::string source_code;
    while (getline(source_file, source_code))
    {
        expand_file << source_code << std::endl;
        ;
    }
    options.imported_line = imported_line;

    return options;
}

void run_commands(CompileOption option, int argc, char** argv)
{
    std::string emitted_object_file = "tmp/compiled.o";
    system("g++ -c -std=c++17 -O3 -Wno-return-type builtins.cpp -o tmp/builtins.o;");
    std::vector<std::string> link_arguments =
        {};  //{emitted_object_file, "-arch",
             //"x86_64","-demangle","-lto_library","/usr/local/Cellar/llvm/9.0.0_1/lib/libLTO.dylib","-no_deduplicate",
             //"-dynamic", "-dylib", "-macosx_version_min",
             //"10.17.0","/usr/local/Cellar/llvm/9.0.0_1/lib/clang/9.0.0/lib/darwin/libclang_rt.osx.a",
             //"/usr/local/Cellar/llvm/9.0.0_1/lib/libLTO.dylib","-syslibroot","/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk","-L/usr/local/lib",
             //"/usr/local/Cellar/llvm/9.0.0_1/lib/clang/9.0.0/lib/darwin/libclang_rt.osx.a",emitted_object_file,
             //"builtins.so"};
    std::string links;
    for (auto link_file : option.get_files_to_link())
    {
        link_arguments.push_back(link_file);
        links += " " + link_file;
    }
    std::string args;
    if (argc > 2)
    {
        for (int i = 2; i < argc; i += 1)
        {
            args += argv[i];
            args += " ";
        }
    }
    // link(link_arguments);
    system(("g++ -lstdc++ " + args + " " + emitted_object_file + " " +
            links + " tmp/builtins.o -o " +
            extract_file_name(option.get_filename()) + ".out")
               .c_str());
    system(("./" + extract_file_name(option.get_filename()) + ".out").c_str());
    system("rm -r tmp");
}

int compile(int argc, char** argv)
{
    try
    {
        std::string filename = argv[1];
        auto compile_option = get_expanded(filename);

        auto clawn_module = std::make_shared<compiler::Module>(
            compile_option.get_filename().c_str(),
            compile_option.imported_line);
        
        auto& ast_root = *clawn_module->generate_ast();

        auto hir = pipeline::ast_to_hir(ast_root, clawn_module);

        std::cout << "\e[1;32m"
                     "got hir"
                     "\e[0m"
                  << std::endl;

        auto specialized_functions =
            pipeline::monomorphizer(*hir, clawn_module->get_type_environment());
        hir->as_mutable<hir::Root>().insert(std::move(specialized_functions));

        std::cout << "\e[1;32m"
                     "specialized hir"
                     "\e[0m"
                  << std::endl;

        auto error = pipeline::verify_hir(*hir, clawn_module);
        if (error.has_value())
        {
            error.value().dump();
            return 1;
        }

        auto mir_context = std::make_shared<mir::Context>(
            clawn_module->get_type_environment());
        clawn::compiler::pipeline::hir_to_mir(*hir, mir_context);

        std::cout << "\e[1;32m"
                     "got mir"
                     "\e[0m"
                  << std::endl;

        auto llvm_context =
            std::shared_ptr<llvm::LLVMContext>(new llvm::LLVMContext);
        std::shared_ptr<llvm::Module> llvm_module(
            new llvm::Module("Clawn", *llvm_context));
        auto llvm_ir_builder = std::shared_ptr<llvm::IRBuilder<>>(
            new llvm::IRBuilder<>(*llvm_context));

        pipeline::mir_to_llvm_ir(*mir_context, *llvm_context, *llvm_module,
                                 *llvm_ir_builder);

        std::cout << "\e[1;32m"
                     "got llvm ir"
                     "\e[0m"
                  << std::endl;


        pipeline::create_binary("tmp/compiled.o", llvm_module.get());

        std::cout << "\e[1;32m"
                     "Compilation succeeded"
                     "\e[0m"
                  << std::endl;
        run_commands(compile_option, argc, argv);
    }
    catch (Error error)
    {
        error.dump();
        exit(1);
    }

    return 0;
}
}  // namespace clawn::compiler