//
// Created by YohayG on 14/06/2022.
//

#pragma once

#include <stdint.h>

#include "stm/include/Observer.hpp"
#include "stm/include/SessionTable.hpp"
#include "application/ssl/ProtectedObjectsTrafficMngr.hpp"
#include "application/shared-data/DPSharedDoublePolicyArray.hpp"
#include "application/tls-flow/processor/src/Md5Digest.hpp"
#include "ParsingStatus.hpp"
#include "../include/ParsingTypes.hpp"

namespace TLSFlow {
	namespace NSProcessor
	{
		namespace Engine
		{
			enum class ExtensionTypeE : uint16_t;

			class IRecordParser {
			public:

			virtual ~IRecordParser() = default;

				virtual ParsingStatus parse(const uint8_t *const payload, uint16_t len, uint16_t &consumed) = 0;

				virtual bool isHandshake() = 0;

				virtual bool isApplicationData() const = 0;

				virtual bool isSupportedVersion() = 0;
				virtual bool isSupportedRecordType() = 0;
				virtual uint16_t getTlsVersion() const = 0;

				virtual uint16_t getTlsRecordLength() const = 0;

				virtual uint8_t getRecordType() const = 0;
			};
			class IHandshakeParser {
			public:
			virtual ~IHandshakeParser() = default;

				virtual ParsingStatus parse(const uint8_t *const payload, uint16_t len, uint16_t &consumed) = 0;

				virtual bool isClientHello() const = 0;

				virtual uint32_t getHandshakeLength() const = 0;

				virtual uint8_t getHandshakeType() const = 0;


			};


			class IClientHelloParser {
			public:
				enum class Status
				{
					Complete,
					IncompleteClientHelloPrefix,
					IncompleteClientHelloBody,
					IncompleteExtensionSection,
					Failure
				};
				virtual ~IClientHelloParser() = default;

				virtual Status parse(const uint8_t *const payload, uint16_t bufferLen, uint32_t expectedLength, uint16_t &consumed) = 0;

				virtual uint8_t getsidLen() const = 0;

				virtual const uint8_t *getpRandom() const = 0;

				virtual const uint8_t *getpSid() const = 0;

				virtual uint16_t getCipherSuitesLen() const = 0;

				virtual const uint8_t *getpCipherSuites() const = 0;

				virtual uint8_t getCompressionLen() const = 0;

				virtual const uint8_t *getpCompressionMethods() const = 0;

				virtual uint16_t getExtensionsLen() const = 0;

				virtual const uint8_t *getpExtensions() const = 0;

				virtual uint16_t getVersion() const = 0;
			};


			class IExtensionsParser {
			public:

				enum class Status
				{
					Complete,
					PendingTypeField,
					IncompleteExtensionTypeField,
					PendingLengthField,
					IncompleteExtensionLengthField,
					PendingExtensionData,
					IncompleteExtensionData,
					Failure
				};

				virtual ~IExtensionsParser() = default;
				virtual Status parse(const uint8_t *const payload, uint16_t len, uint16_t &consumed) = 0;

				/**
				 *
				 * @return the length of a single parsed extension data part (excluding the type and length descriptors fields length)
				 */
				virtual uint16_t getExtensionDataLength() const = 0;

				/**
				 *
				 * @return Length of a single parsed extension, including the type and length descriptors field length
				 */
				virtual uint16_t getSingleExtensionTotalLength() const = 0;

				virtual ExtensionTypeE getExtensionType() const = 0;

				virtual const uint8_t *getExtensionValuePtr() const = 0;

			};

			class ISupportedVersionsExtensionParser {
			public:
				virtual ~ISupportedVersionsExtensionParser() = default;
				virtual ParsingStatus parse(const uint8_t *const payload, uint16_t len, uint16_t &consumed) = 0;

				virtual uint8_t getLength() const = 0;

				virtual uint16_t getNumOfVersions() const = 0;

				virtual uint16_t getVersion(uint16_t index) const = 0;
			};

			class ISupportedGroupsExtensionParser  {
			public:
				virtual ~ISupportedGroupsExtensionParser() = default;
				virtual ParsingStatus parse(const uint8_t *const payload, uint16_t len, uint16_t &consumed) = 0;

				virtual uint16_t getGroupsLen() const = 0;

				virtual uint16_t getNumOfGroups() const = 0;

				virtual uint16_t getGroup(uint16_t index) const = 0;
			};

			class ICipherSuitesParser  {
			public:
				virtual ~ICipherSuitesParser() = default;
				virtual ParsingStatus parse(const uint8_t *const payload, uint16_t len, uint16_t &consumed) = 0;

				virtual uint16_t getCipherSuitesLen() const = 0;

				virtual const uint16_t *getpCipherSuites() const = 0;

				virtual uint16_t getSuite(uint16_t index) const = 0;

				virtual uint16_t getNumOfSuites() const = 0;
			};

			class ISniParser  {
			public:
				virtual ~ISniParser() = default;
				virtual ParsingStatus parse(const uint8_t *const payload, uint16_t len, uint16_t &consumed) = 0;

				virtual uint16_t getSniListLen() const = 0;

				virtual uint16_t getNumOfSni() const = 0;

				virtual uint16_t getSniLen(uint16_t index) const = 0;

				virtual const uint8_t *getpSni(uint16_t index) const = 0;

				virtual uint8_t getSniType(uint16_t index) const = 0;


			};

			class IAlertParser {
			public:
				virtual ~IAlertParser() = default;

				virtual ParsingStatus parse(const uint8_t *const payload, uint16_t len, uint16_t &consumed) = 0;

				virtual uint8_t getAlertLevel() const = 0;

				virtual uint8_t getAlertDescription() const = 0;
			};

			class IParsersContainer {
			public:
				IParsersContainer() = default;

				virtual ~IParsersContainer() = default;

				void init(unique_ptr<IRecordParser> mRecordParser,
						  unique_ptr<IHandshakeParser> mHandshakeParser,
						  unique_ptr<IClientHelloParser> mHelloParser,
						  unique_ptr<IExtensionsParser> mExtParser,
						  unique_ptr<ISupportedGroupsExtensionParser> mSuppGroupsParser,
						  unique_ptr<ISupportedVersionsExtensionParser> mSuppVersionsParser,
						  unique_ptr<ICipherSuitesParser> mCipherSuitesParser,
						  unique_ptr <ISniParser> mSniParser,
						  unique_ptr<IAlertParser> mAlertParser,
						  unique_ptr<Md5Digest> mDigest) {

					tlsRecord = move(mRecordParser);
					handshake = move(mHandshakeParser);
					clientHello = move(mHelloParser);
					extensions = move(mExtParser);
					supportedGroups = move(mSuppGroupsParser);
					supportedVersions = move(mSuppVersionsParser);
					cipherSuites = move(mCipherSuitesParser);
					sni          = move(mSniParser);
					alert        = move(mAlertParser);
					digest = move(mDigest);
				}

				unique_ptr <IRecordParser>                      tlsRecord           = nullptr;
				unique_ptr <IHandshakeParser>                   handshake           = nullptr;
				unique_ptr <IClientHelloParser>                 clientHello         = nullptr;
				unique_ptr <IExtensionsParser>                  extensions          = nullptr;
				unique_ptr <ISupportedGroupsExtensionParser>    supportedGroups     = nullptr;
				unique_ptr <ISupportedVersionsExtensionParser>  supportedVersions   = nullptr;
				unique_ptr <ICipherSuitesParser>                cipherSuites        = nullptr;
				unique_ptr <ISniParser>                         sni                 = nullptr;
				unique_ptr <IAlertParser>                       alert               = nullptr;
				unique_ptr <Md5Digest>                          digest              = nullptr;

			};

			enum class ExtensionTypeE : uint16_t {
				server_name                                      =   0, /* [RFC6066][RFC-ietf-tls-exported-authenticator-15] */
				max_fragment_length                              =   1, /* [RFC6066][RFC8449] */
				client_certificate_url                           =   2, /* [RFC6066] */
				trusted_ca_keys                                  =   3, /* [RFC6066] */
				truncated_hmac                                   =   4, /* [RFC6066][IESG Action 2018-08-16] */
				status_request                                   =   5, /* [RFC6066] */
				user_mapping                                     =   6, /* [RFC4681] */
				client_authz                                     =   7, /* [RFC5878] */
				server_authz                                     =   8, /* [RFC5878] */
				cert_type                                        =   9, /* [RFC6091] */
				supported_groups                                 =   10, /* [RFC8422][RFC7919] */
				ec_point_formats                                 =   11, /* [RFC8422] */
				srp                                              =   12, /* [RFC5054] */
				signature_algorithms                             =   13, /* [RFC8446] */
				use_srtp                                         =   14, /* [RFC5764] */
				heartbeat                                        =   15, /* [RFC6520] */
				application_layer_protocol_negotiation           =   16, /* [RFC7301] */
				status_request_v2                                =   17, /* [RFC6961] */
				signed_certificate_timestamp                     =   18, /* [RFC6962] */
				client_certificate_type                          =   19, /* [RFC7250] */
				server_certificate_type                          =   20, /* [RFC7250] */
				padding                                          =   21, /* [RFC7685] */
				encrypt_then_mac                                 =   22, /* [RFC7366] */
				extended_master_secret                           =   23, /* [RFC7627] */
				token_binding                                    =   24, /* [RFC8472] */
				cached_info                                      =   25, /* [RFC7924] */
				tls_lts                                          =   26, /* [draft-gutmann-tls-lts] */
				compress_certificate                             =   27, /* [RFC8879] */
				record_size_limit                                =   28, /* [RFC8449] */
				pwd_protect                                      =   29, /* [RFC8492] */
				pwd_clear                                        =   30, /* [RFC8492] */
				password_salt                                    =   31, /* [RFC8492] */
				ticket_pinning                                   =   32, /* [RFC8672] */
				tls_cert_with_extern_psk                         =   33, /* [RFC8773] */
				delegated_credentials                            =   34, /* [draft-ietf-tls-subcerts] */
				session_ticket                                   =   35, /* [RFC5077][RFC8447] */
				TLMSP                                            =   36, /* [ETSI TS 103 523-2] */
				TLMSP_proxying                                   =   37, /* [ETSI TS 103 523-2] */
				TLMSP_delegate                                   =   38, /* [ETSI TS 103 523-2] */
				supported_ekt_ciphers                            =   39, /* [RFC8870] */
				Reserved_1                                       =   40, /* [tls-reg-review mailing list] */
				pre_shared_key                                   =   41, /* [RFC8446] */
				early_data                                       =   42, /* [RFC8446] */
				supported_versions                               =   43, /* [RFC8446] */
				cookie                                           =   44, /* [RFC8446] */
				psk_key_exchange_modes                           =   45, /* [RFC8446] */
				Reserved_2                                       =   46, /* [tls-reg-review mailing list] */
				certificate_authorities                          =   47, /* [RFC8446] */
				oid_filters                                      =   48, /* [RFC8446] */
				post_handshake_auth                              =   49, /* [RFC8446] */
				signature_algorithms_cert                        =   50, /* [RFC8446] */
				key_share                                        =   51, /* [RFC8446] */
				transparency_info                                =   52, /* [RFC9162] */
				connection_id_deprecated                         =   53, /* [RFC9146] */
				connection_id                                    =   54, /* [RFC9146] */
				external_id_hash                                 =   55, /* [RFC8844] */
				external_session_id                              =   56, /* [RFC8844] */
				quic_transport_parameters                        =   57, /* [RFC9001] */
				ticket_request                                   =   58, /* [RFC9149] */
				dnssec_chain                                     =   59, /* [RFC9102][RFC Errata 6860] */
				token_binding_prototype_ms                       =   21760, /* https://success.qualys.com/discussions/s/question/0D52L00004TnvMESAZ/new-ie-sslhello-extension */
				type_quic_transport_parameters_draft             =   0xffa5,
				NORMALIZED_GREASE                                =   0x0a0a

			};
			enum class SniTypeE : uint16_t {
				hostName                                         = 0
			};


		}

	}
}
