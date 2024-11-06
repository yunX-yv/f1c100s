#ifndef __BAIDUAPI_H__
#define __BAIDUAPI_H__

#include <stdio.h>
#include <iostream>
#include <string.h>
#include "curl/curl.h"

using namespace std;

#define BAIDU_TTS 1
#define BAIDU_ASR 0


class BaiduOpenAPI
{
private:

    string accessToken;

    const string api_key ;

    const string secret_key ;

    string token_org ;

    RET_ERR get_token();

#if  BAIDU_TTS == 1
    const string TTS_url = "http://tsn.baidu.com/text2audio";
    string tts_url_opt ;
    RET_ERR tts();

#endif

#if  BAIDU_ASR == 1
    const string ASR_url = "http://vop.baidu.com/server_api?dev_pid=1537&cuid=1234567&token=";
    string asr_url_opt ;
    RET_ERR asr();

#endif

public:

//    BaiduOpenAPI();
    BaiduOpenAPI(string api_key, string secret_key) : api_key(api_key), secret_key(secret_key){
        token_org = "http://aip.baidubce.com/oauth/2.0/token?grant_type=client_credentials&client_id="+api_key+"&client_secret="+secret_key;
        get_token();

#if  BAIDU_TTS == 1
        tts();
#endif

#if  BAIDU_ASR == 1
        asr();
#endif

    }
    ~BaiduOpenAPI();
}

#endif