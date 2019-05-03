#ifndef PARSER_H
#define PARSER_H

#include "ast/ast.hpp"
#include <set>

/*
 * program = PROGRAM identifier SEMICOLON program_block DOT
 * program_block =
 */

class Parser {
    enum class IdentType {
        CONST, VAR, TYPE, FUNCTION, PROCEDURE
    };

    Scanner scanner;
    Token   token;
    std::list<std::string> errors_list;
    std::map<std::string, IdentType>  identifiers;

    inline const Token &nextToken() { return token = scanner.nextToken(); }
    template<typename T>
    uptr_list<T> EmptyList() {
        return uptr_list<T>();
    }

    uptr<AST::Program>             parse_program();
    uptr_list<AST::Identifier>     parse_identifier_list();
    uptr<AST::Block>               parse_block();
    uptr_list<AST::TypeDef>        parse_typedef_part();
    uptr_list<AST::VarDecl>        parse_vardecl_part();
    uptr<AST::TypeDenoter>         parse_type_denoter();
    uptr<AST::ConstDecl>           parse_const_decl();
    uptr<AST::Constant>            parse_constant();
    uptr<AST::CompoundStatement>   parse_compound_statement();
    uptr<AST::Statement>           parse_statement();
    uptr<AST::Statement>           parse_simple_statement();
    uptr<AST::Statement>           parse_structured_statement();
    uptr<AST::Number>              parse_number();
    uptr<AST::ConstString>         parse_const_string();
    const Token &process_error(lexem::Type expected, lexem::Type skip_until);
    const Token &process_error(lexem::Type expected,
                               const std::vector<lexem::Type> &skip_until);
    const Token &process_error(lexem::Type expected, lexem::Type skip_until,
                               const char *file, int line);
    const Token &process_error(lexem::Type expected,
                               const std::vector<lexem::Type> &skip_until,
                               const char *file, int line);
    const Token &process_error(const std::vector<lexem::Type> &expected,
                               const std::vector<lexem::Type> &skip_until,
                               const char *file, int line);

public:
    const uptr<AST::Program> entry_point;

    Parser(Scanner scanner):
        scanner(scanner), entry_point(parse_program()) {}
    void print_errors();

};

//class Parser {
//    Scanner scanner;
//    Token   token;
//    std::set<std::string> names;

//    Token nextToken()
//    {
//        token = scanner.nextToken();
//        return token;
//    }

//    std::unique_ptr<AST::Function>   parseFuncMain();   //Function
//    std::unique_ptr<AST::Prototype>  parsePrototype();  //Prototype
//    std::unique_ptr<AST::Body>       parseBody();       //Body
//    std::unique_ptr<AST::Expr>       parseStmt();       //VarExpr/ForExpr/IfExpr
//    std::unique_ptr<AST::AssignExpr> parseAssignment(); //AssignExpr
//    std::unique_ptr<AST::LoopExpr>   parseLoop();       //LoopExpr
//    std::unique_ptr<AST::BinaryOp>   parseBoolOp();     //BinaryOp
//    std::unique_ptr<AST::IfExpr>     parseBranch();     //IfExpr
//    std::unique_ptr<AST::ReturnVal>  parseReturn();     //ReturnVal
//    std::unique_ptr<AST::Expr>       parseValue();      //Var/BinaryOp
//    std::unique_ptr<AST::Expr>       parseVar();        //Var / Number
//public:

//    std::unique_ptr<AST::Expr> entry_point;

//    Parser(Scanner scanner):
//        scanner(scanner)
//    {
//        nextToken();
//        entry_point = parseFuncMain();
//    }


//};

// <func_main> = <func_prototype> <func_body> .
// <func_prototype> = IDENT LPAREN RPAREN.
// <func_body> = LBRACE { stmt } RBRACE .
// <stmt>      = <assign> | <loop> | <branch> | <ret> .
// <assign>    = IDENT = <value> SEMICOLON.
// <loop>      = for LPAREN <value> SEMICOLON <value> SEMICOLON <value> RPAREN <func_body>
// <branch>    = if  LPAREN <value> RPAREN    <value> : <value>
// <ret>       = return <value>
// <var>       = NUM | IDENT
// <value>     = var | var ARITHMOP var | <branch>.

#endif // PARSER_H
