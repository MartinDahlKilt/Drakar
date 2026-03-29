#include "core/GameData.h"
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
                {"Upptäcka fara", 4, false},
                {"Lyssna",        4, false}
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
                {"Simma", 20, true},   // FV set to 20
                {"Smyga",  4, false}
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
                {"Geologi", 5, true}   // FV set to 5
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
                {"Upptäcka fara", 2, false},
                {"Lyssna",        2, false}
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
                {"Gömma sig", 4, false}
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
                {"Slagsmål", 4, false}
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
        {"Bluffa",               SkillType::PRIMARY,   "KAR"},
        {"Finna dolda ting",     SkillType::PRIMARY,   "INT"},
        {"Första hjälpen",       SkillType::PRIMARY,   "INT"},
        {"Gömma sig",            SkillType::PRIMARY,   "INT"},
        {"Hoppa",                SkillType::PRIMARY,   "SMI"},
        {"Klättra",              SkillType::PRIMARY,   "SMI"},
        {"Köpslå",               SkillType::PRIMARY,   "KAR"},
        {"Lyssna",               SkillType::PRIMARY,   "INT"},
        {"Läsa/Skriva modersmål",SkillType::PRIMARY,   "INT", false, false, true},
        {"Rida",                 SkillType::PRIMARY,   "SMI"},
        {"Sjunga",               SkillType::PRIMARY,   "KAR"},
        {"Slagsmål",             SkillType::PRIMARY,   "STY"},
        {"Smyga",                SkillType::PRIMARY,   "SMI"},
        {"Spåra",                SkillType::PRIMARY,   "INT"},
        {"Stjäla föremål",       SkillType::PRIMARY,   "SMI"},
        {"Tala modersmål",       SkillType::PRIMARY,   "INT"},
        {"Upptäcka fara",        SkillType::PRIMARY,   "PSY"},
        {"Värdera",              SkillType::PRIMARY,   "INT"},
        {"Övertala",             SkillType::PRIMARY,   "KAR"},

        // ---- Secondary skills (non-weapon) ----
        {"Administration",       SkillType::SECONDARY, "INT"},
        {"Akrobatik",            SkillType::SECONDARY, "SMI"},
        {"Alkemi",               SkillType::SECONDARY, "INT"},
        {"Animism",              SkillType::SECONDARY, "PSY", false, false, false, false, false, true},
        {"Astrologi",            SkillType::SECONDARY, "INT"},
        {"Avväpna",              SkillType::SECONDARY, "SMI"},
        {"Buktala",              SkillType::SECONDARY, "PSY"},
        {"Bärsärkagång",         SkillType::SECONDARY, "PSY"},
        {"Dans",                 SkillType::SECONDARY, "SMI"},
        {"Djurhelning",          SkillType::SECONDARY, "INT"},
        {"Djurträning",          SkillType::SECONDARY, "PSY"},
        {"Dolk",                 SkillType::SECONDARY, "SMI", true},
        {"Dra vapen",            SkillType::SECONDARY, "SMI"},
        {"Drogkunskap",          SkillType::SECONDARY, "INT"},
        {"Elementarmagi",        SkillType::SECONDARY, "INT", false, false, false, false, false, true},
        {"Förfalskning",         SkillType::SECONDARY, "INT"},
        {"Geografi",             SkillType::SECONDARY, "INT"},
        {"Geologi",              SkillType::SECONDARY, "INT"},
        {"Giftkunskap",          SkillType::SECONDARY, "INT"},
        {"Gyckelkonster",        SkillType::SECONDARY, "SMI"},
        {"Hantera fällor",       SkillType::SECONDARY, "SMI"},
        // Hantverk is parameterised — a single entry; players choose specific craft
        {"Hantverk",             SkillType::SECONDARY, "SMI", false, false, false, false, true},
        {"Hasardspel",           SkillType::SECONDARY, "INT"},
        {"Heraldik",             SkillType::SECONDARY, "INT"},
        {"Historia",             SkillType::SECONDARY, "INT"},
        {"Hypnotisera",          SkillType::SECONDARY, "PSY"},
        {"Knopar",               SkillType::SECONDARY, "SMI"},
        {"Kulturkännedom",       SkillType::SECONDARY, "INT"},
        {"Kunskap om demoner",   SkillType::SECONDARY, "INT"},
        {"Kunskap om magi",      SkillType::SECONDARY, "INT"},
        {"Kunskap om odöda",     SkillType::SECONDARY, "INT"},
        {"Känna magi",           SkillType::SECONDARY, "PSY"},
        {"Låsdyrkning",          SkillType::SECONDARY, "SMI"},
        {"Läkekonst",            SkillType::SECONDARY, "INT"},
        {"Läppläsning",          SkillType::SECONDARY, "INT"},
        {"Läsa/Skriva främmande språk", SkillType::SECONDARY, "INT", false, false, true},
        {"Magisk kanalisering",  SkillType::SECONDARY, "INT"},
        {"Massage",              SkillType::SECONDARY, "SMI"},
        {"Mentalism",            SkillType::SECONDARY, "PSY", false, false, false, false, false, true},
        {"Muta",                 SkillType::SECONDARY, "KAR"},
        {"Målning",              SkillType::SECONDARY, "SMI"},
        {"Navigera",             SkillType::SECONDARY, "INT"},
        {"Orientering",          SkillType::SECONDARY, "INT"},
        {"Räkning",              SkillType::SECONDARY, "INT"},
        {"Schack & brädspel",    SkillType::SECONDARY, "INT"},
        {"Simma",                SkillType::SECONDARY, "FYS"},
        {"Sjökunnighet",         SkillType::SECONDARY, "INT"},
        {"Skådespeleri",         SkillType::SECONDARY, "KAR"},
        {"Spela instrument",     SkillType::SECONDARY, "KAR", false, false, false, true},
        {"Språkkunskap",         SkillType::SECONDARY, "INT"},
        {"Spå väder",            SkillType::SECONDARY, "INT"},
        {"Stavhopp",             SkillType::SECONDARY, "SMI"},
        {"Stridskonster",        SkillType::SECONDARY, "SMI"},
        {"Tala främmande språk", SkillType::SECONDARY, "INT", false, true},
        {"Teckenspråk",          SkillType::SECONDARY, "INT"},
        {"Trästav",              SkillType::SECONDARY, "SMI", true},
        {"Två vapen",            SkillType::SECONDARY, "SMI"},
        {"Undre världen",        SkillType::SECONDARY, "INT"},
        {"Vapenfärdighet",       SkillType::SECONDARY, "var.", true},
        {"Zoologi",              SkillType::SECONDARY, "INT"},
        {"Änterhake",            SkillType::SECONDARY, "SMI"},
        {"Örtkunskap",           SkillType::SECONDARY, "INT"},
        {"Överlevnad",           SkillType::SECONDARY, "INT"},
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
                "Vapenfärdighet",          // 1 any
                "Tala främmande språk",    // max 2
                "Läsa/Skriva främmande språk", // max 1
                "Administration", "Akrobatik", "Buktala", "Dans", "Djurträning",
                "Dolk", "Förfalskning", "Geografi", "Gyckelkonster", "Hantverk",
                "Hasardspel", "Heraldik", "Historia", "Hypnotisera", "Knopar",
                "Kulturkännedom", "Låsdyrkning", "Läppläsning", "Muta",
                "Målning", "Schack & brädspel", "Simma", "Skådespeleri",
                "Spela instrument", "Språkkunskap", "Trästav"
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
                "Tala främmande språk", "Läsa/Skriva främmande språk",
                "Alkemi", "Djurhelning", "Dolk", "Drogkunskap", "Geografi",
                "Giftkunskap", "Hantverk", "Hypnotisera", "Kulturkännedom",
                "Kunskap om demoner", "Kunskap om magi", "Kunskap om odöda",
                "Läkekonst", "Massage", "Orientering", "Simma", "Trästav",
                "Zoologi", "Örtkunskap", "Överlevnad"
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
                "Vapenfärdighet",            // unlimited weapon skills
                "Tala främmande språk",
                "Avväpna", "Bärsärkagång", "Dra vapen", "Dolk", "Geografi",
                "Hantverk", "Hasardspel", "Kulturkännedom", "Simma",
                "Stridskonster", "Trästav", "Två vapen"
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
                "Tala främmande språk",
                "Läsa/Skriva främmande språk",
                "Administration", "Alkemi", "Astrologi", "Dolk", "Drogkunskap",
                "Förfalskning", "Geografi", "Geologi", "Giftkunskap",
                "Hasardspel", "Heraldik", "Historia", "Kulturkännedom",
                "Kunskap om demoner", "Kunskap om magi", "Kunskap om odöda",
                "Räkning", "Schack & brädspel", "Simma", "Språkkunskap",
                "Trästav", "Zoologi", "Örtkunskap"
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
                "Vapenfärdighet",
                "Tala främmande språk",
                "Administration", "Akrobatik", "Dra vapen", "Dolk", "Förfalskning",
                "Geografi", "Giftkunskap", "Hantera fällor", "Hasardspel",
                "Hypnotisera", "Knopar", "Kulturkännedom", "Låsdyrkning",
                "Muta", "Simma", "Skådespeleri", "Stavhopp", "Stridskonster",
                "Teckenspråk", "Trästav", "Undre världen", "Änterhake"
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
                "Tala främmande språk",
                "Läsa/Skriva främmande språk",
                "Alkemi", "Astrologi", "Djurhelning", "Djurträning",
                "Drogkunskap", "Geografi", "Giftkunskap", "Kulturkännedom",
                "Kunskap om demoner", "Kunskap om magi", "Kunskap om odöda",
                "Magisk kanalisering",
                "Animism", "Elementarmagi", "Mentalism",  // choose one magic school
                "Räkning", "Simma", "Språkkunskap", "Trästav",
                "Zoologi", "Örtkunskap"
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
                "Tala främmande språk",
                "Läsa/Skriva främmande språk",
                "Avväpna", "Djurhelning", "Dolk", "Drogkunskap", "Förfalskning",
                "Geografi", "Giftkunskap", "Hantverk", "Heraldik", "Historia",
                "Knopar", "Kulturkännedom", "Kunskap om demoner",
                "Kunskap om magi", "Kunskap om odöda", "Läkekonst", "Massage",
                "Målning", "Räkning", "Simma", "Spela instrument",
                "Språkkunskap", "Stridskonster", "Trästav", "Zoologi", "Örtkunskap"
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
                "Vapenfärdighet",
                "Tala främmande språk",
                "Läsa/Skriva främmande språk",
                "Akrobatik", "Dans", "Dolk", "Geografi", "Hantverk",
                "Hasardspel", "Gyckelkonster", "Knopar", "Kulturkännedom",
                "Muta", "Navigera", "Orientering", "Schack & brädspel",
                "Simma", "Sjökunnighet", "Spela instrument", "Spå väder",
                "Stavhopp", "Trästav", "Undre världen", "Änterhake"
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
                "Vapenfärdighet",
                "Tala främmande språk",
                "Läsa/Skriva främmande språk",
                "Administration", "Avväpna", "Dans", "Djurträning",
                "Dra vapen", "Dolk", "Geografi", "Heraldik", "Historia",
                "Kulturkännedom", "Kunskap om magi", "Kunskap om odöda",
                "Målning", "Räkning", "Schack & brädspel", "Simma",
                "Spela instrument", "Språkkunskap", "Trästav", "Två vapen"
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
                "Vapenfärdighet",
                "Tala främmande språk",
                "Administration", "Akrobatik", "Buktala", "Dra vapen", "Dolk",
                "Drogkunskap", "Förfalskning", "Geografi", "Giftkunskap",
                "Hypnotisera", "Gyckelkonster", "Knopar", "Kulturkännedom",
                "Låsdyrkning", "Läppläsning", "Muta", "Räkning", "Simma",
                "Skådespeleri", "Spela instrument", "Stavhopp",
                "Teckenspråk", "Trästav", "Undre världen", "Änterhake"
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
                "Vapenfärdighet",
                "Tala främmande språk",
                "Animism", "Djurhelning", "Djurträning", "Dolk", "Drogkunskap",
                "Geografi", "Geologi", "Giftkunskap", "Hantverk",
                "Hantera fällor", "Knopar", "Kulturkännedom", "Orientering",
                "Simma", "Spå väder", "Zoologi", "Örtkunskap", "Överlevnad"
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
        if (s.name == name) return s;
    return std::nullopt;
}

// ---------------------------------------------------------------------------
// SPECIAL ABILITY GRANTS
// Maps a 2T20+BP roll total to a list of structured skill bonuses.
// CL bonuses are stored as annotations (display-only); FV bonuses are applied.
// ---------------------------------------------------------------------------
std::vector<SpecialAbilityGrant> getSpecialAbilityGrants(int roll) {
    using T = SpecialAbilityGrant::Type;

    if (roll <= 4)  return {{ T::FV, "",                      1, true  }};  // any secondary skill
    if (roll <= 6)  return {{ T::FV, "Sjökunnighet",           2, false },
                             { T::FV, "Navigera",               2, false }};
    if (roll <= 8)  return {{ T::FV, "Hoppa",                  3, false }};
    if (roll <= 10) return {{ T::FV, "Slagsmål",               3, false }};
    if (roll <= 12) return {{ T::FV, "Hantverk",               3, true  }};  // player picks which Hantverk
    if (roll <= 14) return {{ T::FV, "Akrobatik",              3, false }};
    if (roll <= 16) return {{ T::FV, "Värdera",                3, false }};
    if (roll <= 18) return {{ T::FV, "Två vapen",              3, false }};
    if (roll <= 20) return {{ T::FV, "",                       3, true  }};  // any secondary: set FV to 3
    if (roll <= 22) return {{ T::CL, "Klättra",                3, false }};
    if (roll <= 24) return {{ T::CL, "Magisk kanalisering",    5, false }};
    if (roll <= 26) return {{ T::FV, "",                       2, true  }};  // any one skill
    if (roll <= 28) return {{ T::CL, "Övertala",               3, false },
                             { T::CL, "Muta",                   3, false }};
    if (roll <= 30) return {{ T::FV, "Upptäcka fara",          1, false },
                             { T::FV, "Finna dolda ting",       1, false }};
    if (roll <= 32) return {{ T::CL, "Hypnotisera",            5, false }};
    if (roll <= 34) return {{ T::CL, "Känna magi",             5, false }};
    if (roll <= 36) return {{ T::FV, "Tala främmande språk",  20, true  },   // player picks which language
                             { T::FV, "Läsa/Skriva främmande språk", 20, true }};
    if (roll <= 54) return {{ T::CL, "Finna dolda ting",       2, false },
                             { T::CL, "Upptäcka fara",          2, false }};
    if (roll == 56) return {{ T::FV, "Bärsärkagång",           5, false }};
    if (roll == 58) return {{ T::FV, "Rida",                  10, false }};
    if (roll == 75) return {{ T::FV, "Vapenfärdighet",         5, true  }};  // player picks weapon
    // All other rolls are passive abilities — no direct skill grant
    return {};
}

} // namespace GameData
} // namespace dod
