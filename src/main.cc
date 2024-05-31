#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <unordered_set>
#include <unordered_map>
#include <cstring>
#include <json.hpp>
#include <cpr/cpr.h>
#include <chrono>

// endpoint for translate api
const char *endpoint;

// flag read from command line
int opt;

// options program supports
constexpr char options[] = "ridlkf:q:s:t:h";

// characters for spin progress
constexpr char spinChars[] = "-\\|/";

// support languages
std::unordered_set<const char *> support_langs = {
    "en",
    "zt"};

// support features and its mapping path
enum class Features
{
    TRANSLATE,
    TRANSLATE_FILE,
    DETECT,
    LANGUAGES
};

// Default feature
constexpr Features DEFAULT_FEATURE = Features::TRANSLATE;

std::unordered_map<Features, const char *> support_features = {
    {Features::TRANSLATE, "/translate"},
    {Features::TRANSLATE_FILE, "/translate_file"},
    {Features::DETECT, "/detect"},
    {Features::LANGUAGES, "/languages"}};

// error handler function
inline void errIf(bool, const char *);

// usage for program
inline void usage();

// translate text
inline std::string translateText(const std::string& url, const std::string& req);

inline void showSpinProcess(std::future<std::string>& future_work);

class QueryRequest
{
public:
    QueryRequest() {}
    QueryRequest(const std::string &q, const std::string &source_lang, const std::string &target_lang,
                 const std::string &format, const std::string &api_key)
        : q_(q), source_lang_(source_lang), target_lang_(target_lang), format_(format), api_key_(api_key.empty() ? "" : api_key)
    {
    }
    ~QueryRequest() {}

public:
    void build_json(nlohmann::json &j) noexcept
    {
        j = {
            {"q", q_},
            {"source", source_lang_},
            {"target", target_lang_},
            {"format", format_},
            {"api_key", api_key_}};
#ifdef _DEBUG
        std::cout << "Result of building json: "
                  << '\n'
                  << j.dump()
                  << std::endl;
#endif
    }



private:
    std::string q_;
    std::string source_lang_;
    std::string target_lang_;
    std::string format_;
    std::string api_key_;
};

class QueryResponse
{
public:
    void from_json(const std::string &resp) noexcept
    {
        nlohmann::json respJson = nlohmann::json::parse(resp);
#ifdef _DEBUG
        std::cout << "Parsed response: "
                  << std::endl
                  << respJson.dump()
                  << std::endl;
#endif
        respJson
            .at("translatedText")
            .get_to(translated_text_);
    }

    const std::string &getTranslateText() const
    {
        return translated_text_;
    }

private:
    std::string translated_text_;
};

int main(int argc, char *const *argv)
{
    
    // read config from environment
    endpoint = getenv("TTL_ENDPOINT");
    const char *api_key = getenv("TTL_API_KEY");
    errIf(endpoint == NULL, "Not found environment variable: TTL_ENDPOINT");

#ifdef _DEBUG
    std::cout << "TTL_ENDPOINT: " << endpoint << std::endl;
#endif
    
    char* source_lang = NULL;
    char* target_lang = NULL;
    char* format = NULL;
    char* query = NULL;
    bool key_use = false;
    Features feature_use = DEFAULT_FEATURE;

    try
    {
        while ((opt = getopt(argc, argv, options)) != -1)
        {
            switch (opt)
            {
            case 'h':
            {
                usage();
                exit(0);
            }
            case 's':
            {
                source_lang = optarg;
                break;
            }
            case 't':
            {
                target_lang = optarg;
                break;
            }
            case 'f':
            {
                format = optarg;
                break;
            }
            case 'q':
            {
                query = optarg;
                break;
            }
            case 'k':
            {
                key_use = true;
                break;
            }
            case 'r':
            {
                break;
            }
            case 'i':
            {
                feature_use = Features::TRANSLATE_FILE;
                break;
            }
            case 'd': 
            {
                feature_use = Features::DETECT;
                break;
            }
            case 'l':
            {
                feature_use = Features::LANGUAGES;
                break;
            }
            case '?':
                fprintf(stderr, "Unknown command: -%c\n", optopt);
                exit(1);
            }
        }

        if (source_lang == NULL || target_lang == NULL ||
            query == NULL || format == NULL)
        {
            std::cerr << "These options are required: [-s|-t|-q|-f]" << std::endl;
            usage();
            exit(1);
        }

        if (key_use) {
            errIf(api_key == NULL, "Api key use, but cannot find relevant environment variable.");
        }

        auto feature_location = support_features.find(feature_use);

        assert(feature_location != support_features.end());

        std::stringstream complete_url;
        complete_url << endpoint << support_features.find(feature_use)->second;

#ifdef _DEBUG
        std::cout << "Complete URL: "
                  << complete_url.str()
                  << std::endl;
#endif

        // construct request for translate api
        QueryRequest req(
            query,
            source_lang,
            target_lang,
            format,
            api_key == NULL ? "" : api_key
        );
        nlohmann::json reqJson;
        req.build_json(reqJson);

        // send post request to api server
        std::future<std::string> translateWork = std::async(
            std::launch::async,
            translateText,
            complete_url.str(),
            reqJson.dump());

        std::thread progressThread(showSpinProcess, std::ref(translateWork));

        // output to terminal
        std::cout << translateWork.get() << std::endl;

        progressThread.join();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        exit(1);
    }
    exit(0);
}

inline void errIf(bool cond, const char *msg)
{
    if (cond)
    {
        std::cerr << msg << std::endl;
        exit(1);
    }
}

inline void usage() {
    std::cout << "Usage: "
              << "\n\t"
              << "ttl [-s|-t|-f|-k|-h|-q] [word]"
              << '\n'
              << "Description: "
              << '\n'
              << "-s source language [string]"
              << '\n'
              << "-t target language [string]"
              << '\n'
              << "-f format of input [string]"
              << '\n'
              << "-k whether use api_key or not, if set, it use, in contrast, not use."
              << '\n'
              << "-h help"
              << '\n'
              << "-q query text [string]"
              << std::endl;
}

inline std::string translateText(const std::string &url, const std::string &req)
{
    cpr::Response rawResp = cpr::Post(
            cpr::Url(url),
            cpr::Body(req),
            cpr::Header{
                {"Content-Type", "application/json"}
            }
        );

#ifdef _DEBUG
        std::cout << "Response: "
                  << rawResp.text
                  << std::endl;
#endif

        // parse response
        QueryResponse response;
        response.from_json(rawResp.text);

        // std::this_thread::sleep_for(std::chrono::seconds(3));

        std::cout << std::endl;

        return response.getTranslateText();
}

inline void showSpinProcess(std::future<std::string> &future_work)
{
    int i = 0;
    do
    {
        std::cout << "\r" << spinChars[i % 4] << " Translating... " << std::flush;
        ++i;
    } while (future_work.wait_for(std::chrono::milliseconds(100)) != std::future_status::ready);
}
