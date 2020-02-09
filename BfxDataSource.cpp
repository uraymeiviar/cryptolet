#include "BfxDataSource.h"

const char bfxCert[] = \
"-----BEGIN CERTIFICATE-----\n\
MIIDdzCCAl+gAwIBAgIEAgAAuTANBgkqhkiG9w0BAQUFADBaMQswCQYDVQQGEwJJ\n\
RTESMBAGA1UEChMJQmFsdGltb3JlMRMwEQYDVQQLEwpDeWJlclRydXN0MSIwIAYD\n\
VQQDExlCYWx0aW1vcmUgQ3liZXJUcnVzdCBSb290MB4XDTAwMDUxMjE4NDYwMFoX\n\
DTI1MDUxMjIzNTkwMFowWjELMAkGA1UEBhMCSUUxEjAQBgNVBAoTCUJhbHRpbW9y\n\
ZTETMBEGA1UECxMKQ3liZXJUcnVzdDEiMCAGA1UEAxMZQmFsdGltb3JlIEN5YmVy\n\
VHJ1c3QgUm9vdDCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAKMEuyKr\n\
mD1X6CZymrV51Cni4eiVgLGw41uOKymaZN+hXe2wCQVt2yguzmKiYv60iNoS6zjr\n\
IZ3AQSsBUnuId9Mcj8e6uYi1agnnc+gRQKfRzMpijS3ljwumUNKoUMMo6vWrJYeK\n\
mpYcqWe4PwzV9/lSEy/CG9VwcPCPwBLKBsua4dnKM3p31vjsufFoREJIE9LAwqSu\n\
XmD+tqYF/LTdB1kC1FkYmGP1pWPgkAx9XbIGevOF6uvUA65ehD5f/xXtabz5OTZy\n\
dc93Uk3zyZAsuT3lySNTPx8kmCFcB5kpvcY67Oduhjprl3RjM71oGDHweI12v/ye\n\
jl0qhqdNkNwnGjkCAwEAAaNFMEMwHQYDVR0OBBYEFOWdWTCCR1jMrPoIVDaGezq1\n\
BE3wMBIGA1UdEwEB/wQIMAYBAf8CAQMwDgYDVR0PAQH/BAQDAgEGMA0GCSqGSIb3\n\
DQEBBQUAA4IBAQCFDF2O5G9RaEIFoN27TyclhAO992T9Ldcw46QQF+vaKSm2eT92\n\
9hkTI7gQCvlYpNRhcL0EYWoSihfVCr3FvDB81ukMJY2GQE/szKN+OMY3EU/t3Wgx\n\
jkzSswF07r51XgdIGn9w/xZchMB5hbgF/X++ZRGjD8ACtPhSNzkE1akxehi/oCr0\n\
Epn3o0WC4zxe9Z2etciefC7IpJ5OCBRLbf1wbWsaY71k5h+3zvDyny67G7fyUIhz\n\
ksLi4xaNmjICq44Y3ekQEe5+NauQrz4wlHrQMz2nZQ/1/I6eYs9HRCwBXbsdtTLS\n\
R9I4LtD+gdwyah617jzV/OeBHRnDJELqYzmp\
-----END CERTIFICATE-----\n";

BfxDataSource::BfxDataSource(const std::string cur1, const std::string cur2)
    : HttpsDataSource(cur1, cur2)
{
    this->wifiClient.setCACert(bfxCert);
    this->setPair(cur1, cur2);
}


void BfxDataSource::setPair(const std::string cur1, const std::string cur2)
{
    std::string upCur1 = cur1;
    std::transform(upCur1.begin(), upCur1.end(), upCur1.begin(), ::toupper);

    std::string upCur2 = cur2;
    std::transform(upCur2.begin(), upCur2.end(), upCur2.begin(), ::toupper);

    std::string loCur1 = cur1;
    std::transform(loCur1.begin(), loCur1.end(), loCur1.begin(), ::tolower);

    std::string loCur2 = cur2;
    std::transform(loCur2.begin(), loCur2.end(), loCur2.begin(), ::tolower);

    this->name = "BFX "+upCur1+"/"+upCur2;
    this->url = "https://api-pub.bitfinex.com/v2/ticker/t" + upCur1 + upCur2 + "/";

    this->firstCurrency = loCur1;
    this->secondCurrency = loCur2;
}

std::string BfxDataSource::getName() const
{
    return this->name;
}

std::string BfxDataSource::httpGetUrl() const
{
    return this->url;
}

DataParser* BfxDataSource::getParser() const
{
    return (DataParser*)&this->parser;
}

std::string BfxDataSource::getHostName() const
{
    return "api-pub.bitfinex.com";
}