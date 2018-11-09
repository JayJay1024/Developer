/**
 *  @file
 *  @copyright defined in eos/LICENSE.txt
 */
#pragma once

#include <eosiolib/asset.hpp>
#include <eosiolib/eosio.hpp>

#include <string>

namespace eosiosystem {
   class system_contract;
}

namespace eosio {

   using std::string;

   //  [[eosio::contract]]：新的生成ABI属性标注
   class [[eosio::contract("eosio.token")]] token : public contract {
      public:
         using contract::contract;

         //  [[eosio::action]]：新的生成ABI属性标注
         [[eosio::action]]
         void create( name   issuer,  // 用的是name，原来的account_name已弃用
                      asset  maximum_supply);

         [[eosio::action]]
         void issue( name to, asset quantity, string memo );

         [[eosio::action]]
         void retire( asset quantity, string memo );  // 新增的action

         [[eosio::action]]
         void transfer( name    from,
                        name    to,
                        asset   quantity,
                        string  memo );

         [[eosio::action]]
         void open( name owner, const symbol& symbol, name ram_payer );  // 新增的action

         [[eosio::action]]
         void close( name owner, const symbol& symbol );  // 新增的action

         static asset get_supply( name token_contract_account, symbol_code sym_code )
         {
            stats statstable( token_contract_account, sym_code.raw() );
            const auto& st = statstable.get( sym_code.raw() );
            return st.supply;
         }

         static asset get_balance( name token_contract_account, name owner, symbol_code sym_code )
         {
            accounts accountstable( token_contract_account, owner.value );
            const auto& ac = accountstable.get( sym_code.raw() );
            return ac.balance;
         }

      private:
         //  [[eosio::table]]：新的生成ABI属性标注
         struct [[eosio::table]] account {
            asset    balance;

            // 主索引，multi_index表必须，multi_index的get/find/upper_bound/lower_bound等操作会根据索引过滤
            // 除了主索引，还可以自定义二级索引，二级索引不是multi_index表必须的
            uint64_t primary_key()const { return balance.symbol.code().raw(); }
         };

         struct [[eosio::table]] currency_stats {
            asset    supply;
            asset    max_supply;
            name     issuer;

            uint64_t primary_key()const { return supply.symbol.code().raw(); }
         };

         // 如果账号A拥有eosio.token的EOS币，将会存在一张code为eosio.token，scope为A，名为accounts的multi_index表
         typedef eosio::multi_index< "accounts"_n, account > accounts;

         // 如果eosio.token合约发行了一个叫EOS的币，将会存在一张code为eosio.token，scope为EOS，名为stat的multi_index表
         typedef eosio::multi_index< "stat"_n, currency_stats > stats;

         void sub_balance( name owner, asset value );
         void add_balance( name owner, asset value, name ram_payer );
   };

} /// namespace eosio
