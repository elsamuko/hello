#include "server.hpp"

// trigger with
// openssl s_client -servername "localhost" -connect localhost:2014
int main() {
    server::run();
    return 0;
}
