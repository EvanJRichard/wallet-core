// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Credential.h"
#include "InitialState.h"
#include "TransferableInput.h"
#include "TransferableOp.h"
#include "TransferableOutput.h"
#include "../CoinEntry.h"
#include <stdexcept>

namespace TW::Avalanche {

class BaseTransaction {
  public:
    uint32_t TypeID;
    uint32_t NetworkID;
    Data BlockchainID;
    std::vector<TransferableInput> Inputs;
    std::vector<TransferableOutput> Outputs;
    Data Memo;

    void encode(Data& data) const;

    BaseTransaction(uint32_t typeID, uint32_t networkID, Data& blockchainID,
                    std::vector<TransferableInput>& inputs,
                    std::vector<TransferableOutput>& outputs, Data& memo)
        : TypeID(typeID), NetworkID(networkID), BlockchainID(blockchainID), Memo(memo) {
        if (blockchainID.size() != 32) {
            throw std::invalid_argument("BlockchainID must be 32 bytes.");
        }
        if (memo.size() > 256) {
            throw std::invalid_argument("Memo must be no longer than 256 bytes.");
        }
        Inputs = inputs;
        Outputs = outputs;
        std::sort(Inputs.begin(), Inputs.end());
        std::sort(Outputs.begin(), Outputs.end());
    }
};

class UnsignedCreateAssetTransaction : public BaseTransaction {
  public:
    std::string Name;
    std::string Symbol;
    uint8_t Denomination;
    std::vector<InitialState> InitialStates;

    void encode(Data& data) const;

    UnsignedCreateAssetTransaction(uint32_t networkID, Data& blockchainID,
                                   std::vector<TransferableInput>& inputs,
                                   std::vector<TransferableOutput>& outputs, Data& memo,
                                   std::string& name, std::string& symbol, uint8_t denomination,
                                   std::vector<InitialState>& states)
        : BaseTransaction(1, networkID, blockchainID, inputs, outputs, memo)
        , Name(name)
        , Symbol(symbol)
        , Denomination(denomination)
        , InitialStates(states) {
        if (name.size() > 128) {
            throw std::invalid_argument("Name must be no longer than 128 characters.");
        }
        if (symbol.size() > 4) {
            throw std::invalid_argument("Symbol must be no longer than 4 characters.");
        }
        std::sort(InitialStates.begin(), InitialStates.end());
    }

    UnsignedCreateAssetTransaction(BaseTransaction& baseTxn, std::string& name, std::string& symbol,
                                   uint8_t denomination, std::vector<InitialState>& states)
        : BaseTransaction(baseTxn)
        , Name(name)
        , Symbol(symbol)
        , Denomination(denomination)
        , InitialStates(states) {
        if (name.size() > 128) {
            throw std::invalid_argument("Name must be no longer than 128 characters.");
        }
        if (symbol.size() > 4) {
            throw std::invalid_argument("Symbol must be no longer than 4 characters.");
        }
        std::sort(InitialStates.begin(), InitialStates.end());
    }
};

class UnsignedOperationTransaction : public BaseTransaction {
  public:
    std::vector<TransferableOp> Operations;

    void encode(Data& data) const;

    UnsignedOperationTransaction(uint32_t networkID, Data& blockchainID,
                                 std::vector<TransferableInput>& inputs,
                                 std::vector<TransferableOutput>& outputs, Data& memo,
                                 std::vector<TransferableOp>& ops)
        : BaseTransaction(2, networkID, blockchainID, inputs, outputs, memo), Operations(ops) {
        std::sort(Operations.begin(), Operations.end());
    }

    UnsignedOperationTransaction(BaseTransaction& baseTxn, std::vector<TransferableOp>& ops)
        : BaseTransaction(baseTxn), Operations(ops) {
        std::sort(Operations.begin(), Operations.end());
    }
};

class UnsignedImportTransaction : public BaseTransaction {
  public:
    Data SourceChain;
    std::vector<TransferableInput> ImportInputs;

    void encode(Data& data) const;

    UnsignedImportTransaction(uint32_t networkID, Data& blockchainID,
                              std::vector<TransferableInput>& inputs,
                              std::vector<TransferableOutput>& outputs, Data& memo, Data& source,
                              std::vector<TransferableInput>& importInputs)
        : BaseTransaction(3, networkID, blockchainID, inputs, outputs, memo)
        , SourceChain(source)
        , ImportInputs(importInputs) {
        if (source.size() != 32) {
            throw std::invalid_argument("SourceChain must be 32 bytes.");
        }
        std::sort(ImportInputs.begin(), ImportInputs.end());
    }

    UnsignedImportTransaction(BaseTransaction& baseTxn, Data& source,
                              std::vector<TransferableInput> importInputs)
        : BaseTransaction(baseTxn), SourceChain(source), ImportInputs(importInputs) {
        if (source.size() != 32) {
            throw std::invalid_argument("SourceChain must be 32 bytes.");
        }
        std::sort(ImportInputs.begin(), ImportInputs.end());
    }
};

class UnsignedExportTransaction : public BaseTransaction {
  public:
    Data DestinationChain;
    std::vector<TransferableOutput> ExportOutputs;

    void encode(Data& data) const;

    UnsignedExportTransaction(uint32_t networkID, Data& blockchainID,
                              std::vector<TransferableInput>& inputs,
                              std::vector<TransferableOutput>& outputs, Data& memo, Data& dest,
                              std::vector<TransferableOutput>& exportOutputs)
        : BaseTransaction(3, networkID, blockchainID, inputs, outputs, memo)
        , DestinationChain(dest)
        , ExportOutputs(exportOutputs) {
        if (dest.size() != 32) {
            throw std::invalid_argument("DestinationChain must be 32 bytes.");
        }
        std::sort(ExportOutputs.begin(), ExportOutputs.end());
    }

    UnsignedExportTransaction(BaseTransaction& baseTxn, Data& dest,
                              std::vector<TransferableOutput>& exportOutputs)
        : BaseTransaction(baseTxn), DestinationChain(dest), ExportOutputs(exportOutputs) {
        if (dest.size() != 32) {
            throw std::invalid_argument("DestinationChain must be 32 bytes.");
        }
        std::sort(ExportOutputs.begin(), ExportOutputs.end());
    }
};

class SignedTransaction {
  public:
    const uint16_t CodecID = 0; // TODO EJR x-chain is on codecID 1 now but docs are on 0
    BaseTransaction UnsignedTransaction;
    std::vector<Credential> Credentials;

    void encode(Data& data) const;

    SignedTransaction(BaseTransaction& txn, std::vector<Credential>& credentials)
        : UnsignedTransaction(txn), Credentials(credentials) {}
};

} // namespace TW::Avalanche
