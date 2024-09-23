#include <stdio.h>
#include <curl/curl.h>
#include "../lib/asr/asr.h"

using namespace std;

int main(int argc, char **argv)
{
    printf("For f1c100s\r\n");
    printf("curl version:%s\n", curl_version());

    ASR *asr = new ASR();

    return 0;
}
