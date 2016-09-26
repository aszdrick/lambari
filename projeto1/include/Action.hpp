#ifndef ACTION_HPP
#define ACTION_HPP

#include <list>
#include <vector>
#include "SymbolMap.hpp"

namespace {
    auto& symbols = SymbolMap::instance();
}

class Action {
 public:
    virtual ~Action() {}
    virtual std::string to_string(unsigned = 0) const = 0;
    virtual bool error() const { return false; }
    virtual Type type() const = 0;
};


class Declaration : public Action {
 public:
    Declaration(Type);
    void add(const std::string&);
    void add(const std::string&, Action*);
    std::string to_string(unsigned = 0) const override;
    Type type() const override;

 private:
    Type t;
    std::vector<std::pair<std::string, std::string>> values;
};


class Variable : public Action {
 public:
    Variable(const std::string&);
    bool error() const override;
    Type type() const override;
    std::string to_string(unsigned = 0) const override;

 private:
    bool fail;
    Type t;
    std::string name;
};


class Constant : public Action {
 public:
    Constant(Type type, const std::string& value);
    bool error() const override;
    Type type() const override;
    std::string to_string(unsigned = 0) const override;

 private:
    Type t;
    std::string value;
};


class Operation : public Action {
 public:
    template<typename... Args>
    Operation(Operator op, Action* first, Args&&... args) : op(op) {
        t = first->type();
        set_children(first, std::forward<Args>(args)...);
    }

    template<typename... Args>
    Operation(Operator op, Type type, Args&&... args)
     : op(op), t(type) {
        set_children(std::forward<Args>(args)...);
    }

    bool error() const override;
    Type type() const override;
    std::string to_string(unsigned = 0) const override;
    virtual std::string op_string() const;
    void set_type(Type);

 private:
    Operator op;
    Type t;
    bool fail = false;
    bool needs_coercion = false;
    std::list<Action*> children;

    void check(Action*);
    void set_children();

    template<typename... Args>
    void set_children(Action* action, Args&&... args) {
        check(action);
        fail = fail || action->error();
        children.push_back(action);
        set_children(std::forward<Args>(args)...);
    }
};


class Comparison : public Operation {
 public:
    template<typename... Args>
    Comparison(Operator op, Args&&... args)
     : Operation(op, std::forward<Args>(args)...) {
        set_type(Type::BOOL);
    }
};

class BoolOperation : public Operation {
 public:
    template<typename... Args>
    BoolOperation(Operator op, Args&&... args)
     : Operation(op, Type::BOOL, std::forward<Args>(args)...) {}
};

class Parenthesis : public Operation {
 public:
    Parenthesis(Action* operand) : Operation(Operator::PAR, operand) {}

    std::string op_string() const override {
        return "";
    }
};

class UnaryMinus : public Operation {
 public:
    UnaryMinus(Action* operand) : Operation(Operator::UNARY_MINUS, operand) {}

    std::string op_string() const override {
        return "-u";
    }
};

class Cast : public Operation {
 public:
    Cast(Type type, Action* operand) : Operation(Operator::CAST, operand) {
        set_type(type);
    }

    std::string op_string() const override {
        return "[" + utils::to_string(type()) + "]";
    }
};

class Assignment : public Action {
 public:
    Assignment(Action*, Action*);
    std::string to_string(unsigned = 0) const override;
    Type type() const override;

 private:
    Action* var;
    Action* rhs;
    bool fail;
};

#endif /* ACTION_HPP */
