#pragma once

#include <stddef.h>

#if defined(_WIN32)
#	if defined(ED25519_BUILD_DLL)
#		define ED25519_DECLSPEC __declspec(dllexport)
#	elif defined(ED25519_DLL)
#		define ED25519_DECLSPEC __declspec(dllimport)
#	else
#		define ED25519_DECLSPEC
#	endif
#else
#	define ED25519_DECLSPEC
#endif

#ifdef __cplusplus
extern "C" {
#endif

// Create a random seed.
int ED25519_DECLSPEC ed25519_create_seed(unsigned char *seed);

// Create a public/private key pair from a seed.
void ED25519_DECLSPEC ed25519_create_keypair(unsigned char *public_key, unsigned char *private_key,
					     const unsigned char *seed);

// Sign a message with a key pair.
void ED25519_DECLSPEC ed25519_sign(unsigned char *signature, const unsigned char *message, size_t message_len,
				   const unsigned char *public_key, const unsigned char *private_key);

// Verify a signature on a message with the public key.
int ED25519_DECLSPEC ed25519_verify(const unsigned char *signature, const unsigned char *message, size_t message_len,
				    const unsigned char *public_key);

// Add a scalar to a public/private key pair.
void ED25519_DECLSPEC ed25519_add_scalar(unsigned char *public_key, unsigned char *private_key,
					 const unsigned char *scalar);

// Calculate a shared secret given your secret key and someone else's public key.
void ED25519_DECLSPEC ed25519_key_exchange(unsigned char *shared_secret, const unsigned char *public_key,
					   const unsigned char *private_key);

#ifdef __cplusplus
}
#endif
