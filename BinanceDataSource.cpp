#include "BinanceDataSource.h"
#include <algorithm>

const char binanceCert[] = \
"-----BEGIN CERTIFICATE-----\n\
MIIDrzCCApegAwIBAgIQCDvgVpBCRrGhdWrJWZHHSjANBgkqhkiG9w0BAQUFADBh\n\
MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3\n\
d3cuZGlnaWNlcnQuY29tMSAwHgYDVQQDExdEaWdpQ2VydCBHbG9iYWwgUm9vdCBD\n\
QTAeFw0wNjExMTAwMDAwMDBaFw0zMTExMTAwMDAwMDBaMGExCzAJBgNVBAYTAlVT\n\
MRUwEwYDVQQKEwxEaWdpQ2VydCBJbmMxGTAXBgNVBAsTEHd3dy5kaWdpY2VydC5j\n\
b20xIDAeBgNVBAMTF0RpZ2lDZXJ0IEdsb2JhbCBSb290IENBMIIBIjANBgkqhkiG\n\
9w0BAQEFAAOCAQ8AMIIBCgKCAQEA4jvhEXLeqKTTo1eqUKKPC3eQyaKl7hLOllsB\n\
CSDMAZOnTjC3U/dDxGkAV53ijSLdhwZAAIEJzs4bg7/fzTtxRuLWZscFs3YnFo97\n\
nh6Vfe63SKMI2tavegw5BmV/Sl0fvBf4q77uKNd0f3p4mVmFaG5cIzJLv07A6Fpt\n\
43C/dxC//AH2hdmoRBBYMql1GNXRor5H4idq9Joz+EkIYIvUX7Q6hL+hqkpMfT7P\n\
T19sdl6gSzeRntwi5m3OFBqOasv+zbMUZBfHWymeMr/y7vrTC0LUq7dBMtoM1O/4\n\
gdW7jVg/tRvoSSiicNoxBN33shbyTApOB6jtSj1etX+jkMOvJwIDAQABo2MwYTAO\n\
BgNVHQ8BAf8EBAMCAYYwDwYDVR0TAQH/BAUwAwEB/zAdBgNVHQ4EFgQUA95QNVbR\n\
TLtm8KPiGxvDl7I90VUwHwYDVR0jBBgwFoAUA95QNVbRTLtm8KPiGxvDl7I90VUw\n\
DQYJKoZIhvcNAQEFBQADggEBAMucN6pIExIK+t1EnE9SsPTfrgT1eXkIoyQY/Esr\n\
hMAtudXH/vTBH1jLuG2cenTnmCmrEbXjcKChzUyImZOMkXDiqw8cvpOp/2PV5Adg\n\
06O/nVsJ8dWO41P0jmP6P6fbtGbfYmbW0W5BjfIttep3Sp+dWOIrWcBAI+0tKIJF\n\
PnlUkiaY4IBIqDfv8NZ5YBberOgOzW6sRBc4L0na4UU+Krk2U886UAb3LujEV0ls\n\
YSEY1QSteDwsOoBrp+uvFRTp2InBuThs4pFsiv9kuXclVzDAGySj4dzp30d8tbQk\n\
CAUw7C29C79Fv1C5qfPrmAESrciIxpg0X40KPMbp1ZWVbd4=\n\
-----END CERTIFICATE-----\n";

BinanceDataSource::BinanceDataSource(const std::string cur1, const std::string cur2)
    : HttpsDataSource(cur1, cur2)
{
    this->wifiClient.setCACert(binanceCert);
    this->setPair(cur1, cur2);
}

void BinanceDataSource::setPair(const std::string cur1, const std::string cur2)
{
    std::string upCur1 = cur1;
    std::transform(upCur1.begin(), upCur1.end(), upCur1.begin(), ::toupper);

    std::string upCur2 = cur2;
    std::transform(upCur2.begin(), upCur2.end(), upCur2.begin(), ::toupper);

    std::string loCur1 = cur1;
    std::transform(loCur1.begin(), loCur1.end(), loCur1.begin(), ::tolower);

    std::string loCur2 = cur2;
    std::transform(loCur2.begin(), loCur2.end(), loCur2.begin(), ::tolower);

    this->name = "Binance "+upCur1+"/"+upCur2;
    this->url = "https://api.binance.com/api/v3/ticker/price?symbol="+upCur1+upCur2;

    this->firstCurrency = loCur1;
    this->secondCurrency = loCur2;
}

std::string BinanceDataSource::getName() const
{
    return this->name;
}

std::string BinanceDataSource::httpGetUrl() const
{
    return this->url;
}

DataParser* BinanceDataSource::getParser() const
{
    return (DataParser*)&this->parser;
}

std::string BinanceDataSource::getHostName() const
{
    return "api.binance.com";
}