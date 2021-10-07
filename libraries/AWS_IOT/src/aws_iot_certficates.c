// choi++20201101 modified for aws-iot

/*
 * Copyright 2010-2015 Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * Additions Copyright 2016 Espressif Systems (Shanghai) PTE LTD
 *
 * Licensed under the Apache License, Version 2.0 (the "License").
 * You may not use this file except in compliance with the License.
 * A copy of the License is located at
 *
 *  http://aws.amazon.com/apache2.0
 *
 * or in the "license" file accompanying this file. This file is distributed
 * on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied. See the License for the specific language governing
 * permissions and limitations under the License.
 */

/**
 * @file aws_iot_certifcates.c
 * @brief File to store the AWS certificates in the form of arrays
 */

#ifdef __cplusplus
extern "C" {
#endif

const char aws_root_ca_pem[] = {"-----BEGIN CERTIFICATE-----\n\
MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF\n\
ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6\n\
b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL\n\
MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv\n\
b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj\n\
ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM\n\
9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw\n\
IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6\n\
VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L\n\
93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm\n\
jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC\n\
AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA\n\
A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI\n\
U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs\n\
N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv\n\
o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU\n\
5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy\n\
rqXRfboQnoZsG4q5WTP468SQvvG5\n\
-----END CERTIFICATE-----\n"};

const char certificate_pem_crt[] = {"-----BEGIN CERTIFICATE-----\n\
MIIDWTCCAkGgAwIBAgIUTBDEcuMbTaRjwLAwZDimdhc/1XIwDQYJKoZIhvcNAQEL\n\
BQAwTTFLMEkGA1UECwxCQW1hem9uIFdlYiBTZXJ2aWNlcyBPPUFtYXpvbi5jb20g\n\
SW5jLiBMPVNlYXR0bGUgU1Q9V2FzaGluZ3RvbiBDPVVTMB4XDTIwMTAyNjA3Mzky\n\
M1oXDTQ5MTIzMTIzNTk1OVowHjEcMBoGA1UEAwwTQVdTIElvVCBDZXJ0aWZpY2F0\n\
ZTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAM6R++L2OXDVyKwstlif\n\
XMtiX8T15ZWmpS6cWUJJFnPPORoE/dBZC7rh91IlQAFkj3f8Yqb6L6GJN5RDrMox\n\
8x+1KYtnbEow2wVRX9HCm05x1WOrMZblF8ax/fvFVsTceaDRpOU3DI1ARPfsgL8u\n\
vYZDPw4cN959+Q/gK1/GQKYwKP2UWeC67vaopBUhqDC4WoNYpfWjMJxuv4rfr8c+\n\
57FELtjcDnFbdL5PBZVpZbddqV34EIc1HBUYMbBsnkhqEm3sb10ADV34dxvjwelq\n\
jiBJyuz2VI2DTJu8eFTjMdBiyNds+HgvE0adS6Z82D/tBTNS2SLdm2YKvUHsn/Ep\n\
cK8CAwEAAaNgMF4wHwYDVR0jBBgwFoAU+z0xw2XpPtMP3lCTY4EQiHSynh4wHQYD\n\
VR0OBBYEFCzJOObEDrlY+yIrrxiF5UbeIVfKMAwGA1UdEwEB/wQCMAAwDgYDVR0P\n\
AQH/BAQDAgeAMA0GCSqGSIb3DQEBCwUAA4IBAQA19PQSsvkzWBqMpQxv2XczeTvd\n\
Ihl/1MzHefROMNYK5tbMHddUeqrD3G6p73gscj3/B0aMjFyTTU/N9hLB36HpE3AO\n\
+AvDDjqVFeKZ5e+6DvTBSY44O4zgpzFnv7kI1NplnA+zwOkXOwr4dZrlj8qhR2mx\n\
Y28OywBKCLzUk7NtTQflaS/V4CDFPRM2y5c0/VVSwh56tcb6ID3uWcr3+npZhcTM\n\
+aJOUv/Di5RmU1+b4hxOyUp5IX/ezdAMeTv1DHpzd3MQjCq1QqTyG2PDxBWa6r9C\n\
S4gz3k8BxZjbCJv1mBCwyImDH3I6KjDops5bND2NI0+mrKNiucbz8m0ButSB\n\
-----END CERTIFICATE-----\n"};

const char private_pem_key[] = {"-----BEGIN RSA PRIVATE KEY-----\n\
MIIEpAIBAAKCAQEAzpH74vY5cNXIrCy2WJ9cy2JfxPXllaalLpxZQkkWc885GgT9\n\
0FkLuuH3UiVAAWSPd/xipvovoYk3lEOsyjHzH7Upi2dsSjDbBVFf0cKbTnHVY6sx\n\
luUXxrH9+8VWxNx5oNGk5TcMjUBE9+yAvy69hkM/Dhw33n35D+ArX8ZApjAo/ZRZ\n\
4Lru9qikFSGoMLhag1il9aMwnG6/it+vxz7nsUQu2NwOcVt0vk8FlWllt12pXfgQ\n\
hzUcFRgxsGyeSGoSbexvXQANXfh3G+PB6WqOIEnK7PZUjYNMm7x4VOMx0GLI12z4\n\
eC8TRp1LpnzYP+0FM1LZIt2bZgq9Qeyf8SlwrwIDAQABAoIBAQCte+0Pr7qOCP1/\n\
nA2IYE6OgQ/tWpZ2GH6hpq1P/hHdMSvDrm17jU1MpnoPgzk98jaLjmtL0NIv2A3V\n\
dwCmz0xgVMWPTkykAmPvE2hlwgqs1u1fx2beU4ikVtf3msREkOWVKnxSBOmFt3Q0\n\
w2tdJOQG3nMWZpyFP0btDldfGCEzb7xj3h7xyIvm4a0jpZY/DC+VZQMa2f8JZayy\n\
uc8jKxhr2Ilw3yv/yt8c9L9PVAb8zOgtofZEO3y4zMhV9297uEsxAg+zBSZ7XWpJ\n\
QG8jy1FXhAbgF3DFAkv4wwO9cAilly8D3U2qV+aSbw+dJ3vrwfOfOmjyyFvf1IN/\n\
wn5/yRABAoGBAO/EB1ipJDY+tuDbki0du7GO3uOj2grjlR4Lj9EEfKGB7oqVrEk5\n\
YJ5+f9aQAoUtNAz9oA6s+cB9VxaSV/+j632reB2K3oG1EpyWhKTSjyN0KKulYfUY\n\
RaDheFsvFfiaO+6o8B34wdftxBnvM9k/+3PwTSIm1S7UjwaG8xnk3XupAoGBANyO\n\
j/fqTrHy+DfjcgoVzbVNFjjLZjLz6qT0tttaARqeI66jJYPiSsSAT/H6mHwlKIu7\n\
Yo55jc8lGv+R4IBW6gJoyvCINfdqXjyS3akgHFw41Hp/ZsAR4LeoVf2XkfU9vkXQ\n\
XYQi+Q9kW2rQCJTwLUEIdssEI3G9UGL9VAZVyYCXAoGAKMB1yzD8cafZJ4DKaJas\n\
JzK0WZao4b7qwm+OoyUTFdRiufVUkiGJdYbkrZBOjVowhaC6CqSF0py7J5/6Cv5O\n\
cU6qnMLUrNEc97Zxlx4erBT7MKlsa98TPpP0A/ON1XJJi3f1T/LO56eG9C1+74b7\n\
cyo9wfc9trb0FFTtjtIifvkCgYBbeypN6zqu5pd8HvPbKQYE0C+KaRs/wRovR21c\n\
+C0DDFrCgdr6o+7dKFeAh6+dto/lvVh4vgRKyhozrR8QPHydeTfLosO3lr/1ofkc\n\
wDvjKJNBJ/zHZ+Lsan8YSzj4Hp8AOBfByfvCHZ8pY5ZbqG8szUTwmgvVjm98T5T9\n\
d7G3iwKBgQCR7ArSOZGItYV6Zigs6NaaNbJ/le+wH/5zoXNqJvIPjSJ7u9078Z87\n\
nTF29COF7AXksxy4YhugywFX6ddtDzgESMPRAxrnIeSFgFQEhYfA6SSU44qyXZ05\n\
mAuCS5A/eQvoELAR41sryZPrT7rNpVkTkYLFCbwz9O/hMT5CNLnLzw==\n\
-----END RSA PRIVATE KEY-----\n"};


#ifdef __cplusplus
}
#endif
