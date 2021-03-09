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

BaseTransaction buildBaseTx(const Proto::SigningInput &input) noexcept {
    
}

UnsignedCreateAssetTransaction buildCreateAssetTx(const Proto::SigningInput &input) noexcept {
    
}

UnsignedExportTransaction buildExportTx(const Proto::SigningInput &input) noexcept {
    
}

UnsignedImportTransaction buildImportTx(const Proto::SigningInput &input) noexcept {
    
}

UnsignedOperationTransaction buildOperationTx(const Proto::SigningInput &input) noexcept {
    
}


Proto::SigningOutput Signer::sign(const Proto::SigningInput &input) noexcept {
    auto protoOutput = Proto::SigningOutput();

    std::vector<PrivateKey> privateKeys;
    for (auto& privateKeyBytes : input.private_keys()) {
        auto privateKey = PrivateKey(Data(privateKeyBytes.begin(), privateKeyBytes.end()));
        privateKeys.push_back(privateKey);
    }

    Data encoded;
    switch (input.input_tx().tx_case()) {
    case Proto::UnsignedTx::TxCase::kBaseTx: {
        auto transaction = buildBaseTx(input);
        transaction.encode(encoded);
    }
        break;
    case Proto::UnsignedTx::TxCase::kCreateAssetTx: {
        auto transaction = buildCreateAssetTx(input);
        transaction.encode(encoded);
    }
        break;
    case Proto::UnsignedTx::TxCase::kExportTx: {
        auto transaction = buildExportTx(input);
        transaction.encode(encoded);
    }
        break;
    case Proto::UnsignedTx::TxCase::kImportTx: {
        auto transaction = buildImportTx(input);
        transaction.encode(encoded);
    }
        break;
    case Proto::UnsignedTx::TxCase::kOperationTx: {
        auto transaction = buildOperationTx(input);
        transaction.encode(encoded);
    }
        break;
    }
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
            for (auto &sigidx: input.Input.AddressIndices) { 
                auto addresses = input.SpendableAddresses;
                std::sort(addresses.begin(), addresses.end());
                auto addressRequested = addresses[sigidx];
                for (auto &key : privateKeys) {
                    auto possibleAddress = Address(key.getPublicKey(TWPublicKeyTypeSECP256k1));
                    if (possibleAddress == addressRequested) {
                        auto signature = key.sign(msgBytes, TWCurveED25519); 
                        sigs.push_back(signature);
                    }
                }
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