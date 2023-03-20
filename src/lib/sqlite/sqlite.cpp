// Compile with:
// g++ -shared -o libsqlite3.so -fPIC sqlite.cpp sqlite3.o

#include <iostream>

#include "../evaluator.h"
#include "../list.h"
#include "../primitive.h"
#include "../string.h"

struct Evaluator;
typedef void (*PrimFun)(Evaluator *etor);
extern void definePrim(const std::string& name, PrimFun primFun, Evaluator* etor);

extern "C" {

    #include "sqlite/sqlite3.h"

    namespace plx {

        struct Evaluator;

        void __attribute__ ((constructor)) fileStartup(void) {
            std::cout << "sqlite.cpp " << __func__ << " called\n";
        }

        void __attribute__ ((constructor)) fileShutdown(void) {
            std::cout << "sqlite.cpp " << __func__ << " called\n";
        }

        static void prim_sqlite_open(Evaluator* etor) {
            List* args = (List*)etor->popObj();
            String* dbNameString = (String*)args->_first;
            std::string dbNameStr = dbNameString->_str;
            const char* dbName = dbNameStr.c_str();
            sqlite3 *pdb;
            int res = sqlite3_open(dbName, &pdb);
        }

        void defineAll(Evaluator* etor) {
            definePrim("sqlite:open", prim_sqlite_open, etor);
        }

    }

}
