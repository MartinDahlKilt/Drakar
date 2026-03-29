#include "core/GameData.h"
#include "core/SkillId.h"
#include <algorithm>

namespace dod {
namespace GameData {

// ---------------------------------------------------------------------------
// RACES
// ---------------------------------------------------------------------------
const std::vector<RaceDefinition>& getRaces() {
    static const std::vector<RaceDefinition> races = {
        // Alv  BP:25  STY-1, SMI+3, INT+3, KAR+2   STO 8-14(11)
        {
            "Alv", 25,
            /*statMods*/ {-1, 0, +3, +3, 0, +2, 0},
            /*stoRange*/ {8, 11, 14},
            /*bonuses*/  {
                {SkillId::UPPTACKA_FARA, 4, false},
                {SkillId::LYSSNA,        4, false}
            },
            "Elves resemble humans but have cat-like eyes and pointed ears. "
            "They are immortal, slim and dexterous with exceptional senses. "
            "Suitable for any profession, often Magiker or Utbygdsjägare."
        },
        // Anka  BP:0   SMI+2   STO 3-6(5)
        {
            "Anka", 0,
            {0, 0, +2, 0, 0, 0, 0},
            {3, 5, 6},
            {
                {SkillId::SIMMA, 20, true},   // FV set to 20
                {SkillId::SMYGA,  4, false}
            },
            "Picture a large duck with arms instead of wings. Small and lean "
            "but agile and resilient. Good as Sjöfarare, Tjuv, Jägare or Magiker."
        },
        // Dvärg  BP:25  STY+3, FYS+3   STO 4-9(7)
        {
            "Dvärg", 25,
            {+3, +3, 0, 0, 0, 0, 0},
            {4, 7, 9},
            {
                {SkillId::GEOLOGI, 5, true}   // FV set to 5
            },
            "Short and stocky with impressive beards. Expert craftsmen and miners "
            "with perfect darkvision. Strong and resilient — ideal Krigare. "
            "Never Magiker."
        },
        // Halvalv  BP:15  SMI+2   STO 7-16(13)
        {
            "Halvalv", 15,
            {0, 0, +2, 0, 0, 0, 0},
            {7, 13, 16},
            {
                {SkillId::UPPTACKA_FARA, 2, false},
                {SkillId::LYSSNA,        2, false}
            },
            "Half-elf, half-human — closer to humans in appearance. Slightly more "
            "agile with about twice the hearing and sight of a human. Suits all professions."
        },
        // Halvlängdsman  BP:15  STY-4, SMI+3   STO (small, around 4-8)
        {
            "Halvlängdsman", 15,
            {-4, 0, +3, 0, 0, 0, 0},
            {3, 6, 8},
            {
                {SkillId::GOMMA_SIG, 4, false}
            },
            "Halflings rarely exceed one metre. Peaceful by nature but "
            "surprisingly brave when trouble finds them. Their low Strength "
            "makes them poor Krigare, but superb Tjuv. Never Magiker."
        },
        // Halvorch  BP:10  STY+4, INT-2, KAR-4
        {
            "Halvorch", 10,
            {+4, 0, 0, -2, 0, -4, 0},
            {9, 13, 17},
            {
                {SkillId::SLAGSMAL, 4, false}
            },
            "Half-orc, half-human — stronger and tougher than humans, but "
            "with an unsettling yellowish-brown complexion and gnarled limbs. "
            "Suited to all professions."
        },
        // Människa  BP:10  no mods  STO 8-18(13)
        {
            "Människa", 10,
            {0, 0, 0, 0, 0, 0, 0},
            {8, 13, 18},
            {},  // no special bonuses
            "The most common folk. Humans suit every profession and have no "
            "particular advantages or disadvantages."
        },
    };
    return races;
}

// ---------------------------------------------------------------------------
// SKILLS
// ---------------------------------------------------------------------------
const std::vector<SkillDefinition>& getAllSkills() {
    static const std::vector<SkillDefinition> skills = {
        // ---- Primary skills ----
        {SkillId::BLUFFA,                       SkillType::PRIMARY,   "KAR"},
        {SkillId::FINNA_DOLDA_TING,             SkillType::PRIMARY,   "INT"},
        {SkillId::FORSTA_HJALPEN,               SkillType::PRIMARY,   "INT"},
        {SkillId::GOMMA_SIG,                    SkillType::PRIMARY,   "INT"},
        {SkillId::HOPPA,                        SkillType::PRIMARY,   "SMI"},
        {SkillId::KLATRA,                       SkillType::PRIMARY,   "SMI"},
        {SkillId::KOPSLA,                       SkillType::PRIMARY,   "KAR"},
        {SkillId::LYSSNA,                       SkillType::PRIMARY,   "INT"},
        {SkillId::LASA_SKRIVA_MODERSMAL,        SkillType::PRIMARY,   "INT", false, false, true},
        {SkillId::RIDA,                         SkillType::PRIMARY,   "SMI"},
        {SkillId::SJUNGA,                       SkillType::PRIMARY,   "KAR"},
        {SkillId::SLAGSMAL,                     SkillType::PRIMARY,   "STY"},
        {SkillId::SMYGA,                        SkillType::PRIMARY,   "SMI"},
        {SkillId::SPARA,                        SkillType::PRIMARY,   "INT"},
        {SkillId::STJALA_FOREMAL,               SkillType::PRIMARY,   "SMI"},
        {SkillId::TALA_MODERSMAL,               SkillType::PRIMARY,   "INT"},
        {SkillId::UPPTACKA_FARA,                SkillType::PRIMARY,   "PSY"},
        {SkillId::VARDERA,                      SkillType::PRIMARY,   "INT"},
        {SkillId::OVERTALA,                     SkillType::PRIMARY,   "KAR"},

        // ---- Secondary skills (non-weapon) ----
        {SkillId::ADMINISTRATION,               SkillType::SECONDARY, "INT"},
        {SkillId::AKROBATIK,                    SkillType::SECONDARY, "SMI"},
        {SkillId::ALKEMI,                       SkillType::SECONDARY, "INT"},
        {SkillId::ANIMISM,                      SkillType::SECONDARY, "PSY", false, false, false, false, false, true},
        {SkillId::ASTROLOGI,                    SkillType::SECONDARY, "INT"},
        {SkillId::AVVAPNA,                      SkillType::SECONDARY, "SMI"},
        {SkillId::BUKTALA,                      SkillType::SECONDARY, "PSY"},
        {SkillId::BARSARKAGANGA,                SkillType::SECONDARY, "PSY"},
        {SkillId::DANS,                         SkillType::SECONDARY, "SMI"},
        {SkillId::DJURHELNING,                  SkillType::SECONDARY, "INT"},
        {SkillId::DJURTRANING,                  SkillType::SECONDARY, "PSY"},
        {SkillId::DOLK,                         SkillType::SECONDARY, "SMI", true},
        {SkillId::DRA_VAPEN,                    SkillType::SECONDARY, "SMI"},
        {SkillId::DROGKUNSKAP,                  SkillType::SECONDARY, "INT"},
        {SkillId::ELEMENTARMAGI,                SkillType::SECONDARY, "INT", false, false, false, false, false, true},
        {SkillId::FORFALSKNING,                 SkillType::SECONDARY, "INT"},
        {SkillId::GEOGRAFI,                     SkillType::SECONDARY, "INT"},
        {SkillId::GEOLOGI,                      SkillType::SECONDARY, "INT"},
        {SkillId::GIFTKUNSKAP,                  SkillType::SECONDARY, "INT"},
        {SkillId::GYCKELKONSTER,                SkillType::SECONDARY, "SMI"},
        {SkillId::HANTERA_FALLOR,               SkillType::SECONDARY, "SMI"},
        // Hantverk is parameterised — a single entry; players choose specific craft
        {SkillId::HANTVERK,                     SkillType::SECONDARY, "SMI", false, false, false, false, true},
        {SkillId::HASARDSPEL,                   SkillType::SECONDARY, "INT"},
        {SkillId::HERALDIK,                     SkillType::SECONDARY, "INT"},
        {SkillId::HISTORIA,                     SkillType::SECONDARY, "INT"},
        {SkillId::HYPNOTISERA,                  SkillType::SECONDARY, "PSY"},
        {SkillId::KNOPAR,                       SkillType::SECONDARY, "SMI"},
        {SkillId::KULTURKANNEDOM,               SkillType::SECONDARY, "INT"},
        {SkillId::KUNSKAP_OM_DEMONER,           SkillType::SECONDARY, "INT"},
        {SkillId::KUNSKAP_OM_MAGI,              SkillType::SECONDARY, "INT"},
        {SkillId::KUNSKAP_OM_ODODA,             SkillType::SECONDARY, "INT"},
        {SkillId::KANNA_MAGI,                   SkillType::SECONDARY, "PSY"},
        {SkillId::LASDYRKNING,                  SkillType::SECONDARY, "SMI"},
        {SkillId::LAKEKONST,                    SkillType::SECONDARY, "INT"},
        {SkillId::LAPPLASNING,                  SkillType::SECONDARY, "INT"},
        {SkillId::LASA_SKRIVA_FRAMMANDE_SPRAK,  SkillType::SECONDARY, "INT", false, false, true},
        {SkillId::MAGISK_KANALISERING,          SkillType::SECONDARY, "INT"},
        {SkillId::MASSAGE,                      SkillType::SECONDARY, "SMI"},
        {SkillId::MENTALISM,                    SkillType::SECONDARY, "PSY", false, false, false, false, false, true},
        {SkillId::MUTA,                         SkillType::SECONDARY, "KAR"},
        {SkillId::MALNING,                      SkillType::SECONDARY, "SMI"},
        {SkillId::NAVIGERA,                     SkillType::SECONDARY, "INT"},
        {SkillId::ORIENTERING,                  SkillType::SECONDARY, "INT"},
        {SkillId::RAKNING,                      SkillType::SECONDARY, "INT"},
        {SkillId::SCHACK_BRADSPEL,              SkillType::SECONDARY, "INT"},
        {SkillId::SIMMA,                        SkillType::SECONDARY, "FYS"},
        {SkillId::SJOKUNNIGHET,                 SkillType::SECONDARY, "INT"},
        {SkillId::SKADESPELERI,                 SkillType::SECONDARY, "KAR"},
        {SkillId::SPELA_INSTRUMENT,             SkillType::SECONDARY, "KAR", false, false, false, true},
        {SkillId::SPRAKKUNSKAP,                 SkillType::SECONDARY, "INT"},
        {SkillId::SPA_VADER,                    SkillType::SECONDARY, "INT"},
        {SkillId::STAVHOPP,                     SkillType::SECONDARY, "SMI"},
        {SkillId::STRIDSKONSTER,                SkillType::SECONDARY, "SMI"},
        {SkillId::TALA_FRAMMANDE_SPRAK,         SkillType::SECONDARY, "INT", false, true},
        {SkillId::TECKENSPRAK,                  SkillType::SECONDARY, "INT"},
        {SkillId::TRASTAV,                      SkillType::SECONDARY, "SMI", true},
        {SkillId::TVA_VAPEN,                    SkillType::SECONDARY, "SMI"},
        {SkillId::UNDRE_VARLDEN,                SkillType::SECONDARY, "INT"},
        {SkillId::VAPENFARDIGHET,               SkillType::SECONDARY, "var.", true},
        {SkillId::ZOOLOGI,                      SkillType::SECONDARY, "INT"},
        {SkillId::ANTERHAKE,                    SkillType::SECONDARY, "SMI"},
        {SkillId::ORTKUNSKAP,                   SkillType::SECONDARY, "INT"},
        {SkillId::OVERLEVNAD,                   SkillType::SECONDARY, "INT"},
        // ---- Warrior Expansion skills ----
        {SkillId::HARSKRI,                      SkillType::SECONDARY, "STY"},
        {SkillId::KAMOUFLAGE,                   SkillType::SECONDARY, "SMI"},
        {SkillId::MARSCHERA,                    SkillType::SECONDARY, "FYS"},
        {SkillId::BRYTA_GREPP,                  SkillType::SECONDARY, "STY"},
        {SkillId::SLA_MEDVETSLOST,              SkillType::SECONDARY, "STY"},
        {SkillId::SLOSS_TILL_HAST,              SkillType::SECONDARY, "SMI"},
        {SkillId::STRID_I_MORKER,               SkillType::SECONDARY, "PSY"},
        {SkillId::STRID_I_LUFTEN,               SkillType::SECONDARY, "SMI"},
        {SkillId::DUELLERA,                     SkillType::SECONDARY, "SMI"},
        {SkillId::FALLTEKNIK,                   SkillType::SECONDARY, "SMI"},
        {SkillId::BROTTAS_MED_DJUR,             SkillType::SECONDARY, "STY"},
        {SkillId::VAGNSFORARE,                  SkillType::SECONDARY, "SMI"},
        {SkillId::STRATEGI,                     SkillType::SECONDARY, "INT"},
        {SkillId::TAKTIK,                       SkillType::SECONDARY, "INT"},
        {SkillId::VAPENTEKNIK,                  SkillType::SECONDARY, "SMI"},
        {SkillId::ANFALL_BAKIFRAN,              SkillType::SECONDARY, "SMI"},
    };
    return skills;
}

const std::vector<SkillDefinition>& getPrimarySkills() {
    static std::vector<SkillDefinition> primary = []{
        std::vector<SkillDefinition> v;
        for (const auto& s : getAllSkills())
            if (s.type == SkillType::PRIMARY) v.push_back(s);
        return v;
    }();
    return primary;
}

const std::vector<SkillDefinition>& getSecondarySkills() {
    static std::vector<SkillDefinition> secondary = []{
        std::vector<SkillDefinition> v;
        for (const auto& s : getAllSkills())
            if (s.type == SkillType::SECONDARY) v.push_back(s);
        return v;
    }();
    return secondary;
}

// ---------------------------------------------------------------------------
// PROFESSIONS
// ---------------------------------------------------------------------------
const std::vector<ProfessionDefinition>& getProfessions() {
    static const std::vector<ProfessionDefinition> profs = {
        // ---- Bard ----
        {
            "Bard",
            "A versatile entertainer, troubadour, street musician or jester.",
            "By playing or singing for at least one minute (and succeeding at "
            "the skill check) the Bard temporarily raises their KAR by 5 for "
            "one hour, affecting all KAR-based skill chances.",
            {{"SMI", 12}, {"KAR", 14}},
            12, 1, 2, 1, -1,  // maxSkills, maxWeapon, maxLang, maxLiteracy, maxInstrument (-1=unlimited)
            {
                SkillId::VAPENFARDIGHET,          // 1 any
                SkillId::TALA_FRAMMANDE_SPRAK,    // max 2
                SkillId::LASA_SKRIVA_FRAMMANDE_SPRAK, // max 1
                SkillId::ADMINISTRATION, SkillId::AKROBATIK, SkillId::BUKTALA, SkillId::DANS, SkillId::DJURTRANING,
                SkillId::DOLK, SkillId::FORFALSKNING, SkillId::GEOGRAFI, SkillId::GYCKELKONSTER, SkillId::HANTVERK,
                SkillId::HASARDSPEL, SkillId::HERALDIK, SkillId::HISTORIA, SkillId::HYPNOTISERA, SkillId::KNOPAR,
                SkillId::KULTURKANNEDOM, SkillId::LASDYRKNING, SkillId::LAPPLASNING, SkillId::MUTA,
                SkillId::MALNING, SkillId::SCHACK_BRADSPEL, SkillId::SIMMA, SkillId::SKADESPELERI,
                SkillId::SPELA_INSTRUMENT, SkillId::SPRAKKUNSKAP, SkillId::TRASTAV
            }
        },
        // ---- Helare ----
        {
            "Helare",
            "A healer who can mend wounds, cure disease and neutralise poisons "
            "through the laying on of hands.",
            "Via touch the Healer can heal 1 HP per combat round, costing 1 PSY "
            "per HP healed. Can also cure disease (PSY/2 vs disease strength) and "
            "neutralise poison (PSY/2 vs poison STY). PSY cannot drop below 1.",
            {{"INT", 12}, {"PSY", 12}},
            12, 0, 2, 1, 0,
            {
                SkillId::TALA_FRAMMANDE_SPRAK, SkillId::LASA_SKRIVA_FRAMMANDE_SPRAK,
                SkillId::ALKEMI, SkillId::DJURHELNING, SkillId::DOLK, SkillId::DROGKUNSKAP, SkillId::GEOGRAFI,
                SkillId::GIFTKUNSKAP, SkillId::HANTVERK, SkillId::HYPNOTISERA, SkillId::KULTURKANNEDOM,
                SkillId::KUNSKAP_OM_DEMONER, SkillId::KUNSKAP_OM_MAGI, SkillId::KUNSKAP_OM_ODODA,
                SkillId::LAKEKONST, SkillId::MASSAGE, SkillId::ORIENTERING, SkillId::SIMMA, SkillId::TRASTAV,
                SkillId::ZOOLOGI, SkillId::ORTKUNSKAP, SkillId::OVERLEVNAD
            }
        },
        // ---- Krigare ----
        {
            "Krigare",
            "A fighter who has dedicated their life to combat — mercenary, "
            "bodyguard, city guard, or bandit.",
            "The Warrior always receives +5 on all initiative rolls.",
            {{"STY", 14}, {"FYS", 12}},
            12, -1, 1, 0, 0,
            {
                SkillId::VAPENFARDIGHET,            // unlimited weapon skills
                SkillId::TALA_FRAMMANDE_SPRAK,
                SkillId::AVVAPNA, SkillId::BARSARKAGANGA, SkillId::DRA_VAPEN, SkillId::DOLK, SkillId::GEOGRAFI,
                SkillId::HANTVERK, SkillId::HASARDSPEL, SkillId::KULTURKANNEDOM, SkillId::SIMMA,
                SkillId::STRIDSKONSTER, SkillId::TRASTAV, SkillId::TVA_VAPEN
            }
        },
        // ---- Lärd man ----
        {
            "Lärd man",
            "A scholar who has devoted their life to studying a particular "
            "subject — alchemy, languages, geography, history, etc.",
            "The Scholar always applies -5 to all Fear Table rolls thanks to "
            "their rational outlook.",
            {{"INT", 16}},
            12, 0, 4, 4, 0,
            {
                SkillId::TALA_FRAMMANDE_SPRAK,
                SkillId::LASA_SKRIVA_FRAMMANDE_SPRAK,
                SkillId::ADMINISTRATION, SkillId::ALKEMI, SkillId::ASTROLOGI, SkillId::DOLK, SkillId::DROGKUNSKAP,
                SkillId::FORFALSKNING, SkillId::GEOGRAFI, SkillId::GEOLOGI, SkillId::GIFTKUNSKAP,
                SkillId::HASARDSPEL, SkillId::HERALDIK, SkillId::HISTORIA, SkillId::KULTURKANNEDOM,
                SkillId::KUNSKAP_OM_DEMONER, SkillId::KUNSKAP_OM_MAGI, SkillId::KUNSKAP_OM_ODODA,
                SkillId::RAKNING, SkillId::SCHACK_BRADSPEL, SkillId::SIMMA, SkillId::SPRAKKUNSKAP,
                SkillId::TRASTAV, SkillId::ZOOLOGI, SkillId::ORTKUNSKAP
            }
        },
        // ---- Lönnmördare ----
        {
            "Lönnmördare",
            "An assassin specialising in stealth, lock-picking, climbing "
            "and silent kills.",
            "By attacking from behind (requires successful Smyga; target fails "
            "Upptäcka fara) the Assassin deals ×2 to ×4 damage: a fumble = miss; "
            "miss result = normal damage; success = ×2; perfect = ×4. "
            "Only usable against humanoids no more than 2 m taller.",
            {{"SMI", 14}, {"PSY", 12}},
            12, 1, 1, 0, 0,
            {
                SkillId::VAPENFARDIGHET,
                SkillId::TALA_FRAMMANDE_SPRAK,
                SkillId::ADMINISTRATION, SkillId::AKROBATIK, SkillId::DRA_VAPEN, SkillId::DOLK, SkillId::FORFALSKNING,
                SkillId::GEOGRAFI, SkillId::GIFTKUNSKAP, SkillId::HANTERA_FALLOR, SkillId::HASARDSPEL,
                SkillId::HYPNOTISERA, SkillId::KNOPAR, SkillId::KULTURKANNEDOM, SkillId::LASDYRKNING,
                SkillId::MUTA, SkillId::SIMMA, SkillId::SKADESPELERI, SkillId::STAVHOPP, SkillId::STRIDSKONSTER,
                SkillId::TECKENSPRAK, SkillId::TRASTAV, SkillId::UNDRE_VARLDEN, SkillId::ANTERHAKE
            }
        },
        // ---- Magiker ----
        {
            "Magiker",
            "A spellcaster who has devoted childhood and youth to learning "
            "the magical arts. May learn spells from the start.",
            "Mages have no special profession ability, but can learn spells "
            "at character creation (other professions cannot).",
            {{"INT", 12}, {"PSY", 14}},
            9,  // only 9 skill choices!
            0, 3, 3, 0,
            {
                SkillId::TALA_FRAMMANDE_SPRAK,
                SkillId::LASA_SKRIVA_FRAMMANDE_SPRAK,
                SkillId::ALKEMI, SkillId::ASTROLOGI, SkillId::DJURHELNING, SkillId::DJURTRANING,
                SkillId::DROGKUNSKAP, SkillId::GEOGRAFI, SkillId::GIFTKUNSKAP, SkillId::KULTURKANNEDOM,
                SkillId::KUNSKAP_OM_DEMONER, SkillId::KUNSKAP_OM_MAGI, SkillId::KUNSKAP_OM_ODODA,
                SkillId::MAGISK_KANALISERING,
                SkillId::ANIMISM, SkillId::ELEMENTARMAGI, SkillId::MENTALISM,  // choose one magic school
                SkillId::RAKNING, SkillId::SIMMA, SkillId::SPRAKKUNSKAP, SkillId::TRASTAV,
                SkillId::ZOOLOGI, SkillId::ORTKUNSKAP
            }
        },
        // ---- Munk ----
        {
            "Munk",
            "A person who dedicates their life to religious study and meditation.",
            "By meditating for a full combat round the Monk raises their FV in "
            "any chosen skill by 1 (cumulative, up to double the original FV). "
            "The boost applies to a single roll made within one minute.",
            {{"INT", 12}, {"PSY", 12}},
            12, 0, 3, 3, 2,
            {
                SkillId::TALA_FRAMMANDE_SPRAK,
                SkillId::LASA_SKRIVA_FRAMMANDE_SPRAK,
                SkillId::AVVAPNA, SkillId::DJURHELNING, SkillId::DOLK, SkillId::DROGKUNSKAP, SkillId::FORFALSKNING,
                SkillId::GEOGRAFI, SkillId::GIFTKUNSKAP, SkillId::HANTVERK, SkillId::HERALDIK, SkillId::HISTORIA,
                SkillId::KNOPAR, SkillId::KULTURKANNEDOM, SkillId::KUNSKAP_OM_DEMONER,
                SkillId::KUNSKAP_OM_MAGI, SkillId::KUNSKAP_OM_ODODA, SkillId::LAKEKONST, SkillId::MASSAGE,
                SkillId::MALNING, SkillId::RAKNING, SkillId::SIMMA, SkillId::SPELA_INSTRUMENT,
                SkillId::SPRAKKUNSKAP, SkillId::STRIDSKONSTER, SkillId::TRASTAV, SkillId::ZOOLOGI, SkillId::ORTKUNSKAP
            }
        },
        // ---- Sjöfarare ----
        {
            "Sjöfarare",
            "A seafarer who has spent most of their life at sea — sailor, "
            "captain, pirate, explorer or fisherman.",
            "Due to a hard life at sea the Sailor gains +5 on FYS, STO or STY "
            "on all resistance rolls against elemental spells or natural elements "
            "(fire, cold, etc.).",
            {{"FYS", 12}, {"SMI", 12}},
            12, 3, 3, 1, 2,
            {
                SkillId::VAPENFARDIGHET,
                SkillId::TALA_FRAMMANDE_SPRAK,
                SkillId::LASA_SKRIVA_FRAMMANDE_SPRAK,
                SkillId::AKROBATIK, SkillId::DANS, SkillId::DOLK, SkillId::GEOGRAFI, SkillId::HANTVERK,
                SkillId::HASARDSPEL, SkillId::GYCKELKONSTER, SkillId::KNOPAR, SkillId::KULTURKANNEDOM,
                SkillId::MUTA, SkillId::NAVIGERA, SkillId::ORIENTERING, SkillId::SCHACK_BRADSPEL,
                SkillId::SIMMA, SkillId::SJOKUNNIGHET, SkillId::SPELA_INSTRUMENT, SkillId::SPA_VADER,
                SkillId::STAVHOPP, SkillId::TRASTAV, SkillId::UNDRE_VARLDEN, SkillId::ANTERHAKE
            }
        },
        // ---- Riddare ----
        {
            "Riddare",
            "A warrior of noble birth sworn to protect the weak and uphold "
            "honour. May be a temple knight or vassal of a lord.",
            "By spending 5 PSY points the Knight may either: (1) target any "
            "body part without penalty (detailed combat), or (2) deal maximum "
            "weapon damage including maximum damage bonus. Armour absorption "
            "still applies. PSY recovers normally.",
            {{"STY", 14}, {"FYS", 12}, {"PSY", 12}},
            12, 5, 1, 1, 2,
            {
                SkillId::VAPENFARDIGHET,
                SkillId::TALA_FRAMMANDE_SPRAK,
                SkillId::LASA_SKRIVA_FRAMMANDE_SPRAK,
                SkillId::ADMINISTRATION, SkillId::AVVAPNA, SkillId::DANS, SkillId::DJURTRANING,
                SkillId::DRA_VAPEN, SkillId::DOLK, SkillId::GEOGRAFI, SkillId::HERALDIK, SkillId::HISTORIA,
                SkillId::KULTURKANNEDOM, SkillId::KUNSKAP_OM_MAGI, SkillId::KUNSKAP_OM_ODODA,
                SkillId::MALNING, SkillId::RAKNING, SkillId::SCHACK_BRADSPEL, SkillId::SIMMA,
                SkillId::SPELA_INSTRUMENT, SkillId::SPRAKKUNSKAP, SkillId::TRASTAV, SkillId::TVA_VAPEN
            }
        },
        // ---- Tjuv ----
        {
            "Tjuv",
            "A rogue with extraordinary luck in tight situations, well "
            "connected to the criminal underworld.",
            "By spending PSY points the Thief modifies any skill's chance of "
            "success by +1 per PSY point spent (max +3, max twice per 8-hour rest). "
            "PSY recovers normally.",
            {{"SMI", 16}},
            12, 2, 1, 0, 2,
            {
                SkillId::VAPENFARDIGHET,
                SkillId::TALA_FRAMMANDE_SPRAK,
                SkillId::ADMINISTRATION, SkillId::AKROBATIK, SkillId::BUKTALA, SkillId::DRA_VAPEN, SkillId::DOLK,
                SkillId::DROGKUNSKAP, SkillId::FORFALSKNING, SkillId::GEOGRAFI, SkillId::GIFTKUNSKAP,
                SkillId::HYPNOTISERA, SkillId::GYCKELKONSTER, SkillId::KNOPAR, SkillId::KULTURKANNEDOM,
                SkillId::LASDYRKNING, SkillId::LAPPLASNING, SkillId::MUTA, SkillId::RAKNING, SkillId::SIMMA,
                SkillId::SKADESPELERI, SkillId::SPELA_INSTRUMENT, SkillId::STAVHOPP,
                SkillId::TECKENSPRAK, SkillId::TRASTAV, SkillId::UNDRE_VARLDEN, SkillId::ANTERHAKE
            }
        },
        // ---- Utbygdsjägare ----
        {
            "Utbygdsjägare",
            "A wilderness survivor — scout, forester, woodcutter, guide, "
            "shepherd or explorer.",
            "The Wilderness Hunter has Animism as their profession ability. "
            "They may learn Animism as a profession skill and all general spells "
            "and Animism spells with school value 12 or lower. Cannot learn "
            "spells at character creation.",
            {{"FYS", 12}, {"SMI", 12}, {"PSY", 12}},
            12, 3, 1, 0, 0,
            {
                SkillId::VAPENFARDIGHET,
                SkillId::TALA_FRAMMANDE_SPRAK,
                SkillId::ANIMISM, SkillId::DJURHELNING, SkillId::DJURTRANING, SkillId::DOLK, SkillId::DROGKUNSKAP,
                SkillId::GEOGRAFI, SkillId::GEOLOGI, SkillId::GIFTKUNSKAP, SkillId::HANTVERK,
                SkillId::HANTERA_FALLOR, SkillId::KNOPAR, SkillId::KULTURKANNEDOM, SkillId::ORIENTERING,
                SkillId::SIMMA, SkillId::SPA_VADER, SkillId::ZOOLOGI, SkillId::ORTKUNSKAP, SkillId::OVERLEVNAD
            }
        },
        // ====================================================================
        // Warrior Expansion professions
        // ====================================================================
        // ---- Barbar ----
        {
            "Barbar",
            "The barbarian lives outside civilisation, hardened by life in the "
            "wilderness, in constant struggle with wild animals and hostile tribes.",
            "The Barbarian tolerates more punishment than ordinary people. "
            "+3 to STO and FYS when calculating total HP (affects body-part HP too). "
            "+7 to FYS when calculating SP if using detailed combat rules.",
            {{"STY", 14}, {"FYS", 15}},
            12, 5, 1, 0, 0,
            {
                SkillId::VAPENFARDIGHET,
                SkillId::TALA_FRAMMANDE_SPRAK,
                SkillId::ANFALL_BAKIFRAN, SkillId::BROTTAS_MED_DJUR, SkillId::BRYTA_GREPP,
                SkillId::BARSARKAGANGA, SkillId::DOLK, SkillId::DRA_VAPEN, SkillId::FALLTEKNIK,
                SkillId::GEOGRAFI, SkillId::HANTERA_FALLOR, SkillId::HARSKRI, SkillId::KAMOUFLAGE,
                SkillId::MARSCHERA, SkillId::ORIENTERING, SkillId::SIMMA, SkillId::SLA_MEDVETSLOST,
                SkillId::SLOSS_TILL_HAST, SkillId::SPA_VADER, SkillId::STRID_I_LUFTEN, SkillId::STRID_I_MORKER,
                SkillId::TRASTAV, SkillId::VAGNSFORARE, SkillId::ZOOLOGI, SkillId::ORTKUNSKAP, SkillId::OVERLEVNAD
            },
            true
        },
        // ---- Gladiator ----
        {
            "Gladiator",
            "The gladiator is a professional warrior who fights for money or "
            "their life several times a week before an audience.",
            "Gladiators gain +3 to BC on all weapon skills that are profession skills. "
            "The bonus is added to the BC derived from the relevant stat.",
            {{"STY", 14}, {"FYS", 12}, {"SMI", 12}},
            12, 6, 2, 0, 0,
            {
                SkillId::VAPENFARDIGHET,
                SkillId::TALA_FRAMMANDE_SPRAK,
                SkillId::ANFALL_BAKIFRAN, SkillId::AVVAPNA, SkillId::BROTTAS_MED_DJUR,
                SkillId::BRYTA_GREPP, SkillId::BARSARKAGANGA, SkillId::DJURTRANING,
                SkillId::DOLK, SkillId::DRA_VAPEN, SkillId::DUELLERA, SkillId::FALLTEKNIK,
                SkillId::GEOGRAFI, SkillId::HASARDSPEL, SkillId::HARSKRI, SkillId::KULTURKANNEDOM,
                SkillId::LAKEKONST, SkillId::MUTA, SkillId::SIMMA, SkillId::SKADESPELERI,
                SkillId::SLA_MEDVETSLOST, SkillId::STRID_I_MORKER, SkillId::STRIDSKONSTER,
                SkillId::TRASTAV, SkillId::TVA_VAPEN, SkillId::VAGNSFORARE
            },
            true
        },
        // ---- Krigarmunk ----
        {
            "Krigarmunk",
            "Warrior monks are the martial arm of a monastic order — trained in "
            "a combat art from childhood and highly skilled.",
            "When the Warrior Monk buys techniques for their combat art, they only "
            "pay half the base cost (total rounded up).",
            {{"STY", 14}, {"FYS", 12}, {"SMI", 16}, {"PSY", 15}, {"INT", 11}},
            12, 1, 2, 1, 0,
            {
                SkillId::VAPENFARDIGHET,
                SkillId::TALA_FRAMMANDE_SPRAK,
                SkillId::LASA_SKRIVA_FRAMMANDE_SPRAK,
                SkillId::AKROBATIK, SkillId::ANFALL_BAKIFRAN, SkillId::AVVAPNA,
                SkillId::BRYTA_GREPP, SkillId::DROGKUNSKAP, SkillId::DUELLERA,
                SkillId::FALLTEKNIK, SkillId::GEOGRAFI, SkillId::HANTVERK, SkillId::HISTORIA,
                SkillId::HARSKRI, SkillId::KAMOUFLAGE, SkillId::KULTURKANNEDOM, SkillId::LAKEKONST,
                SkillId::MASSAGE, SkillId::SIMMA, SkillId::SLA_MEDVETSLOST, SkillId::STRID_I_MORKER,
                SkillId::STRIDSKONSTER, SkillId::TRASTAV, SkillId::ORTKUNSKAP, SkillId::OVERLEVNAD
            },
            true
        },
        // ---- Paladin ----
        {
            "Paladin",
            "The paladin is a knight who has sworn themselves to a god. "
            "In word and deed the paladin serves their deity.",
            "The Paladin may learn Mentalism as a profession skill — all Mentalism "
            "spells with school value 12 or lower. They may spend at most 1/3 of "
            "their EP on spells at character creation.",
            {{"STY", 15}, {"FYS", 12}, {"PSY", 14}, {"INT", 12}},
            12, 4, 2, 2, 0,
            {
                SkillId::VAPENFARDIGHET,
                SkillId::TALA_FRAMMANDE_SPRAK,
                SkillId::LASA_SKRIVA_FRAMMANDE_SPRAK,
                SkillId::AVVAPNA, SkillId::BRYTA_GREPP, SkillId::DOLK, SkillId::DJURTRANING,
                SkillId::DRA_VAPEN, SkillId::DUELLERA, SkillId::FALLTEKNIK, SkillId::GEOGRAFI,
                SkillId::HERALDIK, SkillId::HISTORIA, SkillId::HARSKRI, SkillId::KULTURKANNEDOM,
                SkillId::KUNSKAP_OM_DEMONER, SkillId::KUNSKAP_OM_MAGI, SkillId::KUNSKAP_OM_ODODA,
                SkillId::MENTALISM, SkillId::SCHACK_BRADSPEL, SkillId::SIMMA, SkillId::SLA_MEDVETSLOST,
                SkillId::SLOSS_TILL_HAST, SkillId::STRID_I_LUFTEN, SkillId::STRID_I_MORKER,
                SkillId::STRATEGI, SkillId::TAKTIK, SkillId::TRASTAV, SkillId::TVA_VAPEN, SkillId::VAGNSFORARE
            },
            true
        },
        // ---- Prisjägare ----
        {
            "Prisjägare",
            "Bounty hunters track down wanted individuals for a reward. "
            "Their targets may be criminals or freedom fighters.",
            "The Bounty Hunter is always on guard and has developed a sixth "
            "sense for danger. This gives a minimum CL of 17 in Upptäcka fara "
            "at all times, against all forms of physical threat.",
            {{"STY", 14}, {"FYS", 12}, {"SMI", 14}},
            12, 4, 2, 1, 0,
            {
                SkillId::VAPENFARDIGHET,
                SkillId::TALA_FRAMMANDE_SPRAK,
                SkillId::LASA_SKRIVA_FRAMMANDE_SPRAK,
                SkillId::ANFALL_BAKIFRAN, SkillId::AVVAPNA, SkillId::BRYTA_GREPP,
                SkillId::DOLK, SkillId::DRA_VAPEN, SkillId::DROGKUNSKAP,
                SkillId::FALLTEKNIK, SkillId::FORFALSKNING, SkillId::GEOGRAFI,
                SkillId::HASARDSPEL, SkillId::HISTORIA, SkillId::HYPNOTISERA, SkillId::HARSKRI,
                SkillId::KAMOUFLAGE, SkillId::KNOPAR, SkillId::KULTURKANNEDOM,
                SkillId::LASDYRKNING, SkillId::MARSCHERA, SkillId::MUTA, SkillId::SIMMA,
                SkillId::SLA_MEDVETSLOST, SkillId::SLOSS_TILL_HAST, SkillId::STRID_I_LUFTEN,
                SkillId::STRID_I_MORKER, SkillId::STRIDSKONSTER, SkillId::TRASTAV,
                SkillId::UNDRE_VARLDEN
            },
            true
        },
        // ---- Riddare (Warrior Expansion) ----
        {
            "Riddare (Krigarexpansion)",
            "A warrior of noble birth sworn to protect the weak and uphold "
            "honour — expanded version with additional warrior skills.",
            "By spending 5 PSY points the Knight may either: (1) target any "
            "body part without penalty (detailed combat), or (2) deal maximum "
            "weapon damage including maximum damage bonus. Armour absorption "
            "still applies. PSY recovers normally.",
            {{"STY", 14}, {"FYS", 12}, {"PSY", 12}},
            12, 5, 1, 1, 2,
            {
                SkillId::VAPENFARDIGHET,
                SkillId::TALA_FRAMMANDE_SPRAK,
                SkillId::LASA_SKRIVA_FRAMMANDE_SPRAK,
                SkillId::ADMINISTRATION, SkillId::AVVAPNA, SkillId::BRYTA_GREPP,
                SkillId::DANS, SkillId::DJURTRANING, SkillId::DRA_VAPEN, SkillId::DOLK,
                SkillId::DUELLERA, SkillId::FALLTEKNIK, SkillId::GEOGRAFI, SkillId::HARSKRI,
                SkillId::HERALDIK, SkillId::HISTORIA, SkillId::KULTURKANNEDOM,
                SkillId::KUNSKAP_OM_MAGI, SkillId::KUNSKAP_OM_ODODA,
                SkillId::MALNING, SkillId::RAKNING, SkillId::SCHACK_BRADSPEL, SkillId::SIMMA,
                SkillId::SLA_MEDVETSLOST, SkillId::SLOSS_TILL_HAST, SkillId::SPELA_INSTRUMENT,
                SkillId::SPRAKKUNSKAP, SkillId::STRATEGI, SkillId::STRID_I_LUFTEN, SkillId::STRID_I_MORKER,
                SkillId::TAKTIK, SkillId::TRASTAV, SkillId::TVA_VAPEN, SkillId::VAGNSFORARE, SkillId::VAPENTEKNIK
            },
            true
        },
        // ---- Soldat ----
        {
            "Soldat",
            "The soldier serves in an army belonging to a country, lord, or "
            "religious order — from conscripted levy to elite troops.",
            "The soldier has good friends and contacts within at least one army. "
            "They can call on KAR/4 favours per year: a private gets 1T3 friends, "
            "a sergeant 1T6, a captain 2T6, a colonel 3T10. They can always find "
            "employment and sanctuary in their army.",
            {{"STY", 14}, {"FYS", 12}},
            12, 5, 1, 0, 0,
            {
                SkillId::VAPENFARDIGHET,
                SkillId::TALA_FRAMMANDE_SPRAK,
                SkillId::ADMINISTRATION, SkillId::ANFALL_BAKIFRAN, SkillId::AVVAPNA,
                SkillId::BRYTA_GREPP, SkillId::DJURTRANING, SkillId::DOLK, SkillId::DRA_VAPEN,
                SkillId::FALLTEKNIK, SkillId::GEOGRAFI, SkillId::HANTVERK, SkillId::HASARDSPEL,
                SkillId::HERALDIK, SkillId::HISTORIA, SkillId::HARSKRI, SkillId::KAMOUFLAGE,
                SkillId::KULTURKANNEDOM, SkillId::MARSCHERA, SkillId::MUTA, SkillId::ORIENTERING,
                SkillId::RAKNING, SkillId::SIMMA, SkillId::SLA_MEDVETSLOST, SkillId::SLOSS_TILL_HAST,
                SkillId::STRATEGI, SkillId::STRID_I_LUFTEN, SkillId::STRID_I_MORKER,
                SkillId::TAKTIK, SkillId::TRASTAV, SkillId::TVA_VAPEN, SkillId::VAGNSFORARE, SkillId::OVERLEVNAD
            },
            true
        },
        // ---- Sprätthök ----
        {
            "Sprätthök",
            "The swashbuckler does whatever takes their fancy. Life is fun and "
            "full of possibilities — beautiful women, secret documents to steal, "
            "evil dukes to outwit and good wine to drink.",
            "The Swashbuckler can succeed at almost anything heroic and daring "
            "that would be impossible for others — swinging on chandeliers, "
            "tripping soldiers, knifing through sails — as long as it advances "
            "the story and keeps things exciting. The Game Master decides.",
            {{"STY", 14}, {"FYS", 12}, {"SMI", 15}},
            12, 3, 3, 2, 0,
            {
                SkillId::VAPENFARDIGHET,
                SkillId::TALA_FRAMMANDE_SPRAK,
                SkillId::LASA_SKRIVA_FRAMMANDE_SPRAK,
                SkillId::AVVAPNA, SkillId::BRYTA_GREPP, SkillId::BUKTALA, SkillId::DANS,
                SkillId::DOLK, SkillId::DRA_VAPEN, SkillId::DUELLERA, SkillId::FALLTEKNIK,
                SkillId::FORFALSKNING, SkillId::GEOGRAFI, SkillId::HASARDSPEL, SkillId::HISTORIA,
                SkillId::HARSKRI, SkillId::KULTURKANNEDOM, SkillId::LASDYRKNING, SkillId::MUTA,
                SkillId::SIMMA, SkillId::SKADESPELERI, SkillId::SLA_MEDVETSLOST, SkillId::STRID_I_MORKER,
                SkillId::TRASTAV, SkillId::TVA_VAPEN, SkillId::UNDRE_VARLDEN
            },
            true
        },
        // ---- Vapenmästare ----
        {
            "Vapenmästare",
            "The weapon master dedicates their life to perfecting a weapon art. "
            "All their time goes into practice and refinement. Many are peaceful "
            "and have never raised a weapon against anyone.",
            "By spending 5 PSY points the Weapon Master may make one extra parry "
            "or one extra attack beyond normal for one combat round. "
            "PSY recovers normally.",
            {{"STY", 14}, {"FYS", 12}, {"SMI", 15}, {"PSY", 12}},
            12, 2, 3, 3, 0,
            {
                SkillId::VAPENFARDIGHET,
                SkillId::TALA_FRAMMANDE_SPRAK,
                SkillId::LASA_SKRIVA_FRAMMANDE_SPRAK,
                SkillId::ANFALL_BAKIFRAN, SkillId::AVVAPNA, SkillId::BRYTA_GREPP,
                SkillId::DANS, SkillId::DOLK, SkillId::DRA_VAPEN, SkillId::DUELLERA,
                SkillId::FALLTEKNIK, SkillId::GEOGRAFI, SkillId::HISTORIA, SkillId::HARSKRI,
                SkillId::KULTURKANNEDOM, SkillId::SIMMA, SkillId::SLA_MEDVETSLOST,
                SkillId::SLOSS_TILL_HAST, SkillId::STRATEGI, SkillId::STRID_I_LUFTEN,
                SkillId::STRID_I_MORKER, SkillId::STRIDSKONSTER, SkillId::TAKTIK,
                SkillId::TRASTAV, SkillId::VAPENTEKNIK
            },
            true
        },
    };
    return profs;
}

// ---------------------------------------------------------------------------
// AGE CATEGORIES
// ---------------------------------------------------------------------------
const std::vector<AgeCategory>& getAgeCategories() {
    static const std::vector<AgeCategory> ages = {
        // Ung
        {
            "Young (Ung)",
            {-1, +1, +1, 0, -1, 0, 0},
            150, 1.0, 13,
            "Human 16-20 | Dvärg 21-40 | Halvalv/Halvlängdsman 20-30 | Alv: always counts as Young for EP"
        },
        // Mogen
        {
            "Mature (Mogen)",
            {0, 0, 0, 0, 0, 0, 0},
            200, 1.5, 15,
            "Human 21-45 | Dvärg 41-150 | Halvlängdsman 31-60 | Halvalv 41-155"
        },
        // Medelålder
        {
            "Middle-aged (Medelålder)",
            {-2, -1, -1, -1, +2, +1, 0},
            250, 2.0, 17,
            "Human 46-60 | Dvärg 151-250 | Halvlängdsman 61-75 | Halvalv 156-400"
        },
        // Gammal
        {
            "Old (Gammal)",
            {-5, -3, -3, +1, +4, +1, 0},
            300, 2.5, 19,
            "Human 61-80 | Dvärg 251-400 | Halvlängdsman 76-100"
        },
    };
    return ages;
}

// ---------------------------------------------------------------------------
// SOCIAL STANDING TABLE
// (2T6+BP roll result → tier; capital is then further multiplied by age)
// ---------------------------------------------------------------------------
const std::vector<SocialStandingEntry>& getSocialStandingTable() {
    static const std::vector<SocialStandingEntry> table = {
        { 2,  2, "Propertyless (thrall, slave, convict, beggar-monk)",     50},
        { 3,  4, "Lower underclass (beggar, gravedigger, latrine-cleaner)",100},
        { 5,  7, "Upper underclass (apprentice, journeyman, novice)",      200},
        { 8, 11, "Lower middle class (craftsman, trader, city guard)",     500},
        {12, 16, "Upper middle class (merchant, priest, freeholder)",     2000},
        {17, 22, "Lower upper class (councillor, court official)",        5000},
        {23, 29, "Upper upper class (burgher, guild master)",            10000},
        {30, 37, "Low nobility (knight, baron, bishop)",                 20000},
        {38, 99, "High nobility (count, duke, king, emperor)",           50000},
    };
    return table;
}

// ---------------------------------------------------------------------------
// LOOKUP HELPERS
// ---------------------------------------------------------------------------
std::optional<ProfessionDefinition> findProfession(const std::string& name) {
    for (const auto& p : getProfessions())
        if (p.name == name) return p;
    return std::nullopt;
}

std::optional<RaceDefinition> findRace(const std::string& name) {
    for (const auto& r : getRaces())
        if (r.name == name) return r;
    return std::nullopt;
}

std::optional<SkillDefinition> findSkill(const std::string& name) {
    for (const auto& s : getAllSkills())
        if (skillIdToString(s.id) == name) return s;
    return std::nullopt;
}

std::optional<SkillDefinition> findSkill(SkillId id) {
    for (const auto& s : getAllSkills())
        if (s.id == id) return s;
    return std::nullopt;
}

// ---------------------------------------------------------------------------
// SPECIAL ABILITY GRANTS
// Maps a 2T20+BP roll total to a list of structured skill bonuses.
// CL bonuses are stored as annotations (display-only); FV bonuses are applied.
// ---------------------------------------------------------------------------
std::vector<SpecialAbilityGrant> getSpecialAbilityGrants(int roll) {
    using T = SpecialAbilityGrant::Type;

    if (roll <= 4)  return {{ T::FV, SkillId::PLAYER_CHOICE,              1, true  }};  // any secondary skill
    if (roll <= 6)  return {{ T::FV, SkillId::SJOKUNNIGHET,               2, false },
                             { T::FV, SkillId::NAVIGERA,                   2, false }};
    if (roll <= 8)  return {{ T::FV, SkillId::HOPPA,                      3, false }};
    if (roll <= 10) return {{ T::FV, SkillId::SLAGSMAL,                   3, false }};
    if (roll <= 12) return {{ T::FV, SkillId::HANTVERK,                   3, true  }};  // player picks which Hantverk
    if (roll <= 14) return {{ T::FV, SkillId::AKROBATIK,                  3, false }};
    if (roll <= 16) return {{ T::FV, SkillId::VARDERA,                    3, false }};
    if (roll <= 18) return {{ T::FV, SkillId::TVA_VAPEN,                  3, false }};
    if (roll <= 20) return {{ T::FV, SkillId::PLAYER_CHOICE,              3, true  }};  // any secondary: set FV to 3
    if (roll <= 22) return {{ T::CL, SkillId::KLATRA,                     3, false }};
    if (roll <= 24) return {{ T::CL, SkillId::MAGISK_KANALISERING,        5, false }};
    if (roll <= 26) return {{ T::FV, SkillId::PLAYER_CHOICE,              2, true  }};  // any one skill
    if (roll <= 28) return {{ T::CL, SkillId::OVERTALA,                   3, false },
                             { T::CL, SkillId::MUTA,                       3, false }};
    if (roll <= 30) return {{ T::FV, SkillId::UPPTACKA_FARA,              1, false },
                             { T::FV, SkillId::FINNA_DOLDA_TING,           1, false }};
    if (roll <= 32) return {{ T::CL, SkillId::HYPNOTISERA,                5, false }};
    if (roll <= 34) return {{ T::CL, SkillId::KANNA_MAGI,                 5, false }};
    if (roll <= 36) return {{ T::FV, SkillId::TALA_FRAMMANDE_SPRAK,      20, true  },   // player picks which language
                             { T::FV, SkillId::LASA_SKRIVA_FRAMMANDE_SPRAK, 20, true }};
    if (roll <= 54) return {{ T::CL, SkillId::FINNA_DOLDA_TING,           2, false },
                             { T::CL, SkillId::UPPTACKA_FARA,              2, false }};
    if (roll == 56) return {{ T::FV, SkillId::BARSARKAGANGA,              5, false }};
    if (roll == 58) return {{ T::FV, SkillId::RIDA,                      10, false }};
    if (roll == 75) return {{ T::FV, SkillId::VAPENFARDIGHET,             5, true  }};  // player picks weapon
    // All other rolls are passive abilities — no direct skill grant
    return {};
}

// ---------------------------------------------------------------------------
// WARRIOR EXPANSION — BP LEVELS
// ---------------------------------------------------------------------------
const std::vector<BPLevelDefinition>& getBPLevels() {
    static const std::vector<BPLevelDefinition> levels = {
        // name,        bp,  saRolls, ep[4](young/mat/mid/old),  maxFV[4](young/mat/mid/old)
        { "Vanlig",      125, 1, {150, 200, 250, 300}, {13, 15, 17, 19} },
        { "Extraordinär",150, 2, {175, 225, 275, 325}, {15, 17, 19, 20} },
        { "Hjälte",      175, 3, {200, 250, 300, 350}, {17, 19, 20, 20} },
    };
    return levels;
}

// ---------------------------------------------------------------------------
// WARRIOR EXPANSION — SPECIAL ABILITY TEXT
// ---------------------------------------------------------------------------
std::string lookupWarriorSpecialAbility(int roll) {
    if (roll <= 4)  return "Bonus FV+1 in any one secondary skill.";
    if (roll <= 6)  return "Seafaring background: +2 FV in Sjökunnighet and Navigera.";
    if (roll <= 8)  return "Strong ankles: +3 FV to Hoppa.";
    if (roll <= 10) return "Street brawler upbringing: +3 FV to Slagsmål.";
    if (roll <= 12) return "Craftsman background: +3 FV in one chosen Hantverk.";
    if (roll <= 14) return "Nimble frame: +3 FV to Akrobatik.";
    if (roll <= 16) return "Tracker's eye: +3 FV to Orientering.";
    if (roll <= 18) return "Excellent co-ordination: +3 FV to Två vapen.";
    if (roll <= 20) return "Soldier's education: +3 FV in Strategi and Taktik.";
    if (roll <= 22) return "Strong grip: +3 to CL in Klättra.";
    if (roll <= 24) return "Horse eye: always picks the best horse from a herd.";
    if (roll <= 26) return "Good immune system: double FYS when resisting disease.";
    if (roll <= 28) return "Natural leader: +2 KAR; troops you lead gain +5 morale.";
    if (roll <= 30) return "Sixth sense: +1 FV in Upptäcka fara and Finna dolda ting.";
    if (roll <= 32) return "Night sight: see clearly in starlight or moonlight.";
    if (roll <= 34) return "Fast draw: +3 FV to Dra vapen.";
    if (roll <= 36) return "Linguistic talent: FV 20 in one spoken and written language.";
    if (roll <= 38) return "Wide expertise: choose 2 extra secondary skills as profession skills.";
    if (roll <= 40) return "Skilled archer: all projectile weapon ranges +25%.";
    if (roll <= 42) return "Poison resistant: double FYS when resisting poisons.";
    if (roll <= 44) return "Swift-footed: can move double movement for up to 10 rounds.";
    if (roll <= 46) return "Precision sense: CL +1 on all weapon skills.";
    if (roll <= 48) return "Ambidextrous: use both hands without penalty.";
    if (roll <= 50) return "Perfect time sense: always know time within 1 minute.";
    if (roll <= 52) return "Very observant: +2 CL in Finna dolda ting and Upptäcka fara.";
    if (roll <= 54) return "Lightning reflexes: +3 on all initiative rolls.";
    if (roll <= 56) return "Berserker: +5 FV in Bärsärkagång.";
    if (roll == 57) return "Perfect balance: +5 SMI on all balance acts.";
    if (roll == 58) return "Master horseman: +10 FV in Rida; never thrown by a horse.";
    if (roll == 59) return "Ambidextrous (superior): full bilateral use of both hands.";
    if (roll == 60) return "Animal friend: never attacked by normal animals.";
    if (roll == 61) return "Lucky: spend 1 PSY for +1 CL on any skill.";
    if (roll == 62) return "Cat-footed: always land on your feet; take half fall damage.";
    if (roll == 63) return "Deity's favoured: 25% chance gods restore all HP at 0.";
    if (roll == 64) return "Pain tolerance: total SP ×1.5.";
    if (roll == 65) return "Stryktålig: total HP ×1.5.";
    if (roll == 66) return "Lightning striker: always acts first in combat.";
    if (roll == 67) return "Foe-bane: +5 CL against one sworn enemy race/group.";
    if (roll == 68) return "Body control: raise STY and STY-skills by +5 for 3 rounds (twice/day).";
    if (roll == 69) return "Iron fist: always deals maximum unarmed damage.";
    if (roll == 70) return "Extremely fearless: -5 on all Fear Table rolls.";
    if (roll == 71) return "Extreme night vision: fight without penalty in complete darkness.";
    if (roll == 72) return "Elemental hardy: +5 FYS vs all elemental resistance rolls.";
    if (roll == 73) return "Fast healing: physical damage heals at double normal rate.";
    if (roll == 74) return "Find gap: find holes in armour — opponents get -3 to KP absorption. "
                           "Value cannot go negative.";
    if (roll == 75) return "Extrem empati: +10 FV in Rida, Slåss till häst and Strid i luften.";
    if (roll == 76) return "Split personality: gain ability of one other chosen profession.";
    if (roll == 77) return "Professional mastery: cost to learn weapon skills halved.";
    if (roll == 78) return "Shapeshifter: transform into an animal "
                           "(T6: 1=wolf,2=bear,3=lion,4=wolverine,5=eagle,6=tiger).";
    if (roll == 79) return "Quick perception: +5 CL to parry; can parry projectiles you see coming.";
    if (roll == 80) return "Extremely fast: one extra action per combat round.";
    return "+1 on three different chosen stats, or +2 on one chosen stat.";
}

// ---------------------------------------------------------------------------
// WARRIOR EXPANSION — SPECIAL ABILITY GRANTS
// ---------------------------------------------------------------------------
std::vector<SpecialAbilityGrant> getWarriorSpecialAbilityGrants(int roll) {
    using T = SpecialAbilityGrant::Type;

    if (roll <= 4)  return {{ T::FV, SkillId::PLAYER_CHOICE,              1, true  }};
    if (roll <= 6)  return {{ T::FV, SkillId::SJOKUNNIGHET,               2, false },
                             { T::FV, SkillId::NAVIGERA,                   2, false }};
    if (roll <= 8)  return {{ T::FV, SkillId::HOPPA,                      3, false }};
    if (roll <= 10) return {{ T::FV, SkillId::SLAGSMAL,                   3, false }};
    if (roll <= 12) return {{ T::FV, SkillId::HANTVERK,                   3, true  }};
    if (roll <= 14) return {{ T::FV, SkillId::AKROBATIK,                  3, false }};
    if (roll <= 16) return {{ T::FV, SkillId::ORIENTERING,                3, false }};
    if (roll <= 18) return {{ T::FV, SkillId::TVA_VAPEN,                  3, false }};
    if (roll <= 20) return {{ T::FV, SkillId::STRATEGI,                   3, false },
                             { T::FV, SkillId::TAKTIK,                     3, false }};
    if (roll <= 22) return {{ T::CL, SkillId::KLATRA,                     3, false }};
    // 23-26 are passive (horse-eye, immune) — no structured grant
    if (roll <= 30) return {{ T::FV, SkillId::UPPTACKA_FARA,              1, false },
                             { T::FV, SkillId::FINNA_DOLDA_TING,           1, false }};
    if (roll <= 34) return {{ T::FV, SkillId::DRA_VAPEN,                  3, false }};
    if (roll <= 36) return {{ T::FV, SkillId::TALA_FRAMMANDE_SPRAK,      20, true  },
                             { T::FV, SkillId::LASA_SKRIVA_FRAMMANDE_SPRAK, 20, true }};
    // 37-44 passive
    if (roll <= 46) return {{ T::CL, SkillId::VAPENFARDIGHET,             1, false }};
    // 47-54 passive (weapon hand handled separately like base table, 55-56 below)
    if (roll <= 56) return {{ T::FV, SkillId::BARSARKAGANGA,              5, false }};
    if (roll == 58) return {{ T::FV, SkillId::RIDA,                      10, false }};
    if (roll == 75) return {{ T::FV, SkillId::RIDA,                      10, false },
                             { T::FV, SkillId::SLOSS_TILL_HAST,           10, false },
                             { T::FV, SkillId::STRID_I_LUFTEN,            10, false }};
    return {};
}

} // namespace GameData
} // namespace dod
