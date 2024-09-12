
#ifndef LLM_PROCESSING_H
#define LLM_PROCESSING_H

#include <string>

class llm_processing {
public:
    // Constructor
    llm_processing();

    // Function to set the API key
    void set_api_key(const std::string& api_key);

    // Function to call the LLM model with a given prompt
    std::string call(const std::string& prompt);

private:
    // The API key to authenticate with OpenAI
    std::string api_key;

    // Callback function for cURL to write the response data
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* s);
};

#endif // LLM_PROCESSING_H
