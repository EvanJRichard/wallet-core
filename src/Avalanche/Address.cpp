// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Address.h"

#include "../Bech32.h"
#include "../Hash.h"
#include "../HexCoding.h"

#include <iostream>

using namespace TW::Avalanche;

bool Address::isValid(const std::string& string) {
    // split into before and after - 
    auto hyphenPos = string.find("-");
    if (hyphenPos == std::string::npos) {
        return false;
    }

    auto chainID = string.substr(hyphenPos - 1, 1);
    // compare before-hyphen with 'X' stringcompare (make it smarter later)
    if (chainID != "X") {
        // implementation is currently X-chain only
        return false;
    }
    // decode after-hyphen with bech32
    auto afterHyphen = string.substr(hyphenPos + 1);
    auto decoded = Bech32::decode(afterHyphen);
    if (decoded.second.size() != 32) {
        // decode failure
        return false;
    }
    // check hrp against 'avax' (make it smarter later)
    if (decoded.first != "avax") {
        // implementation is currently avax only
        return false;
    }
    // an address is a hash of the public key, so we cannot derive the public key from string for PubKey::isValid
    return true;
}

Address::Address(const std::string& string) {
    if (!isValid(string)) {
        throw std::invalid_argument("Invalid address string");
    }
    auto hyphenPos = string.find("-");
    auto decoded = Bech32::decode(string.substr(hyphenPos + 1));
    bytes.resize(addressSize);
    std::copy_n(decoded.second.begin(), addressSize, bytes.begin());
}

Address::Address(const PublicKey& publicKey) {
    if (publicKey.type != TWPublicKeyTypeSECP256k1) {
        throw std::invalid_argument("Invalid public key type");
    }
    auto result = Hash::ripemd(Hash::sha256(publicKey.bytes));
    bytes.resize(addressSize);
    std::copy_n(result.begin(), addressSize, bytes.begin());
}

std::string Address::string() const {
    return "X-" + Bech32::encode("avax", bytes);
}