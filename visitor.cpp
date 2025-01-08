#include "visitor.hpp"


void SemanticVisitor::visit(ast::Funcs &node){
    //appel du visitor sur toute les functions pour les declarer
    declaration_function = true;
    for (const auto &func : node.funcs){
        func->accept(*this);
    }

    //continuation de la verification semantique apres avoir declaree toute les fonctions
    declaration_function = false;
    for (const auto &func : node.funcs){
        func->accept(*this);
    }
}

void SemanticVisitor::visit(ast::FuncDecl &node){
    if(declaration_function){


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

        //definition du contexte
        setContext(Context::DECLARATION);

        //continuation du SemanticVisitor pour la verification de la function
        node.return_type->accept(*this);
        node.id->accept(*this);
        node.formals->accept(*this);

        //recuperation de la table des functions
        std::shared_ptr<SymbolTable> functable = globalSymbolTable.getTable();    

        //ajout de la function a la symboltable des functions 
        functable->addFunc(node.id->value,parameters_name,returnType);

        //emission de la function dans le scope global
        scopePrinter.emitFunc(node.id->value,node.return_type->type,parameters);

    }
    else {
    //debut de la scope de la function
    scopePrinter.beginScope();

    //creation de la table pour le scope de la function
    std::shared_ptr<SymbolTable> func_table = std::make_shared<SymbolTable>() ;
    globalSymbolTable.addTable(func_table);

    //route du visitor vers statement pour la scope de la function
    node.body->accept(*this);

    //supression de la table et fin du scope de la function
    globalSymbolTable.popTable();
    scopePrinter.endScope();
    }
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
    switch(current_context){
        case Context::DECLARATION :
            //recuperation de la table dans symbol table
            std::shared_ptr<SymbolTable> Table = globalSymbolTable.getTable();

            //verification si la function est deja declaree
            if(Table->findEntry(node.value))
                output::errorDef(node.line,node.value);
            break;

        case Context::REFERENCE_FUNC :
            //recuperation de la table des functions
            std::shared_ptr<SymbolTable> functionTable = globalSymbolTable.getFunctionTable();
            
            //verification si la function est declaree
            if(!functionTable)
                output::errorUndefFunc(node.line,node.value);

            //verification si on utilise un ID de variable pour la function
            if(functionTable)   
            
    }
    
}