#include "llm_processing.h"
#include <curl/curl.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

// Constructor
llm_processing::llm_processing() : api_key("") {}

// Function to set the API key
void llm_processing::set_api_key(const std::string& api_key) {
    this->api_key = api_key;
}

// Function to call the LLM model
std::string llm_processing::call(const std::string& user_input) {
    if (api_key.empty()) {
        return "API key is not set.";
    }

    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if (curl) {
        // Set up the request to OpenAI API
        curl_easy_setopt(curl, CURLOPT_URL, "https://api.openai.com/v1/chat/completions");
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, ("Authorization: Bearer " + api_key).c_str());
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // Custom prompt engineering for radio station-like acknowledgment
        std::string custom_prompt = "You are a military radio command operator. Acknowledge all incoming transmissions with precision and clarity. Use standard radio protocol and keep the communication concise. Respond to the user's commands or questions as if you are in an active command center." ;


        // Appending custom acknowledgment to the user input
        std::string prompt = custom_prompt + " The user just said: '" + user_input + "'. Please acknowledge their input.";



        // Create the JSON payload
        json payload = {
            {"model", "gpt-4"},  // Specify the model here
            {"messages", {
                {{"role", "system"}, {"content", "You are a helpful assistant."}},
                {{"role", "user"}, {"content", prompt}}
            }}
        };
        std::string payload_str = payload.dump();

        // Set the POST data
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload_str.c_str());

        // Write the response to a string
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        // Perform the request
        res = curl_easy_perform(curl);

        // Clean up
        curl_easy_cleanup(curl);

        if (res != CURLE_OK) {
            return "Request failed";
        }

        // Parse the JSON response
        json response = json::parse(readBuffer);
        return response["choices"][0]["message"]["content"];
    }

    return "Failed to initialize cURL";
}

// Callback function to handle cURL response data
size_t llm_processing::WriteCallback(void* contents, size_t size, size_t nmemb, std::string* s) {
    size_t totalSize = size * nmemb;
    s->append((char*)contents, totalSize);
    return totalSize;
}
