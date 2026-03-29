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

} // namespace GameData
} // namespace dod
