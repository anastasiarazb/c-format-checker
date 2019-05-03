#include "ast/ast.hpp"

using namespace AST;

std::string spaces(int n)
{
    return std::string(n, ' ');
}

/* ************************** Program ************************* */

Program::Program(std::string_view name,
               uptr_list<Identifier> &&parameter_list,
               uptr<Block> &&block)
    : name(name),
      parameter_list(std::move(parameter_list)),
      block(std::move(block))
{
    /* empty */
}

void Program::print() const {
    std::cout << "Program "<< name << "\n  parameter_list:" << std::endl;
    for (const uptr<Identifier> &val : parameter_list) {
        val->print(1);
    }
    std::cout << "  block:" << std::endl;
    if (block) {
        block->print();
    }
}

/* ************************** Identifier ************************* */

void Identifier::print(int bias) const {
    for (int i = 0; i < bias; ++i) printf(" ");
    std::cout << "Identifier " << identifier << std::endl;
}

/* ************************** Block ************************* */

Block::Block(uptr_list<TypeDef> &&type_def_part,
             uptr_list<VarDecl> &&variable_decl_part,
             uptr<Expr> &&proc_and_func_decl_part,
             uptr<CompoundStatement> &&statement_part)
    :
      type_def_part(std::move(type_def_part)),
      variable_decl_part(std::move(variable_decl_part)),
      proc_and_func_decl_part(std::move(proc_and_func_decl_part)),
      statement_part(std::move(statement_part))
{
    /* empty */
}

void Block::print(int bias) const {
    for (int i = 0; i < bias; ++i) printf(" ");
    std::cout << "Block "<< std::endl;
    std::cout << " Typedefpart: "<< std::endl;
    for (const uptr<TypeDef> &val : type_def_part) {
        val->print(bias + 2);
    }
    std::cout << " Vardeclpart: "<< std::endl;
    for (const uptr<VarDecl> &val : variable_decl_part) {
        val->print(bias + 2);
    }
    std::cout << " Procedures and functions: "<< std::endl;
    if (proc_and_func_decl_part) {
        proc_and_func_decl_part->print(bias + 2);
    }
    std::cout << " Statement: "<< std::endl;
    if (statement_part) {
        statement_part->print(bias + 2);
    }
}

/* ************************** TypeDef ************************* */

TypeDef::TypeDef(std::string_view identifier, uptr<TypeDenoter> &&type_denoter)
    : identifier(identifier), type_denoter(std::move(type_denoter))
{
    /* empty */
}

void TypeDef::print(int bias) const {
    for (int i = 0; i < bias; ++i) printf(" ");
    std::cout << "TypeDef "<< identifier ;
    if (type_denoter) {
        std::cout << std::endl;
        type_denoter->print(bias + 1);
    } else {
        std::cout << ", type_denoter = <empty>" << std::endl;
    }
}

TypeDenoter::TypeDenoter(std::string_view identifier)
    : identifier(identifier)
{
    /* empty */
}

void TypeDenoter::print(int bias) const {
    for (int i = 0; i < bias; ++i) printf(" ");
    std::cout << "TypeDenoter " << identifier << std::endl;
}
/* ************************** VarDeclPart ************************* */

VarDecl::VarDecl(uptr_list<Identifier> &&identifier_list, uptr<TypeDenoter> &&type_denoter)
    : identifier_list(std::move(identifier_list))
    , type_denoter(std::move(type_denoter))
{
    /* empty */
}

void VarDecl::print(int bias) const {
    Expr::print(bias);
//    for (int i = 0; i < bias; ++i) printf(" ");
    std::cout << "VarDecl: "<< std::endl;
    for (const uptr<Identifier> &id: identifier_list) {
        id->print(bias + 1);
    }
    if (type_denoter) {
        type_denoter->print(bias + 1);
    } else {
        std::cout << "type_denoter = <empty>" << std::endl;
    }

}


/* Statements */

//const std::vector<lexem::Type> StructuredStatement::first_lexems = {
//    lexem::BEGIN, /* CompoundStatement*/
//    lexem::CASE, lexem::IF, //ConditionalStatement
//    lexem::FOR, lexem::REPEAT, lexem::WHILE, //RepetitiveStatement
//    lexem::WITH // WithStatement
//};

CompoundStatement::CompoundStatement(uptr_list<Statement> &&statement_sequence)
    : stmt_list(std::move(statement_sequence))
{
    /* empty */
}

ProcedureStatement::ProcedureStatement(std::string_view identifier,
                   uptr_list<Expr>  &&parameter_list)
    : identifier(identifier)
    , parameter_list(std::move(parameter_list))
{
    /* empty */
}

void ProcedureStatement::print(int bias) const {
    Expr::print(bias);
    std::cout << "ProcedureStatement " << identifier << std::endl;
    for (const uptr<Expr> &val : parameter_list) {
        val->print(bias+1);
    }
}

ForWhile::ForWhile(uptr<Expr> &&start, uptr<Expr> &&end
                       , uptr<Expr> &&step, uptr<Expr> &&body)
    :start(std::move(start)), end(std::move(end)), step(std::move(step)), body(std::move(body))
{
    /* empty */
}

void ForWhile::print(int bias) const {
    Expr::print(bias);
    std::cout << "ForWhile " << std::endl;
    if (start) {
        Expr::print(bias);
        std::cout << "start ";
        start->print(0);
    }
    if (start) {
        Expr::print(bias);
        std::cout << "start ";
        start->print(0);
    }
}


Assignment::Assignment(uptr<Expr> &&lhs, uptr<Expr> &&rhs)
    : lhs(std::move(lhs)), rhs(std::move(rhs))
{
    /* empty */
}

void Assignment::print(int bias) const {
    Expr::print(bias);
    std::cout << "Assignment " << std:: endl;
    Expr::print(bias);
    std::cout << "LHS ";
    if (lhs) {
        Expr::print(bias);
        std::cout << "LHS ";
        lhs->print(0);
    }
    if (rhs) {
        Expr::print(bias);
        std::cout << "RHS ";
        rhs->print(0);
    }
}

void Statement::print(int bias) const {
    Expr::print(bias);
    std::cout << "Statement " << std:: endl;
    Expr::print(bias);
}

void CompoundStatement::print(int bias) const {
    Expr::print(bias);
    std::cout << "CompoundStatement " << std:: endl;
    Expr::print(bias);
    for (const uptr<Statement> &val : stmt_list) {
        val->print(bias+1);
    }
}


/* ----------------------Constants----------------------- */

ConstDecl::ConstDecl(std::string_view identifier, uptr<TypeDenoter> &&constant)
    :identifier(identifier), constant(std::move(constant))
{
    /* empty */
}

void ConstDecl::print(int bias) const
{
    std::cout << spaces(bias) << "ConstDecl: " << identifier << " = ";
    constant->print();

}

Number::Number(int sign, const std::string_view &image)
    : sign(sign)
{
    for (char ch: image) {
        if (ch == '.' || ch == 'e' || ch == 'E') {
            type = Type::REAL;
            fval = std::stof (image.data());
            return;
        }
    }
    type = Type::INTEGER;
    ival = std::stoi (image.data());
}

void Number::print(int bias) const {
    for (int i = 0; i < bias; ++i) printf(" ");
    std::cout << "Number " << (type == Type::INTEGER ? "int" : "real") << std::endl;
}


ConstString::ConstString(std::string_view value)
    : value(value)
{
    /* empty */
}

void ConstString::print(int bias) const
{
    std::cout << spaces(bias) << "ConstString " << value;
}
