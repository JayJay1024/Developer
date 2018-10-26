#include <contracttest/contracttest.hpp>

using namespace eosio;

void contracttest::sayhi( name user ) {
    print( "Hello ", name{user} );
}

// 原来的叫 EOSIO_API
EOSIO_DISPATCH( contracttest, (sayhi) )