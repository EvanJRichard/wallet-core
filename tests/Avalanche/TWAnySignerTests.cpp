// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.


#include "proto/Avalanche.pb.h"
#include <TrustWalletCore/TWAnySigner.h>
#include "HexCoding.h"

#include "../interface/TWTestUtilities.h"
#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Avalanche;


/* example test from Algorand

TEST(TWAnySignerAlgorand, Sign) {
    auto privateKey = parse_hex("d5b43d706ef0cb641081d45a2ec213b5d8281f439f2425d1af54e2afdaabf55b");
    auto note = parse_hex("68656c6c6f");
    auto genesisHash = Base64::decode("wGHE2Pwdvd7S12BL5FaOP20EGYesN73ktiC1qzkkit8=");
    
    Proto::SigningInput input;
    auto &transaction = *input.mutable_transaction_pay();
    transaction.set_to_address("CRLADAHJZEW2GFY2UPEHENLOGCUOU74WYSTUXQLVLJUJFHEUZOHYZNWYR4");
    transaction.set_fee(263000ull);
    transaction.set_amount(1000000000000ull);
    transaction.set_first_round(1937767ull);
    transaction.set_last_round(1938767ull);
    input.set_genesis_id("mainnet-v1.0");
    input.set_genesis_hash(genesisHash.data(), genesisHash.size());
    input.set_note(note.data(), note.size());
    input.set_private_key(privateKey.data(), privateKey.size());

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeAlgorand);

    ASSERT_EQ(hex(output.encoded()), "82a3736967c440baa00062adcdcb5875e4435cdc6885d26bfe5308ab17983c0fda790b7103051fcb111554e5badfc0ac7edf7e1223a434342a9eeed5cdb047690827325051560ba374786e8aa3616d74cf000000e8d4a51000a3666565ce00040358a26676ce001d9167a367656eac6d61696e6e65742d76312e30a26768c420c061c4d8fc1dbdded2d7604be4568e3f6d041987ac37bde4b620b5ab39248adfa26c76ce001d954fa46e6f7465c40568656c6c6fa3726376c42014560180e9c92da3171aa3c872356e30a8ea7f96c4a74bc1755a68929c94cb8fa3736e64c42061bf060efc02e2887dfffc8ed85268c8c091c013eedf315bc50794d02a8791ada474797065a3706179");
}
*/


TEST(TWAnySignerAvalanche, Sign) {
    // auto privateKey = parse_hex("d5b43d706ef0cb641081d45a2ec213b5d8281f439f2425d1af54e2afdaabf55b");
    
    Proto::SigningInput input;
    // basically need to set everything in Input.
    // keep in mind the expected formats in Signer!

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeAvalanche);

    ASSERT_EQ(hex(output.encoded()), "00000000000000003039d891ad56056d9c01f18f43f58b5c784ad07a4a49cf3d1f11623804b5cba2c6bf00000002dbcf890f77f49b96857648b72b77f9f82937f28a68704af05da0dc12ba53f2db0000000700000000000003e8000000000000000000000001000000013cb7d3842e8cee6a0ebd09f1fe884f6861e1b29cdbcf890f77f49b96857648b72b77f9f82937f28a68704af05da0dc12ba53f2db000000070000000000003039000000000000d43100000001000000013cb7d3842e8cee6a0ebd09f1fe884f6861e1b29c00000002f1e1d1c1b1a191817161514131211101f0e0d0c0b0a09080706050403020100000000005dbcf890f77f49b96857648b72b77f9f82937f28a68704af05da0dc12ba53f2db0000000500000000075bcd15000000020000000300000007f1e1d1c1b1a191817161514131211101f0e0d0c0b0a09080706050403020100000000005dbcf890f77f49b96857648b72b77f9f82937f28a68704af05da0dc12ba53f2db0000000500000000075bcd1500000002000000030000000700000004deadbeef00000002000000090000000244ef527f47cab3ed82eb267c27c04869e46531b05db643f5bc97da21148afe161f17634a90f4e22adb810b472062f7e809dde19059fa7048f9972a481fe9390d0044ef527f47cab3ed82eb267c27c04869e46531b05db643f5bc97da21148afe161f17634a90f4e22adb810b472062f7e809dde19059fa7048f9972a481fe9390d00000000090000000244ef527f47cab3ed82eb267c27c04869e46531b05db643f5bc97da21148afe161f17634a90f4e22adb810b472062f7e809dde19059fa7048f9972a481fe9390d0044ef527f47cab3ed82eb267c27c04869e46531b05db643f5bc97da21148afe161f17634a90f4e22adb810b472062f7e809dde19059fa7048f9972a481fe9390d00");
}
