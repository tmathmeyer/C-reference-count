#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "types.h"
#include "ref.h"

void run() {
    list *namelist = S(_list(
            _string("ted", false),
            _list(
                _string("meyer", false),
                EMPTY)));

    list *names2 = map(strcopy, namelist);
    

    print_string_list(namelist);
    print_string_list(names2);

    L(namelist);
    L(names2);
}

int main() {
    run();
}
