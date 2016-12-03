#include "base/errors.hpp"
#include "ev/evaluator.hpp"

void falk::ev::evaluator::analyse(const declare_variable& var,
                                  node_array<1>& nodes) {
    auto get_value = [this](auto& mapper, auto& var) {
        auto type = aut::pop(types_stack);
        if (type != var.s_type) {
            err::semantic<Error::ILLEGAL_ASSIGNMENT>(var.s_type, type);
            return;
        }

        switch (type) {
            case structural::type::SCALAR: {
                auto result = aut::pop(scalar_stack);
                if (!result.error()) {
                    mapper.declare_variable(
                        var.id, variable(result, structural::type::SCALAR)
                    );
                }
                break;
            }
            case structural::type::ARRAY: {
                auto result = aut::pop(array_stack);
                if (!result.error()) {
                    mapper.declare_variable(
                        var.id, variable(result, structural::type::ARRAY)
                    );
                }
                break;
            }
            case structural::type::MATRIX: {
                auto result = aut::pop(matrix_stack);
                if (!result.error()) {
                    mapper.declare_variable(
                        var.id, variable(result, structural::type::MATRIX)
                    );
                }
                break;
            }
        }
    };

    if (!nodes[0]->empty()) {
        nodes[0]->traverse(*this);
        get_value(mapper, var);
    } else {
        mapper.declare_variable(var.id, variable(var.f_type));
    }
}

void falk::ev::evaluator::analyse(var_id& vid, node_array<2>& index) {
    // TODO: handle index
    // Assigned to Ghabriel
    push(vid);
}

void falk::ev::evaluator::analyse(const valueof&, node_array<1>& nodes) {
    nodes[0]->traverse(*this);

    // TODO: handle index
    // Assigned to Ghabriel
    auto vid = aut::pop(id_stack);
    auto& var = mapper.retrieve_variable(vid.id);
    
    switch (var.stored_type()) {
        case structural::type::SCALAR: {
    
            push(var.value<scalar>());
            break;
        }
        case structural::type::ARRAY: {
    
            push(var.value<array>());
            break;
        }
        case structural::type::MATRIX: {
    
            push(var.value<matrix>());
            break;
        }
    }
}

void falk::ev::evaluator::analyse(const block&, std::list<node_ptr>& nodes) {
    for (auto& node : nodes) {
        if (node) {
            node->traverse(*this);        
        }
    }
}

void falk::ev::evaluator::analyse(const conditional&, node_array<3>& nodes) {
    nodes[0]->traverse(*this);
    auto type = aut::pop(types_stack);
    if (type == structural::type::SCALAR) {
        auto result = aut::pop(scalar_stack);
        if (result.boolean()) {
            nodes[1]->traverse(*this);
        } else {
            nodes[2]->traverse(*this);
        }
    } else {
        err::semantic<Error::NON_BOOLEAN_CONDITION>();
    }
}

void falk::ev::evaluator::analyse(const loop& action, node_array<2>& nodes) {
    nodes[0]->traverse(*this);
    auto type = aut::pop(types_stack);
    if (type == structural::type::SCALAR) {
        // auto result = aut::pop(scalar_stack);
        // if (result.boolean()) {
        //     nodes[1]->traverse(*this);
        //     analyse(action, nodes);
        // }
        std::cout << "while loop" << std::endl;
    } else {
        err::semantic<Error::NON_BOOLEAN_CONDITION>();
    }
}


// void falk::ev::evaluator::analyse(const index_access&, node_array<3>& nodes) {
//     nodes[0]->traverse(*this);
//     auto type = aut::pop(types_stack);
//     switch (type) {
//         case structural::type::SCALAR:
//             err::semantic<Error::NOT_A_STRUCTURE>();
//             break;
//         case structural::type::ARRAY:
//             std::cout << "TODO: implement indexed access to arrays";
//             break;
//         case structural::type::MATRIX:
//             std::cout << "TODO: implement indexed access to matrices";
//             break;
//     }
// }

void falk::ev::evaluator::process(rvalue& v) {
    if (!v.empty()) {
        v.traverse(*this);
    }
    while (!types_stack.empty()) {
        print_result();
    }
}

void falk::ev::evaluator::print_result() {
    auto type = aut::pop(types_stack);
    switch (type) {
        case structural::type::SCALAR: {
            auto result = aut::pop(scalar_stack);
            if (!result.error()) {
                std::cout << "res = " << result << std::endl;
                mapper.update_result(variable(result, type));
            }
            break;
        }
        case structural::type::ARRAY: {
            auto result = aut::pop(array_stack);
            if (!result.error()) {
                std::cout << "res = " << result << std::endl;
                mapper.update_result(variable(result, type));
            }
            break;
        }
        case structural::type::MATRIX: {
            auto result = aut::pop(matrix_stack);
            if (!result.error()) {
                std::cout << "res = " << result << std::endl;
                mapper.update_result(variable(result, type));
            }
            break;
        }
    }
}

falk::ev::array& falk::ev::evaluator::extract(array& arr, rvalue& v) {
    v.traverse(*this);
    if (aut::pop(types_stack) == structural::type::SCALAR) {
        auto scalar = aut::pop(scalar_stack);
        arr.push_back(scalar);
    } else {
        // TODO: error
    }
    return arr;
}
