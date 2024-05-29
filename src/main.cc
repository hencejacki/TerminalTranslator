#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <unordered_set>
#include <unordered_map>
#include <cstring>
#include <json.hpp>

// endpoint for translate api
const char *endpoint;

// flag read from command line
int opt;

// options program supports
constexpr char options[] = "kf:q:s:t:h";

// support languages
std::unordered_set<const char *> support_langs = {
    "en",
    "zh"};

// support features and its mapping path
enum class Features
{
    TRANSLATE,
    TRANSLATE_FILE,
    DETECT,
    LANGUAGES
};

std::unordered_map<Features, const char *> support_features = {
    {Features::TRANSLATE, "/translate"},
    {Features::TRANSLATE_FILE, "/translate_file"},
    {Features::DETECT, "/detect"},
    {Features::LANGUAGES, "/languages"}};

// error handler function
inline void errIf(bool, const char *);

// usage for program
inline void usage();

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
#ifdef _DEBUG
        std::cout << "Returned response: "
                  << '\n'
                  << resp
                  << std::endl;
#endif
    }

private:
    std::string translated_text_;
    union DetectedLang
    {
        float confidence_;
        std::string lang_;
    } detected_lang_;
};

int main(int argc, char *const *argv)
{
    const char *api_key = getenv("TTL_API_KEY");

    char* source_lang = NULL;
    char* target_lang = NULL;
    char* format = NULL;
    char* query = NULL;
    bool key_use = false;

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

        // read config from environment
        endpoint = getenv("TTL_ENDPOINT");
        errIf(endpoint == NULL, "Not found environment variable: TTL_ENDPOINT");
        std::cout << "TTL_ENDPOINT: " << endpoint << std::endl;

        if (key_use) {
            errIf(api_key == NULL, "Api key use, but cannot find relevant environment variable.");
        }

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

        // TODO: send http request to api server


        // TODO: parse response
        

        // TODO: output to terminal

    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
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