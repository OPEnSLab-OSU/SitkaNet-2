#ifndef _CERTIFICATES_H_
#define _CERTIFICATES_H_

#ifdef __cplusplus
extern "C"
{
#endif

/* This file is auto-generated by the pycert_bearssl tool.  Do not change it manually.
 * Certificates are BearSSL br_x509_trust_anchor format.  Included certs:
 *
 * Index:    0
 * Label:    DigiCert High Assurance EV Root CA
 * Subject:  C=US,O=DigiCert Inc,OU=www.digicert.com,CN=DigiCert High Assurance EV Root CA
 * Domain(s): www.cloudflare.com
 * 
 * Index:    1
 * Label:    AddTrust External CA Root
 * Subject:  C=SE,O=AddTrust AB,OU=AddTrust External TTP Network,CN=AddTrust External CA Root
 * Domain(s): www.arduino.cc
 */

#define TAs_NUM 2

static const unsigned char TA_DN0[] = {
    0x30, 0x6c, 0x31, 0x0b, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04, 0x06, 0x13,
    0x02, 0x55, 0x53, 0x31, 0x15, 0x30, 0x13, 0x06, 0x03, 0x55, 0x04, 0x0a,
    0x13, 0x0c, 0x44, 0x69, 0x67, 0x69, 0x43, 0x65, 0x72, 0x74, 0x20, 0x49,
    0x6e, 0x63, 0x31, 0x19, 0x30, 0x17, 0x06, 0x03, 0x55, 0x04, 0x0b, 0x13,
    0x10, 0x77, 0x77, 0x77, 0x2e, 0x64, 0x69, 0x67, 0x69, 0x63, 0x65, 0x72,
    0x74, 0x2e, 0x63, 0x6f, 0x6d, 0x31, 0x2b, 0x30, 0x29, 0x06, 0x03, 0x55,
    0x04, 0x03, 0x13, 0x22, 0x44, 0x69, 0x67, 0x69, 0x43, 0x65, 0x72, 0x74,
    0x20, 0x48, 0x69, 0x67, 0x68, 0x20, 0x41, 0x73, 0x73, 0x75, 0x72, 0x61,
    0x6e, 0x63, 0x65, 0x20, 0x45, 0x56, 0x20, 0x52, 0x6f, 0x6f, 0x74, 0x20,
    0x43, 0x41,
};

static const unsigned char TA_RSA_N0[] = {
    0xc6, 0xcc, 0xe5, 0x73, 0xe6, 0xfb, 0xd4, 0xbb, 0xe5, 0x2d, 0x2d, 0x32,
    0xa6, 0xdf, 0xe5, 0x81, 0x3f, 0xc9, 0xcd, 0x25, 0x49, 0xb6, 0x71, 0x2a,
    0xc3, 0xd5, 0x94, 0x34, 0x67, 0xa2, 0x0a, 0x1c, 0xb0, 0x5f, 0x69, 0xa6,
    0x40, 0xb1, 0xc4, 0xb7, 0xb2, 0x8f, 0xd0, 0x98, 0xa4, 0xa9, 0x41, 0x59,
    0x3a, 0xd3, 0xdc, 0x94, 0xd6, 0x3c, 0xdb, 0x74, 0x38, 0xa4, 0x4a, 0xcc,
    0x4d, 0x25, 0x82, 0xf7, 0x4a, 0xa5, 0x53, 0x12, 0x38, 0xee, 0xf3, 0x49,
    0x6d, 0x71, 0x91, 0x7e, 0x63, 0xb6, 0xab, 0xa6, 0x5f, 0xc3, 0xa4, 0x84,
    0xf8, 0x4f, 0x62, 0x51, 0xbe, 0xf8, 0xc5, 0xec, 0xdb, 0x38, 0x92, 0xe3,
    0x06, 0xe5, 0x08, 0x91, 0x0c, 0xc4, 0x28, 0x41, 0x55, 0xfb, 0xcb, 0x5a,
    0x89, 0x15, 0x7e, 0x71, 0xe8, 0x35, 0xbf, 0x4d, 0x72, 0x09, 0x3d, 0xbe,
    0x3a, 0x38, 0x50, 0x5b, 0x77, 0x31, 0x1b, 0x8d, 0xb3, 0xc7, 0x24, 0x45,
    0x9a, 0xa7, 0xac, 0x6d, 0x00, 0x14, 0x5a, 0x04, 0xb7, 0xba, 0x13, 0xeb,
    0x51, 0x0a, 0x98, 0x41, 0x41, 0x22, 0x4e, 0x65, 0x61, 0x87, 0x81, 0x41,
    0x50, 0xa6, 0x79, 0x5c, 0x89, 0xde, 0x19, 0x4a, 0x57, 0xd5, 0x2e, 0xe6,
    0x5d, 0x1c, 0x53, 0x2c, 0x7e, 0x98, 0xcd, 0x1a, 0x06, 0x16, 0xa4, 0x68,
    0x73, 0xd0, 0x34, 0x04, 0x13, 0x5c, 0xa1, 0x71, 0xd3, 0x5a, 0x7c, 0x55,
    0xdb, 0x5e, 0x64, 0xe1, 0x37, 0x87, 0x30, 0x56, 0x04, 0xe5, 0x11, 0xb4,
    0x29, 0x80, 0x12, 0xf1, 0x79, 0x39, 0x88, 0xa2, 0x02, 0x11, 0x7c, 0x27,
    0x66, 0xb7, 0x88, 0xb7, 0x78, 0xf2, 0xca, 0x0a, 0xa8, 0x38, 0xab, 0x0a,
    0x64, 0xc2, 0xbf, 0x66, 0x5d, 0x95, 0x84, 0xc1, 0xa1, 0x25, 0x1e, 0x87,
    0x5d, 0x1a, 0x50, 0x0b, 0x20, 0x12, 0xcc, 0x41, 0xbb, 0x6e, 0x0b, 0x51,
    0x38, 0xb8, 0x4b, 0xcb,
};

static const unsigned char TA_RSA_E0[] = {
    0x01, 0x00, 0x01,
};

static const unsigned char TA_DN1[] = {
    0x30, 0x6f, 0x31, 0x0b, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04, 0x06, 0x13,
    0x02, 0x53, 0x45, 0x31, 0x14, 0x30, 0x12, 0x06, 0x03, 0x55, 0x04, 0x0a,
    0x13, 0x0b, 0x41, 0x64, 0x64, 0x54, 0x72, 0x75, 0x73, 0x74, 0x20, 0x41,
    0x42, 0x31, 0x26, 0x30, 0x24, 0x06, 0x03, 0x55, 0x04, 0x0b, 0x13, 0x1d,
    0x41, 0x64, 0x64, 0x54, 0x72, 0x75, 0x73, 0x74, 0x20, 0x45, 0x78, 0x74,
    0x65, 0x72, 0x6e, 0x61, 0x6c, 0x20, 0x54, 0x54, 0x50, 0x20, 0x4e, 0x65,
    0x74, 0x77, 0x6f, 0x72, 0x6b, 0x31, 0x22, 0x30, 0x20, 0x06, 0x03, 0x55,
    0x04, 0x03, 0x13, 0x19, 0x41, 0x64, 0x64, 0x54, 0x72, 0x75, 0x73, 0x74,
    0x20, 0x45, 0x78, 0x74, 0x65, 0x72, 0x6e, 0x61, 0x6c, 0x20, 0x43, 0x41,
    0x20, 0x52, 0x6f, 0x6f, 0x74,
};

static const unsigned char TA_RSA_N1[] = {
    0xb7, 0xf7, 0x1a, 0x33, 0xe6, 0xf2, 0x00, 0x04, 0x2d, 0x39, 0xe0, 0x4e,
    0x5b, 0xed, 0x1f, 0xbc, 0x6c, 0x0f, 0xcd, 0xb5, 0xfa, 0x23, 0xb6, 0xce,
    0xde, 0x9b, 0x11, 0x33, 0x97, 0xa4, 0x29, 0x4c, 0x7d, 0x93, 0x9f, 0xbd,
    0x4a, 0xbc, 0x93, 0xed, 0x03, 0x1a, 0xe3, 0x8f, 0xcf, 0xe5, 0x6d, 0x50,
    0x5a, 0xd6, 0x97, 0x29, 0x94, 0x5a, 0x80, 0xb0, 0x49, 0x7a, 0xdb, 0x2e,
    0x95, 0xfd, 0xb8, 0xca, 0xbf, 0x37, 0x38, 0x2d, 0x1e, 0x3e, 0x91, 0x41,
    0xad, 0x70, 0x56, 0xc7, 0xf0, 0x4f, 0x3f, 0xe8, 0x32, 0x9e, 0x74, 0xca,
    0xc8, 0x90, 0x54, 0xe9, 0xc6, 0x5f, 0x0f, 0x78, 0x9d, 0x9a, 0x40, 0x3c,
    0x0e, 0xac, 0x61, 0xaa, 0x5e, 0x14, 0x8f, 0x9e, 0x87, 0xa1, 0x6a, 0x50,
    0xdc, 0xd7, 0x9a, 0x4e, 0xaf, 0x05, 0xb3, 0xa6, 0x71, 0x94, 0x9c, 0x71,
    0xb3, 0x50, 0x60, 0x0a, 0xc7, 0x13, 0x9d, 0x38, 0x07, 0x86, 0x02, 0xa8,
    0xe9, 0xa8, 0x69, 0x26, 0x18, 0x90, 0xab, 0x4c, 0xb0, 0x4f, 0x23, 0xab,
    0x3a, 0x4f, 0x84, 0xd8, 0xdf, 0xce, 0x9f, 0xe1, 0x69, 0x6f, 0xbb, 0xd7,
    0x42, 0xd7, 0x6b, 0x44, 0xe4, 0xc7, 0xad, 0xee, 0x6d, 0x41, 0x5f, 0x72,
    0x5a, 0x71, 0x08, 0x37, 0xb3, 0x79, 0x65, 0xa4, 0x59, 0xa0, 0x94, 0x37,
    0xf7, 0x00, 0x2f, 0x0d, 0xc2, 0x92, 0x72, 0xda, 0xd0, 0x38, 0x72, 0xdb,
    0x14, 0xa8, 0x45, 0xc4, 0x5d, 0x2a, 0x7d, 0xb7, 0xb4, 0xd6, 0xc4, 0xee,
    0xac, 0xcd, 0x13, 0x44, 0xb7, 0xc9, 0x2b, 0xdd, 0x43, 0x00, 0x25, 0xfa,
    0x61, 0xb9, 0x69, 0x6a, 0x58, 0x23, 0x11, 0xb7, 0xa7, 0x33, 0x8f, 0x56,
    0x75, 0x59, 0xf5, 0xcd, 0x29, 0xd7, 0x46, 0xb7, 0x0a, 0x2b, 0x65, 0xb6,
    0xd3, 0x42, 0x6f, 0x15, 0xb2, 0xb8, 0x7b, 0xfb, 0xef, 0xe9, 0x5d, 0x53,
    0xd5, 0x34, 0x5a, 0x27,
};

static const unsigned char TA_RSA_E1[] = {
    0x01, 0x00, 0x01,
};

static const br_x509_trust_anchor TAs[] = {
    {
        { (unsigned char *)TA_DN0, sizeof TA_DN0 },
        BR_X509_TA_CA,
        {
            BR_KEYTYPE_RSA,
            { .rsa = {
                (unsigned char *)TA_RSA_N0, sizeof TA_RSA_N0,
                (unsigned char *)TA_RSA_E0, sizeof TA_RSA_E0,
            } }
        }
    },
    {
        { (unsigned char *)TA_DN1, sizeof TA_DN1 },
        BR_X509_TA_CA,
        {
            BR_KEYTYPE_RSA,
            { .rsa = {
                (unsigned char *)TA_RSA_N1, sizeof TA_RSA_N1,
                (unsigned char *)TA_RSA_E1, sizeof TA_RSA_E1,
            } }
        }
    },
};

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* ifndef _CERTIFICATES_H_ */