#include "frontend/parser/parser.hpp"

#include <algorithm>
#include <memory>
#include <vector>

#define PROCESS_ERROR(a, b) process_error(a, b, __FILE__, __LINE__)

/* program = program-heading `;' program-block `.' .
   program-block = block .
   program-heading = `program' identifier [ `(' program-parameter-list `)' ] .
 */
uptr<AST::Program> Parser::parse_program() {
    nextToken();
    // program-heding = `program' identifier [ `(' program-parameter-list `)' ] .
//    if (token != lexem::PROGRAM) {
//        PROCESS_ERROR(lexem::PROGRAM, lexem::END_OF_PROGRAM);
//        return uptr<AST::Program>(nullptr);
//    }
    nextToken();
    if (token != lexem::IDENT) {
        PROCESS_ERROR(lexem::IDENT, lexem::END_OF_FILE);
        return uptr<AST::Program>(nullptr);
    }
    std::string_view ident(token.image());
    nextToken();
    uptr_list<AST::Identifier> program_parameter_list;
    if (token == lexem::LPAREN) {
        nextToken();
        program_parameter_list = std::move(parse_identifier_list());
        (token == lexem::RPAREN)
                ? nextToken()
                : PROCESS_ERROR(lexem::RPAREN, lexem::SEMICOLON);
    }
    (token == lexem::SEMICOLON) ? nextToken() : PROCESS_ERROR(lexem::SEMICOLON, lexem::SEMICOLON);
    uptr<AST::Block> block(parse_block());
    (token == lexem::DOT) ? nextToken() : PROCESS_ERROR(lexem::DOT, lexem::END_OF_FILE);
    (token == lexem::END_OF_FILE) ? nextToken() : PROCESS_ERROR(lexem::END_OF_FILE, lexem::END_OF_FILE);
    return std::make_unique<AST::Program>(
                std::move(ident), std::move(program_parameter_list), std::move(block));
}

uptr_list<AST::Identifier> Parser::parse_identifier_list() {
    uptr_list<AST::Identifier> id_list;
    if (token == lexem::IDENT) {
        id_list.emplace_back(new AST::Identifier(token.image()));
    }
    for (nextToken(); token == lexem::COMMA && nextToken() == lexem::IDENT; nextToken()) {
        id_list.emplace_back(new AST::Identifier(token.image()));
    }
    return id_list;
}

/* *********************** BLOCK *************************** */
uptr<AST::Block> Parser::parse_block() {

    uptr_list<AST::TypeDef>      type_def_part(std::move(parse_typedef_part()));
    uptr_list<AST::VarDecl>      variable_decl_part(std::move(parse_vardecl_part()));
    uptr<AST::Expr>              proc_and_func_decl_part(nullptr);
    uptr<AST::CompoundStatement> statement_part(parse_compound_statement());
    return std::move(std::make_unique<AST::Block>(
                         std::move(type_def_part), std::move(variable_decl_part)
                         , std::move(proc_and_func_decl_part)
                         , std::move(statement_part)));
}

uptr<AST::CompoundStatement> Parser::parse_compound_statement()
{
    uptr_list<AST::Statement> stmt_list;
//    (token == lexem::BEGIN) ? nextToken()
//                            : PROCESS_ERROR(lexem::BEGIN, lexem::END);
//    while (token != lexem::END && token != lexem::END_OF_PROGRAM) {

//    }
//    if (token == lexem::END) {
//        nextToken();
//    }
    return std::make_unique<AST::CompoundStatement>(std::move(stmt_list));
}

/* -------------------- Type defenition part ----------------------- */
uptr_list<AST::TypeDef> Parser::parse_typedef_part() {
    static const std::vector<lexem::Type> stop_symbols
            ({lexem::SEMICOLON, // начало следующего объявления
//              lexem::VAR, // variable-declaration-part
//              lexem::PROCEDURE, lexem::FUNCTION, // procedure-and-function-declaration-part
//              lexem::BEGIN // statement-part
             });
    static const std::vector<lexem::Type> follow(++stop_symbols.begin(), stop_symbols.end());
    uptr_list<AST::TypeDef> type_definition_part;
//    if (token != lexem::TYPE) {
//        return type_definition_part;
//    }
    nextToken();
    std::string_view identifier;
    auto parse_type_defenition = [this, &identifier]() {
        // type-definition = identifier `=' type-denoter
//        identifier.remove_suffix(identifier.size());
        if (token == lexem::IDENT) {
            identifier = token.image();
            nextToken();
        } else {
            PROCESS_ERROR(lexem::IDENT, stop_symbols);
            return uptr<AST::TypeDef>(nullptr);
        }
//        if (token == lexem::EQUAL) {
//            nextToken();
//            return std::make_unique<AST::TypeDef>(identifier, std::move(parse_type_denoter()));
//        }
//        else {
//            PROCESS_ERROR(lexem::EQUAL, stop_symbols);
            return std::make_unique<AST::TypeDef>(identifier, nullptr);
//        }
    };

    while (!token.is_in(follow)) {
        uptr<AST::TypeDef> type_def = std::move(parse_type_defenition());
        if (type_def) {
            type_definition_part.push_back(std::move(type_def));
            if (token != lexem::SEMICOLON) {
                PROCESS_ERROR(lexem::IDENT, lexem::SEMICOLON);
            }
            nextToken();
        }

    }
    return type_definition_part;
}

uptr<AST::TypeDenoter> Parser::parse_type_denoter() {
    if (token != lexem::IDENT) {
        //Никуда не двигаемся в надежде, что ошибку покажет вызвавшее правило
        PROCESS_ERROR(lexem::IDENT, token.type());
        return nullptr;
    }
    std::string_view image = token.image();
    nextToken();
    return std::make_unique<AST::TypeDenoter>(image);
}

/* ---------------------- Variable declaration part -----------------------*/

uptr_list<AST::VarDecl> Parser::parse_vardecl_part() {
    static const std::vector<lexem::Type> stop_symbols
            ({lexem::SEMICOLON, // начало следующего объявления
//              lexem::VAR, // variable-declaration-part
//              lexem::PROCEDURE, lexem::FUNCTION, // procedure-and-function-declaration-part
//              lexem::BEGIN // statement-part
             });
    static const std::vector<lexem::Type> follow(++stop_symbols.begin(), stop_symbols.end());
    // variable-declaration-part = [ `var' variable-declaration `;' { variable-declaration `;'} ] .
    uptr_list<AST::VarDecl> var_decl_part;
//    if (token != lexem::VAR) {
//        return var_decl_part;
//    }
    nextToken();
    auto parse_var_decl = [this]() {
        // variable-declaration = identifier-list ` :' type-denoter
        uptr_list<AST::Identifier> identifier_list;
        if (token == lexem::IDENT) {
            identifier_list = std::move(parse_identifier_list());
        } else {
            PROCESS_ERROR(lexem::IDENT, stop_symbols);
            return uptr<AST::VarDecl>(nullptr);
        }
        if (token == lexem::COLON) {
            nextToken();
            return std::make_unique<AST::VarDecl>(
                        std::move(identifier_list), std::move(parse_type_denoter()));
        } else {
            PROCESS_ERROR(lexem::COLON, stop_symbols);
            return std::make_unique<AST::VarDecl>(std::move(identifier_list), nullptr);
        }
    };

    while (!token.is_in(follow)) {
        uptr<AST::VarDecl> var_decl = std::move(parse_var_decl());
        if (var_decl) {
            var_decl_part.push_back(std::move(var_decl));
            if (token != lexem::SEMICOLON) {
                PROCESS_ERROR(lexem::SEMICOLON, stop_symbols);
            }
            nextToken();
        }

    }
    return var_decl_part;
}


uptr<AST::ConstDecl> Parser::parse_const_decl()
{
    return nullptr;
}

uptr<AST::Constant> Parser::parse_constant()
{
    static const std::vector<lexem::Type> first
            ({lexem::NUM,    // number
              lexem::OPERATOR,   // sign number
              lexem::STRING, // string
             });
    if (token == lexem::STRING) {
        return parse_const_string();
    }
    if (token == lexem::NUM
            || token == lexem::OPERATOR) {
        return parse_number();
    }
    PROCESS_ERROR(first, {lexem::SEMICOLON});
    print_errors();
    std::cerr << "Parser::parse_constant(): unexpected first symbol\n";
    throw std::exception();

}

uptr<AST::ConstString> Parser::parse_const_string()
{
    if (token != lexem::STRING) {
        PROCESS_ERROR(lexem::STRING, {lexem::SEMICOLON});
    }
    return std::make_unique<AST::ConstString>(token.image());
}


uptr<AST::Number> Parser::parse_number()
{
    int sign = 1;
    if (token == lexem::OPERATOR) {
        if (token.image() == std::string_view("-")) {
            sign = -1;
        }
        nextToken();
    }
    return std::make_unique<AST::Number>(sign, token.image());
}
