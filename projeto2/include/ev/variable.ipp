
inline falk::ev::variable::variable(falk::type t):
  data{scalar(t)} {}

inline falk::ev::variable::variable(bool flag):
  data{scalar(falk::type())}, fail{flag} {
    value<scalar>().set_error();
}

template<typename T>
falk::ev::variable::variable(const T& value, structural::type t):
  data{value}, type{t} { }

inline falk::structural::type falk::ev::variable::stored_type() const {
    return type;
}

template<typename T>
T& falk::ev::variable::value() {
    return data.value<T>();
}

inline bool falk::ev::variable::error() const {
    return fail;
}

template<typename Type, Type OP, typename T>
falk::ev::variable& falk::ev::variable::op(op::callback<Type, OP, 2> op,
                                           const T& v) {
    switch (type) {
        case falk::structural::type::SCALAR: {
            auto& raw = data.value<scalar>();
            std::cout << "(variable::op) scalar" << std::endl;
            std::cout << "(variable::op) lhs value = " << raw << std::endl;
            std::cout << "(variable::op) rhs value = " << v << std::endl;
            op(raw, v);
            std::cout << "(variable::op) ok" << std::endl;
            break;
        }
        case falk::structural::type::ARRAY: {
            auto& raw = data.value<array>();
            op(raw, v);
            break;
        }
        case falk::structural::type::MATRIX: {
            auto& raw = data.value<matrix>();
            op(raw, v);
            break;
        }
    }

    return *this;
}

template<typename T>
falk::ev::variable& falk::ev::variable::pow(const T& rhs) {
    return op(op::callback<op::assignment, op::assignment::POW, 2>(), rhs);
}

template<typename T>
falk::ev::variable& falk::ev::variable::operator+=(const T& rhs) {
    return op(op::callback<op::assignment, op::assignment::ADD, 2>(), rhs);
}

template<typename T>
falk::ev::variable& falk::ev::variable::operator-=(const T& rhs) {
    return op(op::callback<op::assignment, op::assignment::SUB, 2>(), rhs);
}

template<typename T>
falk::ev::variable& falk::ev::variable::operator*=(const T& rhs) {
    return op(op::callback<op::assignment, op::assignment::MULT, 2>(), rhs);
}

template<typename T>
falk::ev::variable& falk::ev::variable::operator/=(const T& rhs) {
    return op(op::callback<op::assignment, op::assignment::DIV, 2>(), rhs);
}

template<typename T>
falk::ev::variable& falk::ev::variable::operator%=(const T& rhs) {
    return op(op::callback<op::assignment, op::assignment::MOD, 2>(), rhs);
}

template<typename T>
falk::ev::variable& falk::ev::variable::operator&=(const T& rhs) {
    return op(falk::op::callback<op::logic,op::logic::AND, 2>(), rhs);
}

template<typename T>
falk::ev::variable& falk::ev::variable::operator|=(const T& rhs) {
    return op(falk::op::callback<op::logic,op::logic::AND, 2>(), rhs);
}

template<typename T>
falk::ev::variable& falk::ev::variable::operator=(const T& rhs) {
    return op(falk::op::callback<op::assignment,op::assignment::DIRECT, 2>(), rhs);
}
