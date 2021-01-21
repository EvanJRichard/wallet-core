// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TransferableOutput.h"

namespace TW::Avalanche {

    class InitialState {
      public: 
        uint32_t FxID;
        std::vector<TransactionOutput> Outputs;

        enum FeatureExtension {SECP256k1 = 0, NFT = 1};
        
        InitialState(FeatureExtension fxid, std::vector<TransactionOutput> &outputs)
         : FxID(fxid), Outputs(outputs) {
           std::sort(Outputs.begin(), Outputs.end());
         }

        /// Encodes the InitialState into the provided buffer.
        void encode(Data& data) const;

        bool operator<(const InitialState& other);
    };

} // namespace TW::Avalanche