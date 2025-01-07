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
    
    public:
    std::vector<std::shared_ptr<SymbolEntry>> entries;
    SymbolTable() = default;
    void addEntry(std::shared_ptr<SymbolEntry> entry) {
        entries.push_back(entry);
    }
    std::shared_ptr<SymbolEntry> findEntry(std::string name) {
        for (auto entry : entries) {
            if (dynamic_cast<VarSymbolEntry*>(entry.get())) {
                if (dynamic_cast<VarSymbolEntry*>(entry.get())->name == name) {
                    return entry;
                }
            } else if (dynamic_cast<FuncSymbolEntry*>(entry.get())) {
                if (dynamic_cast<FuncSymbolEntry*>(entry.get())->name == name) {
                    return entry;
                }
            }
        }
        return nullptr;
    }
};



#endif //SYMBOLTABLE_HPP