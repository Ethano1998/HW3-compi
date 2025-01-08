#include "visitor.hpp"

extern int yyparse();

extern std::shared_ptr<ast::Node> program;

main(){
    
    yyparse();

    SemanticVisitor Visitor;

    program->accept(Visitor);

}