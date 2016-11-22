
#ifndef LPI_LPA_CONTEXT_HPP
#define LPI_LPA_CONTEXT_HPP

#include "context.hpp"

// lexer-parser integration
namespace lpi {
    template<typename Lexer, typename Parser, typename Analyser>
    class lpa_context : private context {
        friend Lexer;
        friend Parser;
        friend Analyser;
     public:
        lpa_context();
        lpa_context(Analyser);

        int run();
        void clear();
        void switch_input_stream(std::istream*);
     private:
        Lexer lexer;
        Parser parser;
        Analyser analyser;
        unsigned loc;

        void increase_location(unsigned) override;
        unsigned location() const override;
    };
}

#include "lpa_context.ipp"

#endif /* LPI_LPA_CONTEXT_HPP */