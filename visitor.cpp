#include "visitor.hpp"


void SemanticVisitor::visit(ast::Funcs &node){
    for (const auto &func : node.funcs){
        func->accept(*this);
    }
}

void SemanticVisitor::visit(ast::FuncDecl &node){
    scopePrinter.beginScope();
    std::vector<ast::BuiltInType> parameters;
    std::vector<std::string> parameters_name; 
    for(const auto &params : node.formals->formals ){
       parameters.push_back(params->type->type);
       switch(params->type->type){
            case ast::BuiltInType::VOID : parameters_name.push_back("void");
            case ast::BuiltInType::BOOL : parameters_name.push_back("bool");
            case ast::BuiltInType::BYTE : parameters_name.push_back("byte");
            case ast::BuiltInType::INT  : parameters_name.push_back("int");
            case ast::BuiltInType::STRING : parameters_name.push_back("string");
       }
       
    }
    std::string returnType;
    switch (node.return_type->type)
    {
        case ast::BuiltInType::VOID : returnType = "void";
        case ast::BuiltInType::BOOL : returnType = "bool";
        case ast::BuiltInType::BYTE : returnType = "byte";
        case ast::BuiltInType::INT  : returnType = "int";
        case ast::BuiltInType::STRING : returnType = "string";
    }
    scopePrinter.emitFunc(node.id->value,node.return_type->type,parameters);
    std::shared_ptr<SymbolTable> functable = globalSymbolTable.getTable();
    if(functable->findEntry(node.id->value) != nullptr){
        output::errorDef(node.line,node.id->value);
    }
    functable->addFunc(node.id->value,parameters_name,returnType);
    std::shared_ptr<SymbolTable> func_table = std::make_shared<SymbolTable>() ;
    globalSymbolTable.addTable(func_table);
    node.return_type->accept(*this);
    node.id->accept(*this);
    node.formals->accept(*this);
    node.body->accept(*this);
    globalSymbolTable.popTable();
    scopePrinter.endScope();
}