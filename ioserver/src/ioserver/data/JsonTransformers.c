using nlohmann::json;

#include "../controllers/IoController.h"

namespace r7 {
    void to_json(json& j, const IoController& c);

    void from_json(const json& j, IoController& c);
}