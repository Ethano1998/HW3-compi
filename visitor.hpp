#ifndef VISITOR_HPP
#define VISITOR_HPP

#include <algorithm>
#include <memory>
#include <string>
#include <vector>
#include "symbolTable.hpp"

namespace ast {
    class Num;
    class NumB;
    class String;
    class Bool;
    class ID;
    class BinOp;
    class RelOp;
    class Not;
    class And;
    class Or;
    class Type;
    class Cast;
    class ExpList;
    class Call;
    class Statements;
    class Break;
    class Continue;
    class Return;
    class If;
    class While;
    class VarDecl;
    class Assign;
    class Formal;
    class Formals;
    class FuncDecl;
    class Funcs;
}

namespace output{
    class ScopePrinter;
}

class Visitor {
public:
    virtual void visit(ast::Num &node) = 0;

    virtual void visit(ast::NumB &node) = 0;

    virtual void visit(ast::String &node) = 0;

    virtual void visit(ast::Bool &node) = 0;

    virtual void visit(ast::ID &node) = 0;

    virtual void visit(ast::BinOp &node) = 0;

    virtual void visit(ast::RelOp &node) = 0;

    virtual void visit(ast::Not &node) = 0;

    virtual void visit(ast::And &node) = 0;

    virtual void visit(ast::Or &node) = 0;

    virtual void visit(ast::Type &node) = 0;

    virtual void visit(ast::Cast &node) = 0;

    virtual void visit(ast::ExpList &node) = 0;

    virtual void visit(ast::Call &node) = 0;

    virtual void visit(ast::Statements &node) = 0;

    virtual void visit(ast::Break &node) = 0;

    virtual void visit(ast::Continue &node) = 0;

    virtual void visit(ast::Return &node) = 0;

    virtual void visit(ast::If &node) = 0;

    virtual void visit(ast::While &node) = 0;

    virtual void visit(ast::VarDecl &node) = 0;

    virtual void visit(ast::Assign &node) = 0;

    virtual void visit(ast::Formal &node) = 0;

    virtual void visit(ast::Formals &node) = 0;

    virtual void visit(ast::FuncDecl &node) = 0;

    virtual void visit(ast::Funcs &node) = 0;
};

class SemanticVisitor : public Visitor {
private:
    /*
    std::vector<std::string> indents;
    std::vector<std::string> prefixes;

    /* Helper function to print a string with the current indentation 
    void print_indented(const std::string &str);

    /* Functions to manage the indentation level 
    void enter_child();

    void enter_last_child();

    void leave_child();
    */

public:

    bool from_funcdecl = false;

    enum class Context {
        DECLARATION,
        REFERENCE_FUNC,
        REFERENCE_VAR
    };

    bool declaration_function;

    Context current_context;

    output::ScopePrinter scopePrinter; 

    GlobalSymbolTable globalSymbolTable;

    bool is_loop = false;

    SemanticVisitor();

    void setContext(Context context){
        current_context = context;
    }

    std::string toString(ast::BuiltInType type) {
        switch (type) {
            case ast::BuiltInType::INT:
                return "int";
            case ast::BuiltInType::BOOL:
                return "bool";
            case ast::BuiltInType::BYTE:
                return "byte";
            case ast::BuiltInType::VOID:
                return "void";
            case ast::BuiltInType::STRING:
                return "string";
            default:
                return "unknown";
        }
    }

    ast::BuiltInType toBuiltInType(const std::string &type) {
        if (type == "int") {
            return ast::BuiltInType::INT;
        } else if (type == "bool") {
            return ast::BuiltInType::BOOL;
        } else if (type == "byte") {
            return ast::BuiltInType::BYTE;
        } else if (type == "void") {
            return ast::BuiltInType::VOID;
        } else if (type == "string") {
            return ast::BuiltInType::STRING;
        }
    }
    
    ast::BuiltInType check_assign(std::shared_ptr<ast::Exp> &node);

    void visit(ast::Num &node) override;

    void visit(ast::NumB &node) override;

    void visit(ast::String &node) override;

    void visit(ast::Bool &node) override;

    void visit(ast::ID &node) override;

    void visit(ast::BinOp &node) override;

    void visit(ast::RelOp &node) override;

    void visit(ast::Not &node) override;

    void visit(ast::And &node) override;

    void visit(ast::Or &node) override;

    void visit(ast::Type &node) override;

    void visit(ast::Cast &node) override;

    void visit(ast::ExpList &node) override;

    void visit(ast::Call &node) override;

    void visit(ast::Statements &node) override;

    void visit(ast::Break &node) override;

    void visit(ast::Continue &node) override;

    void visit(ast::Return &node) override;

    void visit(ast::If &node) override;

    void visit(ast::While &node) override;

    void visit(ast::VarDecl &node) override;

    void visit(ast::Assign &node) override;

    void visit(ast::Formal &node) override;

    void visit(ast::Formals &node) override;

    void visit(ast::FuncDecl &node) override;

    void visit(ast::Funcs &node) override;
};


#endif //VISITOR_HPP
