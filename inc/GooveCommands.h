#include "nlohmann/json.hpp"

static const nlohmann::json devStatus = R"({
    "msg": {
        "cmd": "devStatus",
        "data" : {
        }
    }
})"_json;


inline nlohmann::json color(uint8_t r, uint8_t g, uint8_t b)
{
    nlohmann::json req = {
        {"msg", {
            {"cmd", "colorwc"},
            {"data", {
                {"color", {
                    {"r", r},
                    {"g", g},
                    {"b", b}
                }},
                {"colorTemInKelvin", 0}
            }}
        }}
    };

    return req;
}
