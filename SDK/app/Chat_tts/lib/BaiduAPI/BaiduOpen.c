#include "BaiduAPI.h"
#include "common.h"

RET_ERR BaiduOpenAPI::get_token()
{
    //获取Assess token
    CURLcode res = CURLE_OK;
    RETURN_CODE ret = RETURN_OK;

    CURL *curl = curl_easy_init();
    if(curl) {

        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
        curl_easy_setopt(curl, CURLOPT_URL, token_org.c_str());
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        // curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
        // 打印详情
        // curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, "Accept: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        const char *data = "";
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &accessToken);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, onWriteData);
        res = curl_easy_perform(curl);
        if(res == CURLE_OK)
        {
            Json::Reader reader;
            Json::Value root;
            if (reader.parse(accessToken, root))
            {
                cout << "find token!"<< endl;
                // cout << "token is: " << root["access_token"].asString();
                accessToken = root["access_token"].asString();
            }
        }
        else
        {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            ret = RETURN_ERROR;
        }
       
    }
    curl_easy_cleanup(curl);
    return ret;
}

RET_ERR BaiduOpenAPI::tts()
{
    //获取token
    if (accessToken.length() > 0) {
        cout << "tts Token is: " << accessToken << endl;
    } else {
        cout << "Error: " << res << endl;
        return;
    }

    char params[100];

    // 发音人选择, 基础音库：0为度小美，1为度小宇，3为度逍遥，4为度丫丫，
    // 精品音库：5为度小娇，103为度米朵，106为度博文，110为度小童，111为度小萌，默认为度小美 
    int per = 0;
    // 语速，取值0-9，默认为5中语速
    int spd = 6;
    // #音调，取值0-9，默认为5中语调
    int pit = 5;
    // #音量，取值0-9，默认为5中音量
    int vol = 5;
    // 下载的文件格式, 3：mp3(default) 4： pcm-16k 5： pcm-8k 6. wav
	int aue = 6;
	// aue对应的格式，format
	const char formats[4][4] = {"mp3", "pcm", "pcm", "wav"};
	format = formats[aue - 3];
    std::cout << "formats:"  << format << std::endl;

	char params_pattern[] = "ctp=1&lan=zh&cuid=%s&per=%d&spd=%d&pit=%d&vol=%d&aue=%d&tok=";
    snprintf(params, sizeof(params), params_pattern , cuid.c_str(),
             per, spd, pit, vol, aue);

    tts_url_opt  = params; 
    tts_url_opt  += accessToken;
    
    cout << "parames_url: " << tts_url_opt << endl;
}