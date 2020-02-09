#include "IndodaxDataSource.h"
#include <algorithm>

const char indodaxCert[] = \
"-----BEGIN CERTIFICATE-----\n\
MIIENjCCAx6gAwIBAgIBATANBgkqhkiG9w0BAQUFADBvMQswCQYDVQQGEwJTRTEU\n\
MBIGA1UEChMLQWRkVHJ1c3QgQUIxJjAkBgNVBAsTHUFkZFRydXN0IEV4dGVybmFs\n\
IFRUUCBOZXR3b3JrMSIwIAYDVQQDExlBZGRUcnVzdCBFeHRlcm5hbCBDQSBSb290\n\
MB4XDTAwMDUzMDEwNDgzOFoXDTIwMDUzMDEwNDgzOFowbzELMAkGA1UEBhMCU0Ux\n\
FDASBgNVBAoTC0FkZFRydXN0IEFCMSYwJAYDVQQLEx1BZGRUcnVzdCBFeHRlcm5h\n\
bCBUVFAgTmV0d29yazEiMCAGA1UEAxMZQWRkVHJ1c3QgRXh0ZXJuYWwgQ0EgUm9v\n\
dDCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALf3GjPm8gAELTngTlvt\n\
H7xsD821+iO2zt6bETOXpClMfZOfvUq8k+0DGuOPz+VtUFrWlymUWoCwSXrbLpX9\n\
uMq/NzgtHj6RQa1wVsfwTz/oMp50ysiQVOnGXw94nZpAPA6sYapeFI+eh6FqUNzX\n\
mk6vBbOmcZSccbNQYArHE504B4YCqOmoaSYYkKtMsE8jqzpPhNjfzp/haW+710LX\n\
a0Tkx63ubUFfclpxCDezeWWkWaCUN/cALw3CknLa0Dhy2xSoRcRdKn23tNbE7qzN\n\
E0S3ySvdQwAl+mG5aWpYIxG3pzOPVnVZ9c0p10a3CitlttNCbxWyuHv77+ldU9U0\n\
WicCAwEAAaOB3DCB2TAdBgNVHQ4EFgQUrb2YejS0Jvf6xCZU7wO94CTLVBowCwYD\n\
VR0PBAQDAgEGMA8GA1UdEwEB/wQFMAMBAf8wgZkGA1UdIwSBkTCBjoAUrb2YejS0\n\
Jvf6xCZU7wO94CTLVBqhc6RxMG8xCzAJBgNVBAYTAlNFMRQwEgYDVQQKEwtBZGRU\n\
cnVzdCBBQjEmMCQGA1UECxMdQWRkVHJ1c3QgRXh0ZXJuYWwgVFRQIE5ldHdvcmsx\n\
IjAgBgNVBAMTGUFkZFRydXN0IEV4dGVybmFsIENBIFJvb3SCAQEwDQYJKoZIhvcN\n\
AQEFBQADggEBALCb4IUlwtYj4g+WBpKdQZic2YR5gdkeWxQHIzZlj7DYd7usQWxH\n\
YINRsPkyPef89iYTx4AWpb9a/IfPeHmJIZriTAcKhjW88t5RxNKWt9x+Tu5w/Rw5\n\
6wwCURQtjr0W4MHfRnXnJK3s9EK0hZNwEGe6nQY1ShjTK3rMUUKhemPR5ruhxSvC\n\
Nr4TDea9Y355e6cJDUCrat2PisP29owaQgVR1EX1n6diIWgVIEM8med8vSTYqZEX\n\
c4g/VhsxOBi0cQ+azcgOno4uG+GMmIPLHzHxREzGBHNJdmAPx/i9F4BrLunMTA5a\n\
mnkPIAou1Z5jJh5VkpTYghdae9C8x49OhgQ=\n\
-----END CERTIFICATE-----\n";

IndodaxDataSource::IndodaxDataSource(const std::string cur1, const std::string cur2)
    : HttpsDataSource(cur1, cur2)
{
    this->wifiClient.setCACert(indodaxCert);
    this->setPair(cur1, cur2);
}

void IndodaxDataSource::setPair(const std::string cur1, const std::string cur2)
{
    std::string upCur1 = cur1;
    std::transform(upCur1.begin(), upCur1.end(), upCur1.begin(), ::toupper);

    std::string upCur2 = cur2;
    std::transform(upCur2.begin(), upCur2.end(), upCur2.begin(), ::toupper);

    std::string loCur1 = cur1;
    std::transform(loCur1.begin(), loCur1.end(), loCur1.begin(), ::tolower);

    std::string loCur2 = cur2;
    std::transform(loCur2.begin(), loCur2.end(), loCur2.begin(), ::tolower);

    this->name = "Indodax "+upCur1+"/"+upCur2;
    this->url = "https://vip.bitcoin.co.id/api/"+loCur1+"_"+loCur2+"/ticker/";

    this->firstCurrency = loCur1;
    this->secondCurrency = loCur2;
}

std::string IndodaxDataSource::getName() const
{
    return this->name;
}

std::string IndodaxDataSource::httpGetUrl() const
{
    return this->url;
}

DataParser* IndodaxDataSource::getParser() const
{
    return (DataParser*)&this->parser;
}

std::string IndodaxDataSource::getHostName() const
{
    return "vip.bitcoin.co.id";
}