#include "visitor.hpp"


void SemanticVisitor::visit(ast::Funcs &node){
    //appel du visitor sur toute les functions dans lordre
    for (const auto &func : node.funcs){
        func->accept(*this);
    }
}

void SemanticVisitor::visit(ast::FuncDecl &node){
    //debut de la scope de la function
    scopePrinter.beginScope();

    //declaration des vecteurs pour les parametres
    std::vector<ast::BuiltInType> parameters;
    std::vector<std::string> parameters_name;

    //casting des parametres en string 
    for(const auto &params : node.formals->formals ){
       parameters.push_back(params->type->type);
       switch(params->type->type){
            case ast::BuiltInType::VOID : parameters_name.push_back("void"); break;
            case ast::BuiltInType::BOOL : parameters_name.push_back("bool"); break;
            case ast::BuiltInType::BYTE : parameters_name.push_back("byte"); break;
            case ast::BuiltInType::INT  : parameters_name.push_back("int"); break;
            case ast::BuiltInType::STRING : parameters_name.push_back("string"); break;
            default : parameters_name.push_back("unknown"); break;
       }
       
    }

    //casting du return type en string
    std::string returnType;
    switch (node.return_type->type)
    {
        case ast::BuiltInType::VOID : returnType = "void"; break;
        case ast::BuiltInType::BOOL : returnType = "bool"; break;
        case ast::BuiltInType::BYTE : returnType = "byte"; break;
        case ast::BuiltInType::INT  : returnType = "int"; break;
        case ast::BuiltInType::STRING : returnType = "string"; break;
        default : returnType = "unknown"; break;
    }

    //emission de la function dans le scope global
    scopePrinter.emitFunc(node.id->value,node.return_type->type,parameters);

    //recuperation de la table des functions
    std::shared_ptr<SymbolTable> functable = globalSymbolTable.getTable();

    //verification si la function est declaree
    if(functable->findEntry(node.id->value) != nullptr){
        output::errorDef(node.line,node.id->value);
    }

    //ajout de la function a la symboltable des functions 
    functable->addFunc(node.id->value,parameters_name,returnType);

    //creation de la table pour le scope de la function
    std::shared_ptr<SymbolTable> func_table = std::make_shared<SymbolTable>() ;
    globalSymbolTable.addTable(func_table);

    //continuation du SemanticVisitor
    node.return_type->accept(*this);
    node.id->accept(*this);
    node.formals->accept(*this);
    node.body->accept(*this);

    //supression de la table et fin du scope de la function
    globalSymbolTable.popTable();
    scopePrinter.endScope();
}

void SemanticVisitor::visit(ast::Type &node){
    //verification du type
    switch(node.type){
        case ast::BuiltInType::VOID : 
        case ast::BuiltInType::BOOL : 
        case ast::BuiltInType::BYTE : 
        case ast::BuiltInType::INT  : 
        case ast::BuiltInType::STRING :
            break; 
        default:
            output::errorMismatch(node.line);
        break;        
    }
}

void SemanticVisitor::visit(ast::ID &node){
    
}