#include "client.hpp"

// trigger with
// openssl s_client -servername "localhost" -connect localhost:2014
int main() {
    client::run();
    return 0;
}
