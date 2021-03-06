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

std::vector<Address> structToAddresses(const google::protobuf::internal::RepeatedStringTypeTraits::RepeatedFieldType inAddrs) {
    std::vector<Address> out;
    for (auto &addrBytes : inAddrs) {
        auto pubkeyData = Data(addrBytes.begin(), addrBytes.end());
        auto pubkey = PublicKey(pubkeyData, TWPublicKeyTypeSECP256k1);
        auto addr = Address(pubkey);
        out.push_back(addr);
    }
    return out;
}

std::vector<TransferableInput> structToInputs(const google::protobuf::RepeatedPtrField<TW::Avalanche::Proto::TransferableInput> inputStructs) noexcept {
    std::vector<TransferableInput> inputs;
    for (auto &inputStruct : inputStructs) {
        auto txid = Data(inputStruct.tx_id().begin(), inputStruct.tx_id().end());
        auto utxoIndex = inputStruct.utxo_index();
        auto assetID = Data(inputStruct.asset_id().begin(), inputStruct.asset_id().end());
        std::vector<Address> spendableAddresses = structToAddresses(inputStruct.spendable_addresses());

        switch (inputStruct.input().input_case()) {
            case Proto::TransactionInput::kSecpTransferInput: {
                auto secpTxferStruct = inputStruct.input().secp_transfer_input();
                auto amount = secpTxferStruct.amount();
                std::vector<uint32_t> addressIndices;
                for (auto &index : secpTxferStruct.address_indices()) {
                    addressIndices.push_back(index);
                }
                auto txnInput = new SECP256k1TransferInput(amount, addressIndices);
                auto txferInput = TransferableInput(txid, utxoIndex, assetID, txnInput, spendableAddresses);
                inputs.push_back(txferInput);
                break;
            }
        } // end switch-case deciding which input to build
    } // end for loop building input structs
    return inputs;
}

std::vector<TransferableOutput> structToOutputs(const google::protobuf::RepeatedPtrField<TW::Avalanche::Proto::TransferableOutput> outputStructs) noexcept {
    std::vector<TransferableOutput> outputs;
    for (auto &outputStruct : outputStructs) {
        auto assetID = Data(outputStruct.asset_id().begin(), outputStruct.asset_id().end());
        switch (outputStruct.output().output_case()) {
            case Proto::TransactionOutput::kSecpTransferOutput: {
                auto secpTxferStruct = outputStruct.output().secp_transfer_output();
                auto amount = secpTxferStruct.amount();
                auto locktime = secpTxferStruct.locktime();
                auto threshold = secpTxferStruct.threshold();
                auto addresses = structToAddresses(secpTxferStruct.addresses());

                auto txnOutput = new SECP256k1TransferOutput(amount, locktime, threshold, addresses);
                auto txferOutput = TransferableOutput(assetID, txnOutput);
                outputs.push_back(txferOutput);

                break;
            }
            case Proto::TransactionOutput::kSecpMintOutput: {
                auto secpMintStruct = outputStruct.output().secp_mint_output();
                auto locktime = secpMintStruct.locktime();
                auto threshold = secpMintStruct.threshold();
                auto addresses = structToAddresses(secpMintStruct.addresses());
                
                auto txnOutput = new SECP256k1MintOutput(locktime, threshold, addresses);
                auto txferOutput = TransferableOutput(assetID, txnOutput);
                outputs.push_back(txferOutput);

                break;
            }
            case Proto::TransactionOutput::kNftTransferOutput: {
                auto nftTxferStruct = outputStruct.output().nft_transfer_output();
                auto groupID = nftTxferStruct.group_id();
                auto payload = Data(nftTxferStruct.payload().begin(), nftTxferStruct.payload().end());
                auto locktime = nftTxferStruct.locktime();
                auto threshold = nftTxferStruct.threshold();
                auto addresses = structToAddresses(nftTxferStruct.addresses());
                
                auto txnOutput = new NFTTransferOutput(groupID, payload, locktime, threshold, addresses);
                auto txferOutput = TransferableOutput(assetID, txnOutput);
                outputs.push_back(txferOutput);
                break;
            }
            case Proto::TransactionOutput::kNftMintOutput: {
                auto nftMintStruct = outputStruct.output().nft_mint_output();
                auto groupID = nftMintStruct.group_id();
                auto locktime = nftMintStruct.locktime();
                auto threshold = nftMintStruct.threshold();
                auto addresses = structToAddresses(nftMintStruct.addresses());

                auto txnOutput = new NFTMintOutput(groupID, locktime, threshold, addresses);
                auto txferOutput = TransferableOutput(assetID, txnOutput);
                outputs.push_back(txferOutput);
                break;
            }
        } // end switch-case deciding which output struct to build
    } // end for loop building output structs
    return outputs;
}

std::vector<TransferableOp> structToOperations(google::protobuf::RepeatedPtrField<TW::Avalanche::Proto::TransferableOp> opsStruct) noexcept {
    std::vector<TransferableOp> ops;
    for (auto &op : opsStruct) {
        auto assetID = Data(op.asset_id().begin(), op.asset_id().end());
        std::vector<TransferableOp::UTXOID> utxoIDs;
        for (auto &utxoidStruct: op.utxo_ids()) {
            auto txid = Data(utxoidStruct.tx_id().begin(), utxoidStruct.tx_id().end());
            auto utxoIndex = utxoidStruct.utxo_index();
            auto utxoid = std::make_pair(txid, utxoIndex);
            utxoIDs.push_back(utxoid);
        } // end for loop building utxo ids
        switch (op.transfer_op().operation_case()) {
            case Proto::TransferOp::kSecpMintOp: {
                auto txnOp = op.transfer_op().secp_mint_op();
                std::vector<uint32_t> addressIndices;
                for (auto &idx : txnOp.address_indices()) {
                    addressIndices.push_back(idx);
                }

                // parse and build mint output
                auto secpMintStruct = txnOp.mint_output();
                auto secpMintLocktime = secpMintStruct.locktime();
                auto secpMintThreshold = secpMintStruct.threshold();
                auto secpMintAddresses = structToAddresses(secpMintStruct.addresses());                
                auto mintOutput = SECP256k1MintOutput(secpMintLocktime, secpMintThreshold, secpMintAddresses);
                // end build mint output

                // parse and build transfer output
                auto secpTxferStruct = txnOp.transfer_output();
                auto tferAmount = secpTxferStruct.amount();
                auto tferLocktime = secpTxferStruct.locktime();
                auto tferThreshold = secpTxferStruct.threshold();
                auto tferAddresses = structToAddresses(secpTxferStruct.addresses());
                auto tferOutput = SECP256k1TransferOutput(tferAmount, tferLocktime, tferThreshold, tferAddresses);
                // end build transfer output

                auto transactionOp = new SECP256k1MintOperation(addressIndices, mintOutput, tferOutput);
                auto transferableOp = TransferableOp(assetID, utxoIDs, transactionOp);
                ops.push_back(transferableOp);
                break;
            }
            case Proto::TransferOp::kNftMintOp: {
                auto txnOp = op.transfer_op().nft_mint_op();
                std::vector<uint32_t> addressIndices;
                for (auto &idx : txnOp.address_indices()) {
                    addressIndices.push_back(idx);
                }
                auto groupID = txnOp.group_id();
                auto payload = Data(txnOp.payload().begin(), txnOp.payload().end());
                std::vector<Output> outputs;
                for (auto &outputStruct : txnOp.outputs()) {
                    auto locktime = outputStruct.locktime();
                    auto threshold = outputStruct.threshold();
                    auto addrs = structToAddresses(outputStruct.addresses()); 
                    auto output = std::make_tuple(locktime, threshold, addrs);
                    outputs.push_back(output);
                } // end loop building Outputs
                auto transactionOp = new NFTMintOperation(addressIndices, groupID, payload, outputs);
                auto transferableOp = TransferableOp(assetID, utxoIDs, transactionOp);
                ops.push_back(transferableOp);
                break;
            }
            case Proto::TransferOp::kNftTransferOp: {
                auto txnOp = op.transfer_op().nft_transfer_op();
                std::vector<uint32_t> addressIndices;
                for (auto &idx : txnOp.address_indices()) {
                    addressIndices.push_back(idx);
                }
                auto groupID = txnOp.group_id();
                auto payload = Data(txnOp.payload().begin(), txnOp.payload().end());
                auto locktime = txnOp.locktime();
                auto threshold = txnOp.threshold();
                auto addresses = structToAddresses(txnOp.addresses());
                auto nftTransfer = NFTTransferOutput(groupID, payload, locktime, threshold, addresses);
                auto transactionOp = new NFTTransferOperation(addressIndices, nftTransfer);
                auto transferableOp = TransferableOp(assetID, utxoIDs, transactionOp);
                ops.push_back(transferableOp);
                break;
            }
        } // end switch-case deciding which operations struct to build
    } // end for loop building operations structs
    return ops;
}

BaseTransaction structToBaseTx(const TW::Avalanche::Proto::BaseTx txStruct) noexcept {
    // grab members of struct
    auto typeID = txStruct.typeid_();
    auto networkID = txStruct.network_id();
    auto blockchainID = Data(txStruct.blockchain_id().begin(), txStruct.blockchain_id().end()); 
    auto outputStructs = txStruct.outputs();
    auto inputStructs = txStruct.inputs();
    auto memo = Data(txStruct.memo().begin(), txStruct.memo().end());

    auto outputs = structToOutputs(outputStructs);
    auto inputs = structToInputs(inputStructs);

    return BaseTransaction(typeID, networkID, blockchainID, inputs, outputs, memo);
}

BaseTransaction buildBaseTx(const Proto::SigningInput &input) noexcept {
    auto txStruct = input.input_tx().base_tx();
    return structToBaseTx(txStruct);
}

UnsignedCreateAssetTransaction buildCreateAssetTx(const Proto::SigningInput &input) noexcept {
    auto txStruct = input.input_tx().create_asset_tx();
    auto baseTxn = structToBaseTx(txStruct.base_tx());
    auto name = txStruct.name();
    auto symbol = txStruct.symbol();
    auto denomination = txStruct.denomination();
    std::vector<InitialState> initialStates;
    for (auto &stateStruct : txStruct.initial_states()) {
        InitialState::FeatureExtension fxID = InitialState::FeatureExtension::SECP256k1;
        switch (stateStruct.fx_id()) {
            case 0:
                fxID = InitialState::FeatureExtension::SECP256k1;
                break;
            case 1:
                fxID = InitialState::FeatureExtension::NFT;
                break;
        } // end switch-case setting fx id
        auto outputs = structToOutputs(stateStruct.outputs());
        // this gives std::vector<TransferableOutput>. need std::vector<TransactionOutput*>.
        std::vector<TransactionOutput*> outputPointers;
        for (auto &output : outputs) {
            outputPointers.push_back(output.Output->duplicate());
        }
        auto initialState = InitialState(fxID, outputPointers);
        initialStates.push_back(initialState);
    }    
    return UnsignedCreateAssetTransaction(baseTxn, name, symbol, denomination, initialStates);
}

UnsignedExportTransaction buildExportTx(const Proto::SigningInput &input) noexcept {
    auto txStruct = input.input_tx().export_tx();
    auto baseTxn = structToBaseTx(txStruct.base_tx());
    auto dest = Data(txStruct.destination_chain().begin(), txStruct.destination_chain().end());
    auto exports = structToOutputs(txStruct.outs());

    return UnsignedExportTransaction(baseTxn, dest, exports);
}

UnsignedImportTransaction buildImportTx(const Proto::SigningInput &input) noexcept {
    auto txStruct = input.input_tx().import_tx();
    auto baseTxn = structToBaseTx(txStruct.base_tx());
    auto source = Data(txStruct.source_chain().begin(), txStruct.source_chain().end());
    auto imports = structToInputs(txStruct.ins());

    return UnsignedImportTransaction(baseTxn, source, imports);
}

UnsignedOperationTransaction buildOperationTx(const Proto::SigningInput &input) noexcept {
    auto txStruct = input.input_tx().operation_tx();  
    auto baseTxn = structToBaseTx(txStruct.base_tx());
    auto ops = structToOperations(txStruct.ops());

    return UnsignedOperationTransaction(baseTxn, ops);  
}


Proto::SigningOutput Signer::sign(const Proto::SigningInput &input) noexcept {
    auto protoOutput = Proto::SigningOutput();

    std::vector<PrivateKey> privateKeys;
    for (auto& privateKeyBytes : input.private_keys()) {
        auto privateKey = PrivateKey(Data(privateKeyBytes.begin(), privateKeyBytes.end()));
        privateKeys.push_back(privateKey);
    }

    switch (input.input_tx().tx_case()) {
    case Proto::UnsignedTx::TxCase::kBaseTx: {
        auto transaction = buildBaseTx(input);
        auto encoded = Signer::sign(privateKeys, transaction);
        protoOutput.set_encoded(encoded.data(), encoded.size());
    }
        break;
    case Proto::UnsignedTx::TxCase::kCreateAssetTx: {
        auto transaction = buildCreateAssetTx(input);
        auto encoded = Signer::sign(privateKeys, transaction);
        protoOutput.set_encoded(encoded.data(), encoded.size());
    }
        break;
    case Proto::UnsignedTx::TxCase::kExportTx: {
        auto transaction = buildExportTx(input);
        auto encoded = Signer::sign(privateKeys, transaction);
        protoOutput.set_encoded(encoded.data(), encoded.size());
    }
        break;
    case Proto::UnsignedTx::TxCase::kImportTx: {
        auto transaction = buildImportTx(input);
        auto encoded = Signer::sign(privateKeys, transaction);
        protoOutput.set_encoded(encoded.data(), encoded.size());
    }
        break;
    case Proto::UnsignedTx::TxCase::kOperationTx: {
        auto transaction = buildOperationTx(input);
        auto encoded = Signer::sign(privateKeys, transaction);
        protoOutput.set_encoded(encoded.data(), encoded.size());
    }
        break;
    }
    return protoOutput;
}

Data Signer::sign(const std::vector<PrivateKey>& privateKeys, BaseTransaction& transaction) noexcept {
    // see avalanchejs/src/apis/avm/basetx.ts and tx.ts for reference implementations
    Data transactionBytes;
    // TODO this is probably not the best place to store and add the codecID bytes...?
    transactionBytes.push_back(0x00); // first codecID byte: 0x00
    transactionBytes.push_back(0x00); // second codecID byte: 0x00
    transaction.encode(transactionBytes);
    auto msgBytes = Hash::sha256(transactionBytes);
    std::vector<Credential> credentials;
    for (auto &input : transaction.Inputs) {
        if (input.Input->getTypeID() == 5) {
            //secp input, make an SECP credential
            std::vector<Data> sigs;
            for (auto &sigidx: input.Input->getAddressIndices()) { 
                auto addresses = input.SpendableAddresses;
                std::sort(addresses.begin(), addresses.end());
                auto addressRequested = addresses[sigidx]; // TODO this access is not guaranteed to be safe.
                for (auto &key : privateKeys) {
                    auto possibleAddress = Address(key.getPublicKey(TWPublicKeyTypeSECP256k1)); 
                    if (possibleAddress == addressRequested) {
                        auto signature = key.sign(msgBytes, TWCurveSECP256k1); // TODO EJR I thought this was TWCurveED25519 for sure
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
    auto signedTransaction = SignedTransaction(transaction, credentials);
    Data outputData;
    signedTransaction.encode(outputData);
    return outputData;    
}