#include <dlfcn.h>

#include "address.h"
#include "evaluator.h"
#include "list.h"
#include "string.h"

namespace plx {

    typedef void (*DefineAllFunc)(Evaluator* etor);

    void prim_dl_close(Evaluator* etor) {
        List* args = (List*)etor->popObj();
        Address* addr = (Address*)args->_first;
        dlclose(addr->_address);
    }

    void prim_dl_open(Evaluator* etor) {
        List* args = (List*)etor->popObj();
        String* fileNameString = (String*)args->_first;
        std::string fileNameStr = fileNameString->_str;
        const char* fileName = fileNameStr.c_str();
        void* dlHandle = dlopen(fileName, RTLD_LAZY);
        if (!dlHandle) {
            fprintf (stderr, "%s\n", dlerror());
            exit(1);
        }
        dlerror();    /* Clear any existing error */
        DefineAllFunc defineAll = (DefineAllFunc)dlsym(dlHandle, "defineAll");
        char* error;
        if ((error = dlerror()) != NULL)  {
            fprintf (stderr, "%s\n", error);
            exit(1);
        }
        defineAll(etor);
        etor->pushObj(new Address(dlHandle));
    }

}
