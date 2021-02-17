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
    // TODO: Check and finalize implementation

    auto protoOutput = Proto::SigningOutput();
    Data encoded;
    // auto privateKey = PrivateKey(Data(input.private_key().begin(), input.private_key().end()));
    // auto signature = privateKey.sign(payload, TWCurveED25519);
    // encoded = encodeSignature(signature);
    // EJR Note: Basically, `encoded` needs to be filled with whatever bytes get sent to the network, so the full SignedTransaction
    // alternatively, could set up to use sign, below, maybe easier to think about

    protoOutput.set_encoded(encoded.data(), encoded.size());
    return protoOutput;
}

Data Signer::sign(const PrivateKey& privateKey, BaseTransaction& transaction) noexcept {
    // js version: 
    //     sign(kc:KeyChain):Tx {
    //     const txbuff = this.toBuffer();
    //     const msg:Buffer = Buffer.from(createHash('sha256').update(txbuff).digest());
    //     const sigs:Array<Credential> = this.transaction.sign(msg, kc);
    //     return new Tx(this, sigs);
    // }
    // cpp pseudo based on js:
    // get tx bytes
    // msgBytes is the sha256 hash of the buffer
    // signing this msgBytes needs to produce an array of credential. subroutine: 
        //    // figure out which credential to make: secp or nft
              // use the key to sign the input bytes, that's a function on privateKey, use TWCurveSECP256k1
              // build the credential from that (you have a type and a sig, that's what a Credential is constructed with)
    // that gives you the array of credentials to pass to SignedTransaction 
    // then you encode the SignedTransaction to get the data.
    return Data{};    
}