#ifndef AST_HPP
#define AST_HPP
#include "stdint.h"
//#include "llvm/ADT/APInt.h"
//#include "llvm/ADT/STLExtras.h"
//#include "llvm/IR/BasicBlock.h"
//#include "llvm/IR/Constants.h"
//#include "llvm/IR/DerivedTypes.h"
//#include "llvm/IR/Function.h"
//#include "llvm/IR/Instructions.h"
//#include "llvm/IR/IRBuilder.h"
//#include "llvm/IR/LLVMContext.h"
//#include "llvm/IR/LegacyPassManager.h"
//#include "llvm/IR/Module.h"
//#include "llvm/IR/Type.h"
//#include "llvm/IR/Verifier.h"
//#include "llvm/Support/TargetSelect.h"
//#include "llvm/Target/TargetMachine.h"
//#include "llvm/Transforms/Scalar.h"
#include <iostream>
#include <memory>
#include <cassert>
#include <map>
#include "frontend/lexer/scanner.hpp"

namespace compiler {
//extern llvm::Module *Module;
//extern llvm::Function *MainFunction;
//extern std::map<std::string, llvm::AllocaInst *> VarToSpace;
//extern llvm::LLVMContext localContext;
//extern llvm::IRBuilder<> localBuilder;
} // namespace compiler

// http://llvm.org/docs/tutorial/LangImpl02.html
template <typename T>
using uptr = std::unique_ptr<T>;
template <typename T>
using uptr_list = std::vector<uptr<T>>;

namespace AST {
class Expr {
public:
    virtual ~Expr() {}
    virtual void print(int bias = 0) const {for (int i = 0; i < bias; ++i) printf(" ");}
//    virtual llvm::Value *codegen() = 0;
};

class Program;
class Identifier;
class Block;
class TypeDef;
class VarDecl;
class CompoundStatement;

class Statement;

class TypeDenoter;
class Constant;
class Number;
class ConstString;

class Program : public Expr {
public:
    const std::string_view name;
    Program(std::string_view name,
            uptr_list<Identifier> &&parameter_list,
            uptr<Block> &&block);
    //    virtual llvm::Value *codegen() = 0;
    virtual void print() const;
private:
    uptr_list<Identifier> parameter_list;
    uptr<Block>           block;
};

class Identifier : public Expr {
public:
    const std::string_view identifier;
    Identifier(std::string_view identifier): identifier(identifier) {}
    virtual void print(int bias = 0) const;
    //    virtual llvm::Value *codegen() = 0;
};

/* 6.2.1 blocks
block = label-declaration-part constant-definition-part type-definition-part
variable-declaration-part procedure-and-function-declaration-part
statement-part .
х label-declaration-part = [ `label' label { ',' label } ';' ] .
х constant-definition-part = [ `const' constant-definition ` ;' { constant-definition ';' } ] .
type-definition-part = [ `type' type-definition `;' { type-definition `;' } ] .
variable-declaration-part = [ `var' variable-declaration `;' { variable-declaration `;'} ] .
procedure-and-function-declaration-part = { ( procedure-declaration | function-declaration ) ` ;' } .

statement-part = compound-statement .
*/

class Block : public Expr {
    uptr_list<TypeDef> type_def_part;
    uptr_list<VarDecl> variable_decl_part;
    uptr<Expr> proc_and_func_decl_part;
    uptr<CompoundStatement> statement_part;

public:
    Block(uptr_list<TypeDef> &&type_def_part,
          uptr_list<VarDecl> &&variable_decl_part,
          uptr<Expr> &&proc_and_func_decl_part,
          uptr<CompoundStatement> &&statement_part);
    virtual void print(int bias = 0) const;
    //    virtual llvm::Value *codegen() = 0;

};

/* 6.4 Type-definitions
type-definition = identifier `=' type-denoter .
type-denoter = type-identifier | new-type .
new-type = new-ordinal-type | new-structured-type | new-pointer-type
*/

class TypeDef : public Expr {
public:
    const std::string_view identifier;
    uptr<TypeDenoter>       type_denoter;
    TypeDef(std::string_view identifier, uptr<TypeDenoter> &&type_denoter);
    virtual void print(int bias = 0) const;
    //    virtual llvm::Value *codegen() = 0;
};

class TypeDenoter : public Expr {
    const std::string_view identifier;
public:
    TypeDenoter(std::string_view identifier);
    virtual void print(int bias = 0) const;
};

class VarDecl : public Expr {
//variable-declaration = identifier-list ` :' type-denoter
    uptr_list<Identifier>  identifier_list;
    uptr<TypeDenoter>      type_denoter;
public:
    VarDecl(uptr_list<Identifier> &&identifier_list, uptr<TypeDenoter> &&type_denoter);
    virtual void print(int bias = 0) const;
};

//type-denoter = type-identifier | new-type

/* ************************* Statements ****************** */

class Statement : public Expr {
public:
    virtual void print(int bias = 0) const;
};

class CompoundStatement : public Expr {
    uptr_list<Statement> stmt_list;
public:
    CompoundStatement(uptr_list<Statement> &&stmt_list);
    virtual void print(int bias = 0) const;
};


class ForWhile : public Statement {
    uptr<Expr> start, end, step, body;
public:
    ForWhile(uptr<Expr> &&start, uptr<Expr> &&end, uptr<Expr> &&step, uptr<Expr> &&body);
    virtual void print(int bais = 0) const;
};

class Assignment : public Statement {
    //assignment-statement = ( variable-access | function-identifier ) ` :=' expression .
    uptr<Expr> lhs;
    uptr<Expr> rhs;
public:
    Assignment(uptr<Expr> &&lhs, uptr<Expr> &&rhs);
    virtual void print(int bias = 0) const;
};


class ProcedureStatement : public Statement {
/* procedure-statement = procedure-identifier ( [ actual-parameter-list ]
    | read-parameter-list  | readln-parameter-list
    | write-parameter-list | writeln-parameter-list ) .
*/
    std::string_view identifier;
    uptr_list<Expr>   parameter_list;
public:
    ProcedureStatement(std::string_view identifier,
                       uptr_list<Expr> &&parameter_list);
    virtual void print(int bias = 0) const;
};

/* ************************* Types ****************** */

class Constant : public Expr {
public:
    virtual void print(int bias = 0) const = 0;
};

class Number : public Constant {
public:
    enum class Type {
        INTEGER, REAL
    };
    Number(int sign, const std::string_view &image);
    int   int_val()  { return ival; }
    float real_val() { return fval; }
    virtual void print(int bias = 0) const;
//    virtual llvm::Value *codegen() override final;
private:
    Type type;
    union {
        float fval;
        int ival;
    };
    int sign;
};

class ConstString : public Constant {
    std::string_view value;
public:
    ConstString(std::string_view value);
    virtual void print(int bias = 0) const;
};

class ConstDecl : public Expr {
    const std::string_view identifier;
    uptr<TypeDenoter>       constant;
public:
    ConstDecl(std::string_view identifier, uptr<TypeDenoter> &&constant);
    virtual void print(int bias = 0) const;
};

class StructDecl : public Statement {
public:
    StructDecl(std::string &&name, std::map<std::string, Expr> &&fields);
//    virtual llvm::Value *codegen() = 0;
    std::string name;
    std::map<std::string, Expr> fields;
};

//class ReturnVal : public Expr {
//    std::unique_ptr<Expr> return_val;
////    virtual llvm::Value *codegen() override final;
//public:
//    ReturnVal(std::unique_ptr<Expr> return_val)
//        : return_val(std::move(return_val)) {}
//};

//class BinaryOp : public Expr {
//public:
////    llvm::Value *codegen();
//    BinaryOp(char operationCode,
//             std::unique_ptr<Expr> LHS, std::unique_ptr<Expr> RHS) :
//        opcode(operationCode), LHS(std::move(LHS)), RHS(std::move(RHS)) {}
//    char getOp() {return opcode;}
//private:
//    char opcode;
//    std::unique_ptr<Expr> LHS, RHS;
//};

//class AssignExpr : public Expr {
//    std::string varname;
//    std::unique_ptr<Expr> rhs;
//public:
//    AssignExpr(std::string &&varname, std::unique_ptr<Expr> rhs):
//        varname(varname), rhs(std::move(rhs)) {}
//    std::string const& getVarname() {return varname;}
////    virtual llvm::Value *codegen() override final;
//};

//class Var : public Expr {
//    std::string name;
//public:
////    virtual llvm::Value *codegen() override final;
//    const std::string &getName() const { return name; }
//    Var(std::string &&name) : name(name) {}
//    Var(std::string name)   : name(std::move(name)) {}
//};

////Прототип функции (имя и аргументы) - возвращает не Value, а Function,
////это связующее звено между именем функции и представалением в LLVM
//class Prototype {
//    std::string name;
//public:
////    llvm::Function *codegen();
//    const std::string &getName() const { return name; }
//    Prototype(std::string &&name) : name(std::move(name)) {}
//    ~Prototype(){}
//};

//class Function : public Expr {
//public:
//    Function(std::unique_ptr<Prototype> protototype, std::unique_ptr<Expr> body) :
//        protototype(std::move(protototype)), body(std::move(body)) {}
////    virtual llvm::Value *codegen() override final;
//private:
//    std::unique_ptr<Prototype> protototype;
//    std::unique_ptr<Expr> body;
//};

//class Body : public Expr {
//public:
//    Body(std::vector<std::unique_ptr<Expr>> body):
//        body(std::move(body)) {}
//    size_t getSize() {return body.size();}
////    virtual llvm::Value *codegen() override final;
//private:
//    std::vector<std::unique_ptr<Expr>> body;
//};

//// Класс узла выражения для if/then/else. https://habrahabr.ru/post/120881/gimple_assign_rhs_code
//class IfExpr : public Expr
//{
//    std::unique_ptr<Expr> cond, if_body, else_body;
//public:
//    IfExpr(std::unique_ptr<Expr> cond,
//           std::unique_ptr<Expr> if_body,
//           std::unique_ptr<Expr> else_body)
//        : cond(std::move(cond)), if_body(std::move(if_body)), else_body(std::move(else_body)) {}
////    virtual llvm::Value *codegen() override final;
//};

//class LoopExpr : public Expr
//{
//    std::unique_ptr<Expr> cond;
//    std::unique_ptr<Body> body;

//public:
//    LoopExpr(std::unique_ptr<Expr> cond,
//             std::unique_ptr<Body> body)
//        : cond(std::move(cond)), body(std::move(body)){}
////    virtual llvm::Value *codegen() override final;
//};

} // namespace AST
#endif // AST_HPP
