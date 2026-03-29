#include "core/CharacterSerializer.h"
#include "core/SkillId.h"
#include <fstream>
#include <stdexcept>

namespace dod {

using json = nlohmann::json;

static json statsToJson(const Stats& s) {
    return {
        {"STY", s.STY}, {"FYS", s.FYS}, {"SMI", s.SMI},
        {"INT", s.INT}, {"PSY", s.PSY}, {"KAR", s.KAR}, {"STO", s.STO}
    };
}

static json bodyPartHpToJson(const BodyPartHP& hp) {
    return {
        {"chest",     hp.brostKorg},
        {"head",      hp.huvud},
        {"left_arm",  hp.vansterArm},
        {"right_arm", hp.hogerArm},
        {"left_leg",  hp.vansterBen},
        {"right_leg", hp.hogerBen}
    };
}

json CharacterSerializer::toJson(const Character& c) {
    // Skills array
    json skillsArr = json::array();
    for (const auto& s : c.skills) {
        skillsArr.push_back({
            {"name",              s.name},
            {"skill_id",          skillIdToKey(s.skillId)},
            {"base_stat",         s.baseStat},
            {"fv",                s.fv},
            {"fv_base",           s.fvBase},
            {"cl_note",           s.clNote},
            {"is_primary",        s.isPrimary},
            {"is_profession_skill", s.isProfessionSkill}
        });
    }

    // Roll history
    json rollHistArr = json::array();
    for (const auto& r : c.rollHistory) {
        json rollArr = json::array();
        for (int v : r.rolls) rollArr.push_back(v);
        rollHistArr.push_back({
            {"context",   r.context},
            {"dice_expr", r.diceExpr},
            {"rolls",     rollArr},
            {"modifier",  r.modifier},
            {"total",     r.total},
            {"outcome",   r.outcome}
        });
    }

    // Special ability grants
    json saGrantsArr = json::array();
    for (const auto& g : c.specialAbilityGrants) {
        saGrantsArr.push_back({
            {"type",         g.type == SpecialAbilityGrant::Type::FV ? "FV" : "CL"},
            {"skill_id",     skillIdToKey(g.skillId)},
            {"skill_name",   skillIdToString(g.skillId)},
            {"amount",       g.amount},
            {"player_choice",g.playerChoice}
        });
    }

    return {
        {"name",       c.name},
        {"gender",     c.gender},
        {"race",       c.race},
        {"profession", c.profession},
        {"age_category", c.ageCategory},

        {"stats", {
            {"purchased",  statsToJson(c.purchasedStats)},
            {"race_mods",  statsToJson(c.raceMods)},
            {"age_mods",   statsToJson(c.ageMods)},
            {"final",      statsToJson(c.finalStats)}
        }},

        {"derived", {
            {"hp_total",        c.derived.hpTotal},
            {"hp_per_body_part", bodyPartHpToJson(c.derived.hpParts)},
            {"damage_bonus",    c.derived.damageBonus},
            {"movement",        c.derived.movement}
        }},

        {"background_points", {
            {"total",    c.bpTotal},
            {"spent",    c.bpSpent},
            {"remaining", c.bpRemaining}
        }},

        {"special_ability", {
            {"has_ability",  c.hasSpecialAbility},
            {"bp_spent",     c.specialAbilityBPSpent},
            {"description",  c.specialAbilityDesc}
        }},

        {"social", {
            {"roll",        c.socialRoll},
            {"description", c.socialDescription},
            {"capital_base_sm",     c.capitalBase},
            {"starting_capital_sm", c.startingCapital}
        }},

        {"weapon_hand", {
            {"roll",                c.weaponHandRoll},
            {"hand",                c.weaponHand},
            {"from_special_ability", c.weaponHandFromSpecialAbility}
        }},

        {"experience", {
            {"ep_from_age", c.epFromAge},
            {"ep_from_bp",  c.epFromBP},
            {"ep_total",    c.epTotal},
            {"ep_spent",    c.epSpent}
        }},

        {"skills",                skillsArr},
        {"roll_history",          rollHistArr},
        {"special_ability_grants", saGrantsArr},

        {"appearance",   c.appearance},
        {"personality",  c.personality},
        {"background",   c.background}
    };
}

void CharacterSerializer::saveToFile(const Character& c, const std::string& filepath) {
    std::ofstream out(filepath);
    if (!out.is_open()) {
        throw std::runtime_error("Cannot open file for writing: " + filepath);
    }
    out << toJson(c).dump(4);
    if (!out) {
        throw std::runtime_error("Error writing to file: " + filepath);
    }
}

} // namespace dod
