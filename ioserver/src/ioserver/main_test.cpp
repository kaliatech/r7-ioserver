#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include <iostream>
#include <memory>

#include "nlohmann-json/json.hpp"

using nlohmann::json;

// No problem with this, but unneeded copy
// of jsonObj for the return. Also, is the jsonStr
// copied in the jsonObj or is this going to have due
// to jsonStr going out of scope?
json getJsonNormal() {
    std::string jsonStr = "{\"id\":\"test\"}";
    return json::parse(jsonStr);
}

json getJsonNormalMem() {
    std::string* jsonStr = new std::string("{\"id\":\"test\"}");
    json jsonObj = json::parse(*jsonStr);
    delete jsonStr;
    return jsonObj;
}


json* getJsonPointer() {
    std::string* jsonStr = new std::string("{\"id\":\"test\"}");
    json jsonObj = json::parse(*jsonStr);
    json* jsonPtr = new json(jsonObj);
    delete jsonStr;
    return jsonPtr;
    //return new json("{\"id\":\"test\"}"_json);
}

std::unique_ptr<json> getJsonUPointer() {
     std::unique_ptr<json> jsonPtr = std::make_unique<json>(json("{\"id\":\"test\"}"_json));
     return jsonPtr;
    //return std::make_unique(new json("{\"id\":\"test\"}"_json));
}

int main_tests(int argc, char *argv[])
{
    nlohmann::json json;
    json = getJsonNormal();
    std::cout << json.dump(2) << std::endl;

    json = getJsonNormalMem();
    std::cout << json.dump(2) << std::endl;

    nlohmann::json* jsonPtr;
    jsonPtr = getJsonPointer();
    std::cout << jsonPtr->dump(2) << std::endl;

    auto jsonUPointer = getJsonUPointer();
    std::cout << jsonUPointer->dump(2) << std::endl;

    system("pause");

    return 0;
}
