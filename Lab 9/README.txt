The following commands were run to generate the private and public keys:
openssl genrsa -out private-key.pem 2048
openssl rsa -in private-key.pem -pubout -out public-key.pem

The following commands were run to compile the encrypter and decrypter programs:
gcc encrypter.c -lssl -lcrypto -o e.out
gcc decrypter.c -lssl -lcrypto -o d.out

The following command was run to encrypt the file: 
./e.out "-----BEGIN PUBLIC KEY-----
MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAwOmkAMp/mnELQTh+YX2M
YYbHu0394bUdRlkrvws1t8+l6F3k0cuo3Qs4zQi87IflyG0v5oa2Suyi901DTmDy
XOE6BZDoDFuSS8a4tGliV5NpURke4862vkhP632RZAGOBzS6mvTOf6A8NWfWh1bO
sNyeI1h2+7fHWhNnlIQsFOnpAmaXLdj+vFVio+WzgbOeBbvVpDLf0slfrNIJtkhC
QrIDlFLXjnrzO6x7lKUPfBX5zdDFm/+D9qKmdx9I1Wp01I3PUqJt/N9l9xrxCpNW
FUeqDC6oJEtUu2BpXqr6LVr+yeRLyjdO8gf0RBKQMLLfKsNrwocD6IirWboE+Fzy
9wIDAQAB
-----END PUBLIC KEY-----
" inputfile.txt encryptedfile.txt

The following command was run to decrypt the file: 
./d.out "-----BEGIN RSA PRIVATE KEY-----
MIIEpAIBAAKCAQEAwOmkAMp/mnELQTh+YX2MYYbHu0394bUdRlkrvws1t8+l6F3k
0cuo3Qs4zQi87IflyG0v5oa2Suyi901DTmDyXOE6BZDoDFuSS8a4tGliV5NpURke
4862vkhP632RZAGOBzS6mvTOf6A8NWfWh1bOsNyeI1h2+7fHWhNnlIQsFOnpAmaX
Ldj+vFVio+WzgbOeBbvVpDLf0slfrNIJtkhCQrIDlFLXjnrzO6x7lKUPfBX5zdDF
m/+D9qKmdx9I1Wp01I3PUqJt/N9l9xrxCpNWFUeqDC6oJEtUu2BpXqr6LVr+yeRL
yjdO8gf0RBKQMLLfKsNrwocD6IirWboE+Fzy9wIDAQABAoIBAQCVWc0g16VmujIO
cmBVtc9jCefD+SOfBIr/lbWqmFEkV1yIw3iNe/DmAPQeuK8bKDMbsnytKoojA4U/
lM+Q9Llucuhqur8MIjDm/ZiuBBCg4H1ezeX+VGFie7V0CajwFcwRhB8cLH9ai4FR
j2ylJu2qO4MqtiiXxLZP5EoCzzkZjozWFW36SrX83wvfntJ54WR5pa5rzljBm+5w
QIiguJhWFgFuwCTSF0vJcQ7ACww8n8/W1Zdj7e7w1MEkXRkqN/5hxz+bzQ6SW17V
vI/GB6b5x2L1mg94O3nF3CNVTmMmiaXGfYjjKmht5nsLkFGRA4A3/d3HlZSocaed
OPEXqmARAoGBAPNJ8xGtEHA3cRrXMzY41c0+sWw+d3Iv5jlNWWmpixM7EUoZVYdu
3q49CimnLUDTVzBctT4JXBb4j95bfS7qE2RNrMyo5PZpj18VK5bRR7YjSMUKu7Q1
4gQm6BPw2EvUy7EDAcht74LhhCn1KBB9/kR0igSo4vWP5iYNJztO7zz1AoGBAMr9
5YUUpOnn481sCA+htXMTqUjyd92lsGwCqKoNwxoRNm2C2kJB1rUaQlU7XzlaglGl
y7p25DIjOFc5R458Cj0CZ8ol2L3IKOXZVJF1knqm0MO9lN+i/wP7a2GALhVhjACh
TJLBgeS0j326HdOJtdGzVndUUD+kEvHicWUUyjy7AoGARPRtt1HU+elM3m2mLzUW
rBCb2vEti6i12QHqWL9cErxIHzdk/XjYFoxoZ/vhIKGF8cgk+otOlZgntr85kOV5
T5GQa4VnN8wdfbaTXzC74YzIoEh1gRNQlxqr5pL5yyxp+ya+B5/ZJcBjZDz++QWs
3pwlTkLL6Qu7EIfj2bR6rE0CgYEAqNJaVcDimupRyiGj3qRFzSw36URGrl9Dh8D6
V1vUvFQ+aPuf2+T31Wy+aDZVrtR4d8Ek27JPfOdPzzMDMRw2Q6FBgpzRUp0qbma+
b69RoJImI/UC4K+l2cD7AH7jVMLo9gkwLwmipvwILq9gaPMp9Ppq2hmUGCmz29nR
JxkQTUsCgYA9Mt5paOEtWAq38uVbXh+SqZylhohMHHFvSEUuAvik2SsYx1j+zn+3
CPk/7DyQUjgR5en3A0W2faVmyDM8x5o8Nx8ksPPSoikCXgrv0G2rbXxSWRTxpOQ1
BjmjNSAJ0kwfguTCD6ntGn4D68ovv1krP6v4esPqsc+GoaWPYsSmmg==
-----END RSA PRIVATE KEY-----
" encryptedfile.txt outputfile.txt


Note:
1. Using a key size of 8192 results in segmentation faults. Probably due to memory issues. So a key size of 2048 has been used. 
This means that the input file cannot be too large. The provided sample input file has 107 characters.
2. Additionally, the decryption program randomly fails with a decoding error. This is despite running the same encryption and decryption programs.
In case this happens, please re run both the encryption and decryption program a few of times. Generally it works fine at least 2 out of 3 times. 
But occassionally, it takes quite a few more tries. But the decryption program does work correctly on the provided encrypted file.