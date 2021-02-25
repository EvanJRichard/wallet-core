// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"
#include "Address.h"
#include "../PublicKey.h"

using namespace TW;
using namespace TW::Avalanche;


Proto::SigningOutput Signer::sign(const Proto::SigningInput &input) noexcept {
    auto protoOutput = Proto::SigningOutput();
    auto privateKey = PrivateKey(Data(input.private_key().begin(), input.private_key().end()));
    std::vector<PrivateKey> privateKeys = {privateKey};  //TODO somehow grab private keys from input
    auto transaction = BaseTransaction(); //TODO somehow build transaction from input, This is kind of a big design question - what should be in proto::signinginput?
    encoded = sign(privateKeys, transaction);
    protoOutput.set_encoded(encoded.data(), encoded.size());
    return protoOutput;
}

Data Signer::sign(const std::vector<PrivateKey>& privateKeys, BaseTransaction& transaction) noexcept {
    // see avalanchejs/src/apis/avm/basetx.ts and tx.ts for reference implementations
    // get tx bytes
    Data transactionBytes;
    transaction.encode(transactionBytes);
    // msgBytes is the sha256 hash of the buffer
    auto msgBytes = Hash::sha256(transactionBytes);
    // signing this msgBytes needs to produce an array of credential. TODO key <-> credential association is probably incorrect
    std::vector<Credential> credentials;
    for (auto &input : transaction.Inputs) {
        if (input.Input.TypeID == 5) {
            //secp input, make an SECP credential
            std::vector<Data> sigs;
            for (auto &sigidx: input.AddressIndices) {
                auto signature = privateKeys[sigidx].sign(msgBytes, TWCurveED25519); // TODO might need smarter key selection than that
                sigs.push_back(signature);
            }
            auto credential = SECP256k1Credential(sigs);
            credentials.push_back(credential);
        } else {
            //nft input, make an NFT credential
            // TODO define NFT input
            // TODO add support for NFT inputs
        }
    }
    // that gives you the array of credentials to pass to SignedTransaction 
    auto signedTransaction = SignedTransaction(transaction, credentials);
    Data outputData;
    signedTransaction.encode(outputData);
    return outputData;    
}