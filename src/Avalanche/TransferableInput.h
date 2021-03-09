// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../Data.h"
#include "Address.h"
#include <vector>

namespace TW::Avalanche {

class TransactionInput{ //TODO user-devs should never actually make a TransactionInput, we want this to be an Interface or something
  public: 
    uint32_t TypeID; // TODO turn these various TypeIDs into enums; and review where they live/class structure
    std::vector<uint32_t> AddressIndices;
    // void encode (Data& data) const; //we want to enforce that all subclasses can encode
  protected:
    TransactionInput(): TypeID(0), AddressIndices() {}  
};

/// Avalanche transaction input.
class TransferableInput {
  public:
    Data TxID;
    uint32_t UTXOIndex;
    Data AssetID;
    TransactionInput Input;
    std::vector<Address> SpendableAddresses; // corresponding to the Output this came from. not encoded

    /// Encodes the input into the provided buffer.
    void encode(Data& data) const;

    TransferableInput(Data &txid, uint32_t utxoIndex, Data &assetID, TransactionInput &input)
        : TxID(txid) , UTXOIndex(utxoIndex)
        , AssetID(assetID), Input(input) {}

    
    bool operator<(const TransferableInput& other) const;
};


class SECP256k1TransferInput : public TransactionInput {
  
  public:
    uint32_t TypeID = 5; 
    uint64_t Amount;

    SECP256k1TransferInput(uint64_t amount, std::vector<uint32_t> addressIndices)
      : Amount(amount) {
        AddressIndices = addressIndices;
        std::sort(AddressIndices.begin(), AddressIndices.end());
      }
  
    void encode (Data& data) const;
};

} // namespace TW::Avalanche
