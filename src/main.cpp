#include <stdio.h>

#include "src/repl.h"

int ARGC;
char** ARGV;

void startup(void) {
}

void shutdown(void) {
}

int main(int argc, char* argv[]) {
    ARGC = argc;
    ARGV = argv;
    startup();
    plx::REPL repl;
    int res = repl.loop();
    shutdown();
    return res;
}
