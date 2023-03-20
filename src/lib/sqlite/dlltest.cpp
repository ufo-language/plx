// This is an example of how to create a .so file.
// Compile with:
// g++ -shared -o libhello.so -fPIC dlltest.cpp

#include <iostream>

#include "../evaluator.h"

extern "C" {

    struct Evaluator;

    void __attribute__ ((constructor)) fileStartup(void) {
        std::cout << __func__ << " called\n";
    }

    void __attribute__ ((constructor)) fileShutdown(void) {
        std::cout << __func__ << " called\n";
    }

    static void prim_repl(Evaluator* etor) {
    }

    void defineAll(Evaluator* etor) {
        std::cout << "Hello, world! from " << __func__ << "\n";
    }

}
