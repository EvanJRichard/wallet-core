// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Avalanche/Signer.h"
#include "Avalanche/Address.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include "../src/Base58.h" // TODO EJR: avax base58 may be slightly different than bitcoin base58.

#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Avalanche;


const auto privateKeyOneBytes = Base58::bitcoin.decode("ewoqjP7PxY4yr3iLTpLisriqt94hdyDFNgchSxGGztUrTXtNN");
const auto privateKeyOne = PrivateKey(privateKeyOneBytes); 
const auto publicKeyOne = privateKeyOne.getPublicKey(TWPublicKeyTypeSECP256k1);
const auto addressOne = Address(publicKeyOne); // hopefully comes out as X-avax18jma8ppw3nhx5r4ap8clazz0dps7rv5u00z96u
// const auto publicKeyTwo = PublicKey(parse_hex("0x02b33c917f2f6103448d7feb42614037d05928433cb25e78f01a825aa829bb3c27"), TWPublicKeyTypeSECP256k1);
// const auto addressTwo = Address(publicKeyTwo); 
// const auto publicKeyThree = PublicKey(parse_hex("0xc3344128e060128ede3523a24a461c8943ab0859"), TWPublicKeyTypeSECP256k1);
// const auto addressThree = Address(publicKeyTwo); 

auto blockchainID = Base58::bitcoin.decode("2eNy1mUFdmaxXNj1eQHUe7Np4gju9sJsEtWQ4MX3ToiNKuADed");
uint32_t netID = 12345;
auto assetID = parse_hex("0x000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f"); 
uint32_t baseTypeID = 0;
Data memo = {0xde, 0xad, 0xbe, 0xef};

// These tests are modeled after the avalanchejs signer tests
// many example values are likewise modeled after the docs.avax.network tx serialization examples

TEST(AvalancheSigner, SignWithLocktimeThreshold) {
    auto amount = 1000;   
    auto locktime = 0;
    auto threshold = 1;
    // TODO: Finalize test implementation

    auto addressesInOne = std::vector<uint32_t>{3, 7};
    auto txidOne = parse_hex("0xf1e1d1c1b1a191817161514131211101f0e0d0c0b0a090807060504030201000");
    auto utxoIndexOne = 5;
    auto coreInputOne = SECP256k1TransferInput(123456789, addressesInOne);
    auto inputOne = TransferableInput(txidOne, utxoIndexOne, assetID, coreInputOne);

    auto addressesInTwo = std::vector<uint32_t>{3, 7};
    auto txidTwo = parse_hex("0xf1e1d1c1b1a191817161514131211101f0e0d0c0b0a090807060504030201000");
    auto utxoIndexTwo = 5;
    auto coreInputTwo = SECP256k1TransferInput(123456789, addressesInTwo);
    auto inputTwo = TransferableInput(txidTwo, utxoIndexTwo, assetID, coreInputTwo);
    
    auto inputs = std::vector<TransferableInput>{inputOne, inputTwo};

    auto addressesOutOne = std::vector<Address>{addressOne};
    auto coreOutputOne = SECP256k1TransferOutput(12345, 54321, threshold, addressesOutOne);
    auto outputOne = TransferableOutput(assetID, coreOutputOne);

    auto addressesOutTwo = std::vector<Address>{addressOne};
    auto coreOutputTwo = SECP256k1TransferOutput(amount, locktime, threshold, addressesOutTwo);
    auto outputTwo = TransferableOutput(assetID, coreOutputTwo);

    auto outputs = std::vector<TransferableOutput>{outputOne, outputTwo};


    // it wants: uint32_t, uint32_t, Data, std::vector<TransferableInput>, std::vector<TransferableOutput>, Data
    // it has: uint32_t, const uint32_t, const Data, std:;vector<Inputs>, std::vecotr<Outputs>, Data
    auto transaction = BaseTransaction(baseTypeID, netID, blockchainID, inputs, outputs, memo);
    // looking at the js code, the below goldens are equivalent in hex and cb58
    // from test script: unsigned tx looks like 00000000000000003039d891ad56056d9c01f18f43f58b5c784ad07a4a49cf3d1f11623804b5cba2c6bf00000002dbcf890f77f49b96857648b72b77f9f82937f28a68704af05da0dc12ba53f2db0000000700000000000003e8000000000000000000000001000000013cb7d3842e8cee6a0ebd09f1fe884f6861e1b29cdbcf890f77f49b96857648b72b77f9f82937f28a68704af05da0dc12ba53f2db000000070000000000003039000000000000d43100000001000000013cb7d3842e8cee6a0ebd09f1fe884f6861e1b29c0000000200000005dbcf890f77f49b96857648b72b77f9f82937f28a68704af05da0dc12ba53f2db0000000500000000075bcd1500000002000000030000000700000005dbcf890f77f49b96857648b72b77f9f82937f28a68704af05da0dc12ba53f2db0000000500000000075bcd1500000002000000030000000700000004deadbeef    
    // or maybe 113q5k7s7Duh4wjHzpQuE6CHyXogsi1kHZk8hEesymA9KSj2p3tzY3L6P9Yp577JZw9NDhXNsqezcFGyEbP9KdgyuhKb4vbaM1rbYvYo1PhVkMihv1rFSchgayWZt9Xwrj5u2EACdYtu3Rs947RYyd6UBnVjd5BJq7Pmh5wgBVAp78vXoTytojfvwzzHrnNoPctgz9mBxeJQ4Ug2US7vvbeXiJXMUwmAFnRZgwCboD4PzUUx4mk9zAyY4gL9Qjz65spLRbGvKiNmWBLtkoqPammrUbeZhcg6zQRMBHVnGJYu8Am2Sf5nWHQZGikp8gJMKvqt4YaactrPtCcJGKwLU6KZc3rvWwKEozqUzZ7WSFPC6fuj1iMKoHkjV2E2M64UiFzARFczAMdoUZuzS287T2zURdD4iX6dHpJYfr6x4px3DQr1gJDD1gtiSyFReo6yhK3h8
    std::vector<PrivateKey> keyRing = {privateKeyOne};
    auto encodedSignedTransaction = Signer::sign(keyRing, transaction); // this signature may change, huh?
    // from test script: signed tx looks like 00000000000000003039d891ad56056d9c01f18f43f58b5c784ad07a4a49cf3d1f11623804b5cba2c6bf00000002dbcf890f77f49b96857648b72b77f9f82937f28a68704af05da0dc12ba53f2db0000000700000000000003e8000000000000000000000001000000013cb7d3842e8cee6a0ebd09f1fe884f6861e1b29cdbcf890f77f49b96857648b72b77f9f82937f28a68704af05da0dc12ba53f2db000000070000000000003039000000000000d43100000001000000013cb7d3842e8cee6a0ebd09f1fe884f6861e1b29c0000000200000005dbcf890f77f49b96857648b72b77f9f82937f28a68704af05da0dc12ba53f2db0000000500000000075bcd1500000002000000030000000700000005dbcf890f77f49b96857648b72b77f9f82937f28a68704af05da0dc12ba53f2db0000000500000000075bcd1500000002000000030000000700000004deadbeef0000000200000009000000022b5def69e5c8a2e5a87d45d651fe473b2fdaa1ab296dfd8c53511d30471960ef601a22135fbd1c2cbb14204dfe4aaf46bb424f7d5254d743327e382cef6276dd002b5def69e5c8a2e5a87d45d651fe473b2fdaa1ab296dfd8c53511d30471960ef601a22135fbd1c2cbb14204dfe4aaf46bb424f7d5254d743327e382cef6276dd0000000009000000022b5def69e5c8a2e5a87d45d651fe473b2fdaa1ab296dfd8c53511d30471960ef601a22135fbd1c2cbb14204dfe4aaf46bb424f7d5254d743327e382cef6276dd002b5def69e5c8a2e5a87d45d651fe473b2fdaa1ab296dfd8c53511d30471960ef601a22135fbd1c2cbb14204dfe4aaf46bb424f7d5254d743327e382cef6276dd00
    // or maybe 111111112X8qZ3asgvrvjcHeePuKo8WkTbgTpTinAcHugAC8HnCt68brfJUPZTk8martxrrnVjGEMoSFzUQDsymamaerJPRMLz3mqxpEi8Jq7W1gph4CUxuh2cfRukG9jACwCW6FBVAKuRkBamU1Chdu9dNQczmKp28eePAQMAXCyLth7GWZUf3DWSKuh191frTS1EiDtq1ihGRbbF9ZgCo3126cXU77jbRNqWrrv5M7itM1hkBP7keT5r28CZbmGhZmKpwch8vzpTK8E27kiGqKEPtC7WaCQdGcVN35GXhC34QL9RjACWCTATAjX8jSqe5PGEA4eFZeDpDamFdYpXTkd1p54DvJFYCwcY5p1igQAQTGHmzPnGxpK4mM71y6eBF5WWQ2MgK31NXKNKnMbVPciPycUSrpnBAAr8aEFdjGaKStsgAAtAhMPW1h3zJtFZCCRae537ZzVAncZevHkGJxFbuKtYBJ7UmFhydr9pUZf7VsgRKhgakboxTj7YN69GwtUG3TSAYkuXD4XhcVDo82rTYMtLfshYTDdXGFtgNQbTYjJwJR8CwN2NmAvSwqPqk3ijDg2Tkr5vw9fcDHgTwb5Mbfwg8e2QkenwJvLQFfaKX31YbESLaNto8WjmYkHKXgm9VJdVVnaQaZW6XixxayxaNLdubo7DPa78CsVpw8f4mChyPRf3wPVD1Zw9RdWGG41JgnAjXszfEbemhuafAnTtjLVCWVvHBmtLHX4RNNp3js2Zb9fWtJVBt1wRontxGrQezrvXLYmNwEkJh2rjshRtWemPWMHJdZH62NqVyyvtuE2RBaGTBgi7eaTQoBs3kThdizn2jSAYa

    //ASSERT_EQ(hex(encodedSignedTransaction), "");
    //ASSERT_EQ(...)
}

TEST(AvalancheSigner, SignWithoutAsofLocktimeThreshold) {
  /*
  test('Creation Tx2 without asof, locktime, threshold', () => {
    const txu:UnsignedTx = set.buildBaseTx(
      netid, blockchainID,
      new BN(9000), assetID,
      addrs3, addrs1, addrs1
    );
    const tx:Tx = txu.sign(keymgr1);
    const tx2:Tx = new Tx();
    tx2.fromBuffer(tx.toBuffer());
    expect(tx2.toBuffer().toString('hex')).toBe(tx.toBuffer().toString('hex'));
    expect(tx2.toString()).toBe(tx.toString());
  });
  */
}

TEST(AvalancheSigner, SignOperationTx) {
  /*
  test('Creation Tx3 using OperationTx', () => {
    const txu:UnsignedTx = set.buildNFTTransferTx(
      netid, blockchainID, 
      addrs3, addrs1, addrs2, nftutxoids, new BN(90), avaxAssetID, undefined,
      UnixNow(), UnixNow().add(new BN(50)), 1,
    );
    const tx:Tx = txu.sign(keymgr1);
    const tx2:Tx = new Tx();
    tx2.fromBuffer(tx.toBuffer());
    expect(tx2.toBuffer().toString('hex')).toBe(tx.toBuffer().toString('hex'));
  });
  */
}

TEST(AvalancheSigner, SignImportTx) {
/*
  test('Creation Tx4 using ImportTx', () => {
    const txu:UnsignedTx = set.buildImportTx(
      netid, blockchainID, addrs3, addrs1, addrs2, importUTXOs, bintools.cb58Decode(PlatformChainID), new BN(90), assetID,
      new UTF8Payload("hello world").getPayload(), UnixNow());
    const tx:Tx = txu.sign(keymgr1);
    const tx2:Tx = new Tx();
    tx2.fromBuffer(tx.toBuffer());
    expect(tx2.toBuffer().toString('hex')).toBe(tx.toBuffer().toString('hex'));
  });
*/
}

TEST(AvalancheSigner, SignExportTx) {
/*
  test('Creation Tx5 using ExportTx', () => {
    const txu:UnsignedTx = set.buildExportTx(
      netid, blockchainID, new BN(90), avaxAssetID,
      addrs3, addrs1, addrs2, bintools.cb58Decode(PlatformChainID), 
      undefined, undefined, new UTF8Payload("hello world").getPayload(), UnixNow()
    )
    const tx:Tx = txu.sign(keymgr1);
    const tx2:Tx = new Tx();
    tx2.fromBuffer(tx.toBuffer());
    expect(tx.toBuffer().toString('hex')).toBe(tx2.toBuffer().toString('hex'));
  });
*/
}

