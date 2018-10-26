#pragma once

#include <eosiolib/eosio.hpp>

using namespace eosio;

// class [[eosio::contract]] contracttest : public contract { // 这样也是可以的，[[eosio::contract]]只能放 class 和 contracttest 中间
CONTRACT contracttest : public contract {
  public:
      // using contract::contract;  // 或下一行
      contracttest( name s, name code, datastream<const char*> ds ) : contract( s, code, ds ) {}  // 或上一行

      ACTION sayhi( name user );  // 严格检查user合法性(不能超过12个字符，有下列字符组成: .12345abcdefghijklmnopqrstuvwxyz)
      // ACTION sayhi( capi_name user );  // 不会检查user是否合法

      // 这样也是可以的
      // [[eosio::action]]
      // void sayhi( name user );
};
