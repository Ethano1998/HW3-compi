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
        parameters_name.push_back(toString(params->type->type));
        }

        //casting du return type en string
        std::string returnType = toString(node.return_type->type);
     

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
    from_funcdecl = true;

    //route du visitor vers statement pour la scope de la function
    node.formals->accept(*this);
    node.body->accept(*this);

    //supression de la table et fin du scope de la function
    globalSymbolTable.popTable();
    scopePrinter.endScope();
    }
}

void SemanticVisitor::visit(ast::Type &node){
    //verification du type
    auto check = toString(node.type);
    if(check == "unknown")
        output::errorMismatch(node.line);
}

void SemanticVisitor::visit(ast::ID &node){
    switch(current_context){
        case Context::DECLARATION : {

            //verification si la function est deja declaree
            if(globalSymbolTable.findEntry(node.value))
                output::errorDef(node.line,node.value);
            break;
        }
        case Context::REFERENCE_FUNC :{
            //recuperation de la table dans symbol table
            std::shared_ptr<SymbolTable> Table = globalSymbolTable.getTable();

            //verification si la function est declaree en tant que variable
            if(std::dynamic_pointer_cast<VarSymbolEntry>(Table->findEntry(node.value)))
                output::errorDefAsVar(node.line,node.value);

            //recuperation de la table des functions
            std::shared_ptr<SymbolTable> functionTable = globalSymbolTable.getFunctionTable();
            
            //verification si la function est declaree
            if(!(functionTable->findEntry(node.value)))
                output::errorUndefFunc(node.line,node.value);
            break;
        }
        case Context::REFERENCE_VAR :{
            //recuperation de la table des functions
            std::shared_ptr<SymbolTable> functionTable = globalSymbolTable.getFunctionTable();
            
            //verification si la variable est declaree en tant que fonction
            if(std::dynamic_pointer_cast<FuncSymbolEntry>(functionTable->findEntry(node.value)))
                output::errorDefAsFunc(node.line,node.value);

            //recuperation de la table dans symbol table
            std::shared_ptr<SymbolTable> Table = globalSymbolTable.getTable();

            //verification si variable est declaree
            if(!(Table->findEntry(node.value)))
                output::errorUndef(node.line,node.value);
            break;    
        }       
    }
}

void SemanticVisitor::visit(ast::Formals &node){
    if(declaration_function){
        for(const auto &formal : node.formals){
            std::string doublon = formal->id->value;
            for(const auto &check : node.formals){
                if(check->id->value == doublon && check != formal)
                    output::errorDef(check->id->line,doublon);
            }
            formal->type->accept(*this);
        }
    }
    else{
        for(const auto &formal : node.formals){
            formal->accept(*this);
        }
    }
}

void SemanticVisitor::visit(ast::Formal &node){
    auto table = globalSymbolTable.getTable();
    table->addParam(node.id->value,toString(node.type->type));
    scopePrinter.emitVar(node.id->value,node.type->type,table->param_offset);
}

void SemanticVisitor::visit(ast::Statements &node){
    if(from_funcdecl){
        from_funcdecl = false;
        for(const auto &statement : node.statements){
            statement->accept(*this);
        }
    } else{
        scopePrinter.beginScope();
        for(const auto &statement : node.statements){
            statement->accept(*this);
        }
        scopePrinter.endScope();
    }    
}

void SemanticVisitor::visit(ast::VarDecl &node){
    current_context = Context::DECLARATION;
    node.id->accept(*this);
    node.type->accept(*this);
    



}