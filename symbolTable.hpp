#ifndef SYMBOLTABLE_HPP
#define SYMBOLTABLE_HPP

#include <vector>
#include <memory>
#include <string>


class SymbolEntry {
    public:
    virtual ~SymbolEntry() = default;
};

class VarSymbolEntry : public SymbolEntry {
public:
    std::string name;
    std::string type;
    int offset;
    VarSymbolEntry(std::string name, std::string type, int offset) : name(name), type(type), offset(offset) {}
};

class FuncSymbolEntry : public SymbolEntry {
public:
    std::string name;
    std::vector<std::string> paramTypes;
    std::string returnType;
    FuncSymbolEntry(std::string name,std::vector<std::string> paramTypes ,std::string returnType) : name(name), paramTypes(paramTypes), returnType(returnType) {}
};

class SymbolTable {
    int offset = 0;
    std::vector<std::shared_ptr<SymbolEntry>> entries;

    public:
    SymbolTable() = default;
    
    std::shared_ptr<SymbolEntry> findEntry(std::string name) {
        for (auto entry : entries) {
            if (std::dynamic_pointer_cast<VarSymbolEntry>(entry)) {
                if (std::dynamic_pointer_cast<VarSymbolEntry>(entry)->name == name) {
                    return entry;
                }
            } else if (std::dynamic_pointer_cast<FuncSymbolEntry>(entry)) {
                if (std::dynamic_pointer_cast<FuncSymbolEntry>(entry)->name == name) {
                    return entry;
                }
            }
        }
        return nullptr;
    }

    void addVar(std::string name, std::string type) {
        entries.push_back(std::make_shared<VarSymbolEntry>(name, type, offset));
        offset++;
    }

    void addFunc(std::string name, std::vector<std::string> paramTypes, std::string returnType) {
        entries.push_back(std::make_shared<FuncSymbolEntry>(name, paramTypes, returnType));
    }
};

class GlobalSymbolTable {
    std::vector<std::shared_ptr<SymbolTable>> tables;
    public:
    GlobalSymbolTable(){
        std::shared_ptr<SymbolTable> table = std::make_shared<SymbolTable>();
        table->addFunc("print", std::vector<std::string>{"string"}, "void");
        table->addFunc("printi", std::vector<std::string>{"int"}, "void");
        tables.push_back(table);
    };

    void addTable(std::shared_ptr<SymbolTable> table) {
        tables.push_back(table);
    }

    std::shared_ptr<SymbolTable> getTable() {
        return tables.back();
    }

    std::shared_ptr<SymbolTable> popTable() {
        auto table = tables.back();
        tables.pop_back();
        return table;
    }

    std::shared_ptr<SymbolEntry> findEntry(std::string name) {
        for (auto table : tables) {
            auto entry = table->findEntry(name);
            if (entry) {
                return entry;
            }
        }
        return nullptr;
    }
};



#endif //SYMBOLTABLE_HPP