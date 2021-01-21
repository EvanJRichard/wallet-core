// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../Data.h"
#include "TransferableOutput.h"
#include "Output.h"
#include <vector>

namespace TW::Avalanche {

class TransactionOp {
  public:
    /// Encodes the op into the provided buffer.
    virtual void encode(Data& data) const;
  protected:
    TransactionOp(){}
};

/// Avalanche transaction operation.
class TransferableOp {
  public: 
    using TxID = Data;
    using OutputIndex = uint32_t;
    using UTXOID = std::pair<TxID, OutputIndex>;
    std::vector<UTXOID> UTXOIDs;
  private:
    bool sortUTXOIDs(UTXOID lhs, UTXOID rhs);
  public:
    Data AssetID;
    TransactionOp TransferOp;

    /// Encodes the op into the provided buffer.
    void encode(Data& data) const;

    TransferableOp(Data &assetID, std::vector<UTXOID> &utxoIDs, TransactionOp &transferOp)
      : AssetID(assetID), UTXOIDs(utxoIDs), TransferOp(transferOp) {
        std::sort(UTXOIDs.begin(), UTXOIDs.end(), sortUTXOIDs);
      }

    bool operator<(const TransferableOp& other);
};


class SECP256k1MintOperation : TransactionOp {
  private:
    uint32_t typeID = 8;
  public: 
    std::vector<uint32_t> AddressIndices;
    SECP256k1MintOutput MintOutput;
    SECP256k1TransferOutput TransferOutput;

    SECP256k1MintOperation(std::vector<uint32_t> &addressIndices, SECP256k1MintOutput &mintOutput, SECP256k1TransferOutput &transferOutput)
      : AddressIndices(addressIndices), MintOutput(mintOutput), TransferOutput(transferOutput) {
        std::sort(AddressIndices.begin(), AddressIndices.end());
      }

    void encode (Data& data) const;
};

class NFTMintOperation : TransactionOp {
  private:
    uint32_t typeID = 12;
  public: 
    std::vector<uint32_t> AddressIndices;
    uint32_t GroupID;
    Data Payload;

    std::vector<Output> Outputs; 

    NFTMintOperation(std::vector<uint32_t> &addressIndices, uint32_t groupID, Data &payload, std::vector<Output> &outputs)
    : AddressIndices(addressIndices), GroupID(groupID), Payload(payload), Outputs(outputs) {
      std::sort(AddressIndices.begin(), AddressIndices.end());
      SortOutputs(Outputs);
    }

    void encode (Data& data) const;
};

class NFTTransferOperation : TransactionOp {
  private:
    uint32_t typeID = 13;
  public: 
    std::vector<uint32_t> AddressIndices;
    NFTTransferOutput TransferOutput;

    NFTTransferOperation(std::vector<uint32_t> &addressIndices, NFTTransferOutput &transferOutput)
      : AddressIndices(addressIndices), TransferOutput(transferOutput) {
        std::sort(AddressIndices.begin(), AddressIndices.end());
        std::sort(TransferOutput.Addresses.begin(), TransferOutput.Addresses.end());
      }

    void encode (Data& data) const;
};

} // namespace TW::Avalanche
