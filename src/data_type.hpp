#pragma once
#include <algorithm>
#include <map>
#include <nlohmann/json.hpp>
#include <optional>
#include <string>
#include <vector>
#include <SFML/Graphics/Transform.hpp>

using json = nlohmann::json;

struct AnimationConfig {
    float node_size;
	int frame_rate;
	int frame_duration;
    float line_thickness;
    sf::Transform graphic_transform;
    sf::Transform physic_transform;
};

namespace nlohmann {
template<typename T> struct adl_serializer<std::optional<T>> {
    static void to_json(json &j, const std::optional<T> &opt) {
        if (opt.has_value()) {
            j = nullptr;
        } else {
            j = opt.value();
        }
    }

    static void from_json(const json &j, std::optional<T> &opt) {
        if (j.is_null()) {
            opt = std::nullopt;
        } else {
            opt = std::make_optional<T>(j.template get<T>()); // same as above, but with
                                                              // adl_serializer<T>::from_json
        }
    }
};
} // namespace nlohmann

struct TeamDataAnimation {
    int score;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(TeamDataAnimation, score)
};

struct TeamData {
    int id;
    std::string name;
    std::string color;
	uint32_t color_int;
    int score;
    std::optional<TeamDataAnimation> next;
    friend void to_json(nlohmann::json &nlohmann_json_j, const TeamData &nlohmann_json_t) {
        nlohmann_json_j["id"] = nlohmann_json_t.id;
        nlohmann_json_j["name"] = nlohmann_json_t.name;
        nlohmann_json_j["color"] = nlohmann_json_t.color;
        nlohmann_json_j["score"] = nlohmann_json_t.score;
        nlohmann_json_j["next"] = nlohmann_json_t.next;
    }
    friend void from_json(const nlohmann::json &nlohmann_json_j, TeamData &nlohmann_json_t) {
        nlohmann_json_j.at("id").get_to(nlohmann_json_t.id);
        nlohmann_json_j.at("name").get_to(nlohmann_json_t.name);
        nlohmann_json_j.at("color").get_to(nlohmann_json_t.color);
		nlohmann_json_t.color_int = std::stoul(nlohmann_json_t.color.substr(1, 6) + "ff", nullptr, 16);
        nlohmann_json_j.at("score").get_to(nlohmann_json_t.score);
        if (nlohmann_json_j.contains("next") && !nlohmann_json_j["next"].empty()) {
            nlohmann_json_j.at("next").get_to(nlohmann_json_t.next);
        } else {
            nlohmann_json_t.next = std::nullopt;
        }
    }
};

struct AntGroupData {
    int team;
    int qt;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(AntGroupData, team, qt)
};

struct RawNodeDataAnimation {
    std::optional<int> food;
    std::optional<std::vector<AntGroupData>> ants;
    friend void to_json(nlohmann::json &nlohmann_json_j,
                        const RawNodeDataAnimation &nlohmann_json_t) {
        nlohmann_json_j["food"] = nlohmann_json_t.food;
        nlohmann_json_j["ants"] = nlohmann_json_t.ants;
    }
    friend void from_json(const nlohmann::json &nlohmann_json_j,
                          RawNodeDataAnimation &nlohmann_json_t) {
        if (nlohmann_json_j.contains("food") && !nlohmann_json_j["food"].empty()) {
            nlohmann_json_j.at("food").get_to(nlohmann_json_t.food);
        } else {
            nlohmann_json_t.food = std::nullopt;
        }

        if (nlohmann_json_j.contains("ants") && !nlohmann_json_j["ants"].empty()) {
            nlohmann_json_j.at("ants").get_to(nlohmann_json_t.ants);
        } else {
            nlohmann_json_t.ants = std::nullopt;
        }
    }
};

struct RawNodeData {
    std::optional<std::string> type; // "VIDE", "NOURRITURE", "EAU", "REINE"
    std::optional<int> food;
    std::optional<int> team;
    int id;
    float x;
    float y;
    int pheromones;
    std::optional<std::vector<AntGroupData>> ants;
    std::optional<RawNodeDataAnimation> anim;
    friend void to_json(nlohmann::json &nlohmann_json_j, const RawNodeData &nlohmann_json_t) {
        nlohmann_json_j["type"] = nlohmann_json_t.type;
        nlohmann_json_j["food"] = nlohmann_json_t.food;
        nlohmann_json_j["team"] = nlohmann_json_t.team;
        nlohmann_json_j["id"] = nlohmann_json_t.id;
        nlohmann_json_j["x"] = nlohmann_json_t.x;
        nlohmann_json_j["y"] = nlohmann_json_t.y;
        nlohmann_json_j["pheromones"] = nlohmann_json_t.pheromones;
        nlohmann_json_j["ants"] = nlohmann_json_t.ants;
        nlohmann_json_j["anim"] = nlohmann_json_t.anim;
    }
    friend void from_json(const nlohmann::json &nlohmann_json_j, RawNodeData &nlohmann_json_t) {
        if (nlohmann_json_j.contains("type") && !nlohmann_json_j["type"].empty()) {
            nlohmann_json_j.at("type").get_to(nlohmann_json_t.type);
        } else {
            nlohmann_json_t.type = std::nullopt;
        }

        if (nlohmann_json_j.contains("food") && !nlohmann_json_j["food"].empty()) {
            nlohmann_json_j.at("food").get_to(nlohmann_json_t.food);
        } else {
            nlohmann_json_t.food = std::nullopt;
        }

        if (nlohmann_json_j.contains("team") && !nlohmann_json_j["team"].empty()) {
            nlohmann_json_j.at("team").get_to(nlohmann_json_t.team);
        } else {
            nlohmann_json_t.team = std::nullopt;
        }

        nlohmann_json_j.at("id").get_to(nlohmann_json_t.id);
        nlohmann_json_j.at("x").get_to(nlohmann_json_t.x);
        nlohmann_json_j.at("y").get_to(nlohmann_json_t.y);
        nlohmann_json_j.at("pheromones").get_to(nlohmann_json_t.pheromones);

        if (nlohmann_json_j.contains("ants") && !nlohmann_json_j["ants"].empty()) {
            nlohmann_json_j.at("ants").get_to(nlohmann_json_t.ants);
        } else {
            nlohmann_json_t.ants = std::nullopt;
        }

        if (nlohmann_json_j.contains("anim") && !nlohmann_json_j["anim"].empty()) {
            nlohmann_json_j.at("anim").get_to(nlohmann_json_t.anim);
        } else {
            nlohmann_json_t.anim = std::nullopt;
        }
    }
};

struct AntGroupEdgeAnimationData {
    double progress;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(AntGroupEdgeAnimationData, progress)
};

struct EdgeGroupData {
    AntGroupData group_data;
    double progress;
    std::optional<AntGroupEdgeAnimationData> anim_data;
    friend void to_json(nlohmann::json &nlohmann_json_j, const EdgeGroupData &nlohmann_json_t) {
        nlohmann_json_j["qt"] = nlohmann_json_t.group_data.qt;
		nlohmann_json_j["team"] = nlohmann_json_t.group_data.team;
        nlohmann_json_j["progress"] = nlohmann_json_t.progress;
        nlohmann_json_j["anim"] = nlohmann_json_t.anim_data;
    }
    friend void from_json(const nlohmann::json &nlohmann_json_j, EdgeGroupData &nlohmann_json_t) {
        nlohmann_json_j.at("qt").get_to(nlohmann_json_t.group_data.qt);
		nlohmann_json_j.at("team").get_to(nlohmann_json_t.group_data.team);
        nlohmann_json_j.at("progress").get_to(nlohmann_json_t.progress);
        if (nlohmann_json_j.contains("anim") && !nlohmann_json_j["anim"].empty()) {
            nlohmann_json_j.at("anim").get_to(nlohmann_json_t.anim_data);
        } else {
            nlohmann_json_t.anim_data = std::nullopt;
        }
    }
};

struct RawEdgeData {
    int id_1;
    int id_2;
    std::optional<double> life_ratio;
    std::optional<std::vector<EdgeGroupData>> groups;
    friend void to_json(nlohmann::json &nlohmann_json_j, const RawEdgeData &nlohmann_json_t) {
        nlohmann_json_j["id_1"] = nlohmann_json_t.id_1;
        nlohmann_json_j["id_2"] = nlohmann_json_t.id_2;
        nlohmann_json_j["life_ratio"] = nlohmann_json_t.life_ratio;
        nlohmann_json_j["groups"] = nlohmann_json_t.groups;
    }
    friend void from_json(const nlohmann::json &nlohmann_json_j, RawEdgeData &nlohmann_json_t) {
        nlohmann_json_j.at("id_1").get_to(nlohmann_json_t.id_1);
        nlohmann_json_j.at("id_2").get_to(nlohmann_json_t.id_2);
        if (nlohmann_json_j.contains("life_ratio") && !nlohmann_json_j["life_ratio"].empty()) {
            nlohmann_json_j.at("life_ratio").get_to(nlohmann_json_t.life_ratio);
        } else {
            nlohmann_json_t.life_ratio = std::nullopt;
        }
        if (nlohmann_json_j.contains("groups")  && !nlohmann_json_j["groups"].empty()) {
            nlohmann_json_j.at("groups").get_to(nlohmann_json_t.groups);
        } else {
            nlohmann_json_t.groups = std::nullopt;
        }
    }
};

struct NodeDataAnimation {
    std::optional<int> food;
    std::optional<std::map<int, AntGroupData>> ants;
    friend void to_json(nlohmann::json &nlohmann_json_j, const NodeDataAnimation &nlohmann_json_t) {
        nlohmann_json_j["food"] = nlohmann_json_t.food;
        nlohmann_json_j["ants"] = nlohmann_json_t.ants;
    }
    friend void from_json(const nlohmann::json &nlohmann_json_j,
                          NodeDataAnimation &nlohmann_json_t) {
        if (nlohmann_json_j.contains("food") && !nlohmann_json_j["food"].empty()) {
            nlohmann_json_j.at("food").get_to(nlohmann_json_t.food);
        } else {
            nlohmann_json_t.food = std::nullopt;
        }
    
        if (nlohmann_json_j.contains("ants") && !nlohmann_json_j["ants"].empty()) {
            nlohmann_json_j.at("ants").get_to(nlohmann_json_t.ants);
        } else {
            nlohmann_json_t.ants = std::nullopt;
        }
    
    }
};

class NodeData {
  public:
    int id;
    sf::Vector2f pos_;
    std::string type; // "VIDE", "EAU", "NOURRITURE", "REINE"
    int food;
    int team;
    std::map<int, AntGroupData> ants;
    std::optional<NodeDataAnimation> anim;
    int phero;

    explicit NodeData() : id(-1), pos_(sf::Vector2f(0, 0)), type("VIDE"), food(-1), team(-1), phero(-1) {}

    explicit NodeData(const RawNodeData &raw)
        : id(raw.id), pos_(sf::Vector2f(raw.x, raw.y)), type(raw.type.value_or("VIDE")),
          food((raw.type.value_or("VIDE") == "NOURRITURE") ? raw.food.value_or(0) : 0),
          team((raw.type.value_or("VIDE") == "REINE") ? raw.team.value_or(0) : 0),
          phero(raw.pheromones) {

        if (raw.ants.has_value()) {
            for (const auto &ant : raw.ants.value()) {
                ants[ant.team] = ant;
            }
        }

        if (raw.anim.has_value()) {

            std::map<int, AntGroupData> o;
            if (raw.anim->ants.has_value()) {
                for (const auto &ant : raw.anim->ants.value()) {
                    o[ant.team] = ant;
                }
            }

            anim = NodeDataAnimation{
                raw.anim->food,
                raw.anim->ants ? std::make_optional<std::map<int, AntGroupData>>(o) : std::nullopt};
        }
    }
};

class EdgeData {
  public:
    int id_1;
    int id_2;
    double life_ratio;
    std::vector<EdgeGroupData> groups;

    explicit EdgeData(const RawEdgeData &raw)
        : id_1(raw.id_1), id_2(raw.id_2), life_ratio(raw.life_ratio.value_or(0)),
          groups(raw.groups.value_or(std::vector<EdgeGroupData>())) {

        for (auto &group : groups) {
            if (group.anim_data.has_value()) {
                group.anim_data->progress = std::clamp(group.anim_data->progress, 0.0, 1.0);
            }
        }
    }
};

class WorldData {
  public:
    std::map<int, TeamData> teams;
    std::map<int, NodeData> nodes;
    int max_food;
    std::vector<EdgeData> edges;

    explicit WorldData(const json &raw) : max_food(raw["max_food"].get<int>()) {

        for (const auto &team : raw["teams"].get<std::vector<TeamData>>()) {
            teams[team.id] = team;
        }

        for (const auto &node : raw["nodes"].get<std::vector<RawNodeData>>()) {
            nodes[node.id] = NodeData(node);
        }

        for (const auto &edge : raw["edges"].get<std::vector<RawEdgeData>>()) {
            edges.emplace_back(edge);
        }
    }
};
