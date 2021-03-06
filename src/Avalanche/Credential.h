// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include "../Data.h"

namespace TW::Avalanche {

    class Credential {
      public: 
        uint32_t TypeID;
        std::vector<Data> Signatures;  
    
        /// Encodes the Credential into the provided buffer.
        void encode(Data& data) const;

        bool operator<(const Credential& other) const;

      protected:
        Credential(uint32_t typeID, std::vector<Data> &sigs)
         : TypeID(typeID), Signatures(sigs) {}
    };

    class SECP256k1Credential : public Credential {
      public:
        SECP256k1Credential(std::vector<Data> &sigs)
        : Credential(9, sigs) {}
    };

    class NFTCredential : public Credential {
      public:
        NFTCredential(std::vector<Data> &sigs)
        : Credential(14, sigs) {}
    };

} // namespace TW::Avalanche