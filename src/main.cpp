#include <iostream>

#include "frontend/lexer/scanner.hpp"
#include "frontend/parser/parser.hpp"

void test_params(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Too less arguments!\n Usages: <progam_source_file>\n. Please, try again!"
                  << std::endl;
        exit(1);
    }

    if (argc > 2) {
        std::cout << "Too much arguments, parameters ";
        for (int i = 2; i < argc - 1; ++i) {
            std::cout << "\"" << argv[i] << "\", ";
        }
        std::cout << "\"" << argv[argc - 1] << "\" ignored" << std::endl;
    }

}

int main(int argc, char* argv[])
{
    test_params(argc, argv);
    Scanner scan (argv[1]);
    while (scan.nextToken().type() != lexem::END_OF_FILE);
    scan.print_errors();
//    Scanner scan("../prog.txt");
//    Parser p(scan);
//    p.print_errors();
//    if (p.entry_point) p.entry_point->print();
//    llvm::InitializeNativeTarget();
//    llvm::InitializeNativeTargetAsmPrinter();
//    llvm::InitializeNativeTargetAsmParser();
//    compiler::Module = new llvm::Module("lab2", compiler::localContext);
//    p.entry_point->codegen();
//    compiler::Module->dump();
//    delete compiler::Module;
    return 0;
}

