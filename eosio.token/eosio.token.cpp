/**
 *  @file
 *  @copyright defined in eos/LICENSE.txt
 */

#include <eosio.token/eosio.token.hpp>

namespace eosio {

// 给issue账号创建代币，代币符号和发型数量为maximum_supply
void token::create( name   issuer,
                    asset  maximum_supply )
{
    // 据此我们知道，如果创建合约的账号没有授予权限给别的账号，则只有它自己才可以创建新的代币
    require_auth( _self );

    auto sym = maximum_supply.symbol;
    eosio_assert( sym.is_valid(), "invalid symbol name" );
    eosio_assert( maximum_supply.is_valid(), "invalid supply");
    eosio_assert( maximum_supply.amount > 0, "max-supply must be positive");

    // 确认该合约下还没存在该代币
    stats statstable( _self, sym.code().raw() );
    auto existing = statstable.find( sym.code().raw() );
    eosio_assert( existing == statstable.end(), "token with symbol already exists" );

    // 向合约的multi_index表添加代币信息
    statstable.emplace( _self, [&]( auto& s ) {
       s.supply.symbol = maximum_supply.symbol;  // 代币符号，初始数量默认为0
       s.max_supply    = maximum_supply;         // 发行量和符号
       s.issuer        = issuer;                 // 打币发行者
    });
}

// 代币发行者给to账号发送代币，发送数量为quantity，附加备注信息为memo
void token::issue( name to, asset quantity, string memo )
{
    auto sym = quantity.symbol;
    eosio_assert( sym.is_valid(), "invalid symbol name" );
    eosio_assert( memo.size() <= 256, "memo has more than 256 bytes" );  // 备注信息不能超过 256 bytes

    // 确认该合约下存在该代币
    stats statstable( _self, sym.code().raw() );
    auto existing = statstable.find( sym.code().raw() );
    eosio_assert( existing != statstable.end(), "token with symbol does not exist, create token before issue" );
    const auto& st = *existing;

    require_auth( st.issuer );  // 该动作需要代币发行者的授权
    eosio_assert( quantity.is_valid(), "invalid quantity" );
    eosio_assert( quantity.amount > 0, "must issue positive quantity" );

    eosio_assert( quantity.symbol == st.supply.symbol, "symbol precision mismatch" );
    eosio_assert( quantity.amount <= st.max_supply.amount - st.supply.amount, "quantity exceeds available supply");  // 不能超过剩余发行数量

    // 更新已发行的数量
    statstable.modify( st, same_payer, [&]( auto& s ) {
       s.supply += quantity;
    });

    // 因为下面如果to != st.issuer的时候，执行的transfer里面会做sub_balance，
    // 所以这里需要增加发行者的accounts表的代币数量
    add_balance( st.issuer, quantity, st.issuer );  // 影响accounts表

    // 上面add_balance使发行者拥有了quantity数量的代币，
    // 如果to账号不是发行者，则发行者通过转账的方式，把刚才quantity数量的代币转给to
    if( to != st.issuer ) {
      SEND_INLINE_ACTION( *this, transfer, { {st.issuer, "active"_n} },
                          { st.issuer, to, quantity, memo }
      );
    }
}

// 销毁代币，数量为quantity，附加备注信息为memo
void token::retire( asset quantity, string memo )
{
    auto sym = quantity.symbol;
    eosio_assert( sym.is_valid(), "invalid symbol name" );
    eosio_assert( memo.size() <= 256, "memo has more than 256 bytes" );

    // 确保该合约下存在该代币
    stats statstable( _self, sym.code().raw() );
    auto existing = statstable.find( sym.code().raw() );
    eosio_assert( existing != statstable.end(), "token with symbol does not exist" );
    const auto& st = *existing;

    require_auth( st.issuer );  // 销毁操作需要得到发行者的授权
    eosio_assert( quantity.is_valid(), "invalid quantity" );
    eosio_assert( quantity.amount > 0, "must retire positive quantity" );

    eosio_assert( quantity.symbol == st.supply.symbol, "symbol precision mismatch" );

    // 更新已发行（流通中）的数量
    statstable.modify( st, same_payer, [&]( auto& s ) {
       s.supply -= quantity;
    });

    // 销毁数量为quantity，则从发行者手中减去这么多数量的代币
    sub_balance( st.issuer, quantity );
}

// from给to转账，数量为quantity，转账备注为memo
void token::transfer( name    from,
                      name    to,
                      asset   quantity,
                      string  memo )
{
    eosio_assert( from != to, "cannot transfer to self" );  // 不能给自己转账
    require_auth( from );  // 需要得到from的授权
    eosio_assert( is_account( to ), "to account does not exist");
    auto sym = quantity.symbol.code();
    stats statstable( _self, sym.raw() );
    const auto& st = statstable.get( sym.raw() );  // 如果合约的stat表中不存在该代币，执行get操作将回滚这次转账(transfer)操作

    // 把该转账操作通知给from和to
    require_recipient( from );
    require_recipient( to );

    eosio_assert( quantity.is_valid(), "invalid quantity" );
    eosio_assert( quantity.amount > 0, "must transfer positive quantity" );
    eosio_assert( quantity.symbol == st.supply.symbol, "symbol precision mismatch" );
    eosio_assert( memo.size() <= 256, "memo has more than 256 bytes" );

    auto payer = has_auth( to ) ? to : from;

    sub_balance( from, quantity );
    add_balance( to, quantity, payer );  // 如果to账号第一次获得该代币，add_balance中执行multi_index表插入的时候，payer将支付ram费用
}

// 从owner账号中，减去value数量的代币
void token::sub_balance( name owner, asset value ) {
   accounts from_acnts( _self, owner.value );

   const auto& from = from_acnts.get( value.symbol.code().raw(), "no balance object found" );  // 确保owner已经拥有该代币
   eosio_assert( from.balance.amount >= value.amount, "overdrawn balance" );  // 确保余额充足

   // 更新owner的表数据
   from_acnts.modify( from, owner, [&]( auto& a ) {
         a.balance -= value;
   });
}

// 给owner账号增加value数量的代币，如果owner第一次获得该代币，将由ram_payer支付ram费用
void token::add_balance( name owner, asset value, name ram_payer )
{
   accounts to_acnts( _self, owner.value );
   auto to = to_acnts.find( value.symbol.code().raw() );
   if( to == to_acnts.end() ) {  // 说明第一次获得该代币
      to_acnts.emplace( ram_payer, [&]( auto& a ){  // 向表中添加记录，并由ram_payer支付ram费用（记录需要保存到ram中）
        a.balance = value;
      });
   } else {  // 说明不是第一次获得该代币
      to_acnts.modify( to, same_payer, [&]( auto& a ) {  // 这里的same_payer指的是上面emplace时的ram_payer，这里的数据占用ram大小没有改变，所以ram费用没有增加也没有减少
        a.balance += value;  // 更新表中的数据
      });
   }
}

// 如果owner已经拥有该代币的话，open将不会更改任何东西
// 如果owner还未拥有该代币，ram_payer将帮助它支付ram费用，以在表中添加一条余额为0的代币记录
void token::open( name owner, const symbol& symbol, name ram_payer )
{
   require_auth( ram_payer );  // 支付ram费用的账号需要授权

   auto sym_code_raw = symbol.code().raw();

   stats statstable( _self, sym_code_raw );
   const auto& st = statstable.get( sym_code_raw, "symbol does not exist" );
   eosio_assert( st.supply.symbol == symbol, "symbol precision mismatch" );

   accounts acnts( _self, owner.value );
   auto it = acnts.find( sym_code_raw );

   // 如果owner还未拥有该代币，则向表中添加一条记录，余额为0，由ram_payer来支付ram费用
   if( it == acnts.end() ) {
      acnts.emplace( ram_payer, [&]( auto& a ){
        a.balance = asset{0, symbol};
      });
   }
}

// 如果owner的symbol代币余额为0，通过close可以释放该代币占用的ram
void token::close( name owner, const symbol& symbol )
{
   require_auth( owner );
   accounts acnts( _self, owner.value );
   auto it = acnts.find( symbol.code().raw() );
   eosio_assert( it != acnts.end(), "Balance row already deleted or never existed. Action won't have any effect." );  // 确保owner拥有该代币
   eosio_assert( it->balance.amount == 0, "Cannot close because the balance is not zero." );  // 确保owner的该币的余额为0
   acnts.erase( it );  // 删除表中的记录
}

} /// namespace eosio

EOSIO_DISPATCH( eosio::token, (create)(issue)(transfer)(open)(close)(retire) )
