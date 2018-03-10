#pragma once

#include <boost/assign/list_of.hpp>
#include <map>
#include <iostream>
#include <string>

namespace r7 {

enum class MimeType {
    PLAIN_TXT,
    APPLICATION_JSON,
};

class MimeTypeHelper
{
    public:
        MimeTypeHelper(MimeType type, const std::string& str);

    public:
        const MimeType type;
        const std::string str;
};

const std::map<MimeType, MimeTypeHelper> MimeTypes = boost::assign::map_list_of
   (MimeType::PLAIN_TXT, MimeTypeHelper(MimeType::PLAIN_TXT, "plain/txt"))
   (MimeType::APPLICATION_JSON, MimeTypeHelper(MimeType::APPLICATION_JSON, "application/json"));
}
