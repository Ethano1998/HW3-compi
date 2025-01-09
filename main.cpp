#include "SemanticVisitor.hpp"
#include "nodes.hpp"
#include <iostream>

extern int yyparse();

extern std::shared_ptr<ast::Node> program;

int main(){
    
    yyparse();

    SemanticVisitor Visitor;

    program->accept(Visitor);

    std::cout << Visitor.scopePrinter << std::endl;

}