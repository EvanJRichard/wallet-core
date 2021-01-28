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

#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Avalanche;

// These tests are modeled after the avalanchejs signer tests.

TEST(AvalancheSigner, SignWithAsofLocktimeThreshold) {
    // TODO: Finalize test implementation

    //auto key = PrivateKey(parse_hex("__PRIVKEY_DATA__"));
    //auto publicKey = key.getPublicKey(TWPublicKeyTypeED25519);
    //auto from = Address(publicKey);
    //auto to = Address("__TO_ADDRESS__");
    //...
    //auto transaction = Transaction(...)
    //auto signature = Signer::sign(key, transaction);
    //auto result = transaction.serialize(signature);

    //ASSERT_EQ(hex(serialized), "");
    //ASSERT_EQ(...)
}

TEST(AvalancheSigner, SignWithoutAsofLocktimeThreshold) {
}

TEST(AvalancheSigner, SignOperationTx) {

}

TEST(AvalancheSigner, SignImportTx) {
}

TEST(AvalancheSigner, SignExportTx) {
}


/*
  test('Creation Tx1 with asof, locktime, threshold', () => {
    const txu:UnsignedTx = set.buildBaseTx(
      netid, blockchainID,
      new BN(9000), assetID, addrs3, addrs1, addrs1, undefined, undefined, undefined,
      UnixNow(), UnixNow().add(new BN(50)), 1,
    );
    const tx:Tx = txu.sign(keymgr1);

    const tx2:Tx = new Tx();
    tx2.fromString(tx.toString());
    expect(tx2.toBuffer().toString('hex')).toBe(tx.toBuffer().toString('hex'));
    expect(tx2.toString()).toBe(tx.toString());
  });
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

  test('Creation Tx4 using ImportTx', () => {
    const txu:UnsignedTx = set.buildImportTx(
      netid, blockchainID, addrs3, addrs1, addrs2, importUTXOs, bintools.cb58Decode(PlatformChainID), new BN(90), assetID,
      new UTF8Payload("hello world").getPayload(), UnixNow());
    const tx:Tx = txu.sign(keymgr1);
    const tx2:Tx = new Tx();
    tx2.fromBuffer(tx.toBuffer());
    expect(tx2.toBuffer().toString('hex')).toBe(tx.toBuffer().toString('hex'));
  });

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