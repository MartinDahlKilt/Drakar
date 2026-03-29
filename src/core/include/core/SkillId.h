#pragma once
#include <string>

namespace dod {

/// Typed identifiers for every skill in the game.
/// PLAYER_CHOICE is a sentinel used wherever the player must pick a skill
/// (replaces the old empty-string convention in SpecialAbilityGrant etc.).
enum class SkillId {
    // ---- Primary ----
    BLUFFA,
    FINNA_DOLDA_TING,
    FORSTA_HJALPEN,
    GOMMA_SIG,
    HOPPA,
    KLATRA,
    KOPSLA,
    LYSSNA,
    LASA_SKRIVA_MODERSMAL,
    RIDA,
    SJUNGA,
    SLAGSMAL,
    SMYGA,
    SPARA,
    STJALA_FOREMAL,
    TALA_MODERSMAL,
    UPPTACKA_FARA,
    VARDERA,
    OVERTALA,

    // ---- Secondary ----
    ADMINISTRATION,
    AKROBATIK,
    ALKEMI,
    ANIMISM,
    ASTROLOGI,
    AVVAPNA,
    BUKTALA,
    BARSARKAGANGA,
    DANS,
    DJURHELNING,
    DJURTRANING,
    DOLK,
    DRA_VAPEN,
    DROGKUNSKAP,
    ELEMENTARMAGI,
    FORFALSKNING,
    GEOGRAFI,
    GEOLOGI,
    GIFTKUNSKAP,
    GYCKELKONSTER,
    HANTERA_FALLOR,
    HANTVERK,
    HASARDSPEL,
    HERALDIK,
    HISTORIA,
    HYPNOTISERA,
    KNOPAR,
    KULTURKANNEDOM,
    KUNSKAP_OM_DEMONER,
    KUNSKAP_OM_MAGI,
    KUNSKAP_OM_ODODA,
    KANNA_MAGI,
    LASDYRKNING,
    LAKEKONST,
    LAPPLASNING,
    LASA_SKRIVA_FRAMMANDE_SPRAK,
    MAGISK_KANALISERING,
    MASSAGE,
    MENTALISM,
    MUTA,
    MALNING,
    NAVIGERA,
    ORIENTERING,
    RAKNING,
    SCHACK_BRADSPEL,
    SIMMA,
    SJOKUNNIGHET,
    SKADESPELERI,
    SPELA_INSTRUMENT,
    SPRAKKUNSKAP,
    SPA_VADER,
    STAVHOPP,
    STRIDSKONSTER,
    TALA_FRAMMANDE_SPRAK,
    TECKENSPRAK,
    TRASTAV,
    TVA_VAPEN,
    UNDRE_VARLDEN,
    VAPENFARDIGHET,
    ZOOLOGI,
    ANTERHAKE,
    ORTKUNSKAP,
    OVERLEVNAD,

    // ---- Sentinel ----
    PLAYER_CHOICE,   ///< Player must choose at runtime (was empty string "")
};

/// Swedish display name for a skill id, e.g. SLAGSMAL → "Slagsmål".
/// Returns "" for PLAYER_CHOICE.
inline const char* skillIdToString(SkillId id) {
    switch (id) {
        case SkillId::BLUFFA:                       return "Bluffa";
        case SkillId::FINNA_DOLDA_TING:             return "Finna dolda ting";
        case SkillId::FORSTA_HJALPEN:               return "Första hjälpen";
        case SkillId::GOMMA_SIG:                    return "Gömma sig";
        case SkillId::HOPPA:                        return "Hoppa";
        case SkillId::KLATRA:                       return "Klättra";
        case SkillId::KOPSLA:                       return "Köpslå";
        case SkillId::LYSSNA:                       return "Lyssna";
        case SkillId::LASA_SKRIVA_MODERSMAL:        return "Läsa/Skriva modersmål";
        case SkillId::RIDA:                         return "Rida";
        case SkillId::SJUNGA:                       return "Sjunga";
        case SkillId::SLAGSMAL:                     return "Slagsmål";
        case SkillId::SMYGA:                        return "Smyga";
        case SkillId::SPARA:                        return "Spåra";
        case SkillId::STJALA_FOREMAL:               return "Stjäla föremål";
        case SkillId::TALA_MODERSMAL:               return "Tala modersmål";
        case SkillId::UPPTACKA_FARA:                return "Upptäcka fara";
        case SkillId::VARDERA:                      return "Värdera";
        case SkillId::OVERTALA:                     return "Övertala";
        case SkillId::ADMINISTRATION:               return "Administration";
        case SkillId::AKROBATIK:                    return "Akrobatik";
        case SkillId::ALKEMI:                       return "Alkemi";
        case SkillId::ANIMISM:                      return "Animism";
        case SkillId::ASTROLOGI:                    return "Astrologi";
        case SkillId::AVVAPNA:                      return "Avväpna";
        case SkillId::BUKTALA:                      return "Buktala";
        case SkillId::BARSARKAGANGA:                return "Bärsärkagång";
        case SkillId::DANS:                         return "Dans";
        case SkillId::DJURHELNING:                  return "Djurhelning";
        case SkillId::DJURTRANING:                  return "Djurträning";
        case SkillId::DOLK:                         return "Dolk";
        case SkillId::DRA_VAPEN:                    return "Dra vapen";
        case SkillId::DROGKUNSKAP:                  return "Drogkunskap";
        case SkillId::ELEMENTARMAGI:                return "Elementarmagi";
        case SkillId::FORFALSKNING:                 return "Förfalskning";
        case SkillId::GEOGRAFI:                     return "Geografi";
        case SkillId::GEOLOGI:                      return "Geologi";
        case SkillId::GIFTKUNSKAP:                  return "Giftkunskap";
        case SkillId::GYCKELKONSTER:                return "Gyckelkonster";
        case SkillId::HANTERA_FALLOR:               return "Hantera fällor";
        case SkillId::HANTVERK:                     return "Hantverk";
        case SkillId::HASARDSPEL:                   return "Hasardspel";
        case SkillId::HERALDIK:                     return "Heraldik";
        case SkillId::HISTORIA:                     return "Historia";
        case SkillId::HYPNOTISERA:                  return "Hypnotisera";
        case SkillId::KNOPAR:                       return "Knopar";
        case SkillId::KULTURKANNEDOM:               return "Kulturkännedom";
        case SkillId::KUNSKAP_OM_DEMONER:           return "Kunskap om demoner";
        case SkillId::KUNSKAP_OM_MAGI:              return "Kunskap om magi";
        case SkillId::KUNSKAP_OM_ODODA:             return "Kunskap om odöda";
        case SkillId::KANNA_MAGI:                   return "Känna magi";
        case SkillId::LASDYRKNING:                  return "Låsdyrkning";
        case SkillId::LAKEKONST:                    return "Läkekonst";
        case SkillId::LAPPLASNING:                  return "Läppläsning";
        case SkillId::LASA_SKRIVA_FRAMMANDE_SPRAK:  return "Läsa/Skriva främmande språk";
        case SkillId::MAGISK_KANALISERING:          return "Magisk kanalisering";
        case SkillId::MASSAGE:                      return "Massage";
        case SkillId::MENTALISM:                    return "Mentalism";
        case SkillId::MUTA:                         return "Muta";
        case SkillId::MALNING:                      return "Målning";
        case SkillId::NAVIGERA:                     return "Navigera";
        case SkillId::ORIENTERING:                  return "Orientering";
        case SkillId::RAKNING:                      return "Räkning";
        case SkillId::SCHACK_BRADSPEL:              return "Schack & brädspel";
        case SkillId::SIMMA:                        return "Simma";
        case SkillId::SJOKUNNIGHET:                 return "Sjökunnighet";
        case SkillId::SKADESPELERI:                 return "Skådespeleri";
        case SkillId::SPELA_INSTRUMENT:             return "Spela instrument";
        case SkillId::SPRAKKUNSKAP:                 return "Språkkunskap";
        case SkillId::SPA_VADER:                    return "Spå väder";
        case SkillId::STAVHOPP:                     return "Stavhopp";
        case SkillId::STRIDSKONSTER:                return "Stridskonster";
        case SkillId::TALA_FRAMMANDE_SPRAK:         return "Tala främmande språk";
        case SkillId::TECKENSPRAK:                  return "Teckenspråk";
        case SkillId::TRASTAV:                      return "Trästav";
        case SkillId::TVA_VAPEN:                    return "Två vapen";
        case SkillId::UNDRE_VARLDEN:                return "Undre världen";
        case SkillId::VAPENFARDIGHET:               return "Vapenfärdighet";
        case SkillId::ZOOLOGI:                      return "Zoologi";
        case SkillId::ANTERHAKE:                    return "Änterhake";
        case SkillId::ORTKUNSKAP:                   return "Örtkunskap";
        case SkillId::OVERLEVNAD:                   return "Överlevnad";
        case SkillId::PLAYER_CHOICE:                return "";
    }
    return "";
}

/// Stable ASCII key for a skill id, e.g. SLAGSMAL → "SLAGSMAL".
/// Used as a durable JSON key independent of Swedish spelling.
/// Returns "PLAYER_CHOICE" for SkillId::PLAYER_CHOICE.
inline const char* skillIdToKey(SkillId id) {
    switch (id) {
        case SkillId::BLUFFA:                       return "BLUFFA";
        case SkillId::FINNA_DOLDA_TING:             return "FINNA_DOLDA_TING";
        case SkillId::FORSTA_HJALPEN:               return "FORSTA_HJALPEN";
        case SkillId::GOMMA_SIG:                    return "GOMMA_SIG";
        case SkillId::HOPPA:                        return "HOPPA";
        case SkillId::KLATRA:                       return "KLATRA";
        case SkillId::KOPSLA:                       return "KOPSLA";
        case SkillId::LYSSNA:                       return "LYSSNA";
        case SkillId::LASA_SKRIVA_MODERSMAL:        return "LASA_SKRIVA_MODERSMAL";
        case SkillId::RIDA:                         return "RIDA";
        case SkillId::SJUNGA:                       return "SJUNGA";
        case SkillId::SLAGSMAL:                     return "SLAGSMAL";
        case SkillId::SMYGA:                        return "SMYGA";
        case SkillId::SPARA:                        return "SPARA";
        case SkillId::STJALA_FOREMAL:               return "STJALA_FOREMAL";
        case SkillId::TALA_MODERSMAL:               return "TALA_MODERSMAL";
        case SkillId::UPPTACKA_FARA:                return "UPPTACKA_FARA";
        case SkillId::VARDERA:                      return "VARDERA";
        case SkillId::OVERTALA:                     return "OVERTALA";
        case SkillId::ADMINISTRATION:               return "ADMINISTRATION";
        case SkillId::AKROBATIK:                    return "AKROBATIK";
        case SkillId::ALKEMI:                       return "ALKEMI";
        case SkillId::ANIMISM:                      return "ANIMISM";
        case SkillId::ASTROLOGI:                    return "ASTROLOGI";
        case SkillId::AVVAPNA:                      return "AVVAPNA";
        case SkillId::BUKTALA:                      return "BUKTALA";
        case SkillId::BARSARKAGANGA:                return "BARSARKAGANGA";
        case SkillId::DANS:                         return "DANS";
        case SkillId::DJURHELNING:                  return "DJURHELNING";
        case SkillId::DJURTRANING:                  return "DJURTRANING";
        case SkillId::DOLK:                         return "DOLK";
        case SkillId::DRA_VAPEN:                    return "DRA_VAPEN";
        case SkillId::DROGKUNSKAP:                  return "DROGKUNSKAP";
        case SkillId::ELEMENTARMAGI:                return "ELEMENTARMAGI";
        case SkillId::FORFALSKNING:                 return "FORFALSKNING";
        case SkillId::GEOGRAFI:                     return "GEOGRAFI";
        case SkillId::GEOLOGI:                      return "GEOLOGI";
        case SkillId::GIFTKUNSKAP:                  return "GIFTKUNSKAP";
        case SkillId::GYCKELKONSTER:                return "GYCKELKONSTER";
        case SkillId::HANTERA_FALLOR:               return "HANTERA_FALLOR";
        case SkillId::HANTVERK:                     return "HANTVERK";
        case SkillId::HASARDSPEL:                   return "HASARDSPEL";
        case SkillId::HERALDIK:                     return "HERALDIK";
        case SkillId::HISTORIA:                     return "HISTORIA";
        case SkillId::HYPNOTISERA:                  return "HYPNOTISERA";
        case SkillId::KNOPAR:                       return "KNOPAR";
        case SkillId::KULTURKANNEDOM:               return "KULTURKANNEDOM";
        case SkillId::KUNSKAP_OM_DEMONER:           return "KUNSKAP_OM_DEMONER";
        case SkillId::KUNSKAP_OM_MAGI:              return "KUNSKAP_OM_MAGI";
        case SkillId::KUNSKAP_OM_ODODA:             return "KUNSKAP_OM_ODODA";
        case SkillId::KANNA_MAGI:                   return "KANNA_MAGI";
        case SkillId::LASDYRKNING:                  return "LASDYRKNING";
        case SkillId::LAKEKONST:                    return "LAKEKONST";
        case SkillId::LAPPLASNING:                  return "LAPPLASNING";
        case SkillId::LASA_SKRIVA_FRAMMANDE_SPRAK:  return "LASA_SKRIVA_FRAMMANDE_SPRAK";
        case SkillId::MAGISK_KANALISERING:          return "MAGISK_KANALISERING";
        case SkillId::MASSAGE:                      return "MASSAGE";
        case SkillId::MENTALISM:                    return "MENTALISM";
        case SkillId::MUTA:                         return "MUTA";
        case SkillId::MALNING:                      return "MALNING";
        case SkillId::NAVIGERA:                     return "NAVIGERA";
        case SkillId::ORIENTERING:                  return "ORIENTERING";
        case SkillId::RAKNING:                      return "RAKNING";
        case SkillId::SCHACK_BRADSPEL:              return "SCHACK_BRADSPEL";
        case SkillId::SIMMA:                        return "SIMMA";
        case SkillId::SJOKUNNIGHET:                 return "SJOKUNNIGHET";
        case SkillId::SKADESPELERI:                 return "SKADESPELERI";
        case SkillId::SPELA_INSTRUMENT:             return "SPELA_INSTRUMENT";
        case SkillId::SPRAKKUNSKAP:                 return "SPRAKKUNSKAP";
        case SkillId::SPA_VADER:                    return "SPA_VADER";
        case SkillId::STAVHOPP:                     return "STAVHOPP";
        case SkillId::STRIDSKONSTER:                return "STRIDSKONSTER";
        case SkillId::TALA_FRAMMANDE_SPRAK:         return "TALA_FRAMMANDE_SPRAK";
        case SkillId::TECKENSPRAK:                  return "TECKENSPRAK";
        case SkillId::TRASTAV:                      return "TRASTAV";
        case SkillId::TVA_VAPEN:                    return "TVA_VAPEN";
        case SkillId::UNDRE_VARLDEN:                return "UNDRE_VARLDEN";
        case SkillId::VAPENFARDIGHET:               return "VAPENFARDIGHET";
        case SkillId::ZOOLOGI:                      return "ZOOLOGI";
        case SkillId::ANTERHAKE:                    return "ANTERHAKE";
        case SkillId::ORTKUNSKAP:                   return "ORTKUNSKAP";
        case SkillId::OVERLEVNAD:                   return "OVERLEVNAD";
        case SkillId::PLAYER_CHOICE:                return "PLAYER_CHOICE";
    }
    return "PLAYER_CHOICE";
}

/// Reverse lookup from Swedish display name to SkillId.
/// Returns SkillId::PLAYER_CHOICE for any unrecognised string.
inline SkillId skillIdFromString(const std::string& name) {
    if (name == "Bluffa")                          return SkillId::BLUFFA;
    if (name == "Finna dolda ting")                return SkillId::FINNA_DOLDA_TING;
    if (name == "Första hjälpen")                  return SkillId::FORSTA_HJALPEN;
    if (name == "Gömma sig")                       return SkillId::GOMMA_SIG;
    if (name == "Hoppa")                           return SkillId::HOPPA;
    if (name == "Klättra")                         return SkillId::KLATRA;
    if (name == "Köpslå")                          return SkillId::KOPSLA;
    if (name == "Lyssna")                          return SkillId::LYSSNA;
    if (name == "Läsa/Skriva modersmål")           return SkillId::LASA_SKRIVA_MODERSMAL;
    if (name == "Rida")                            return SkillId::RIDA;
    if (name == "Sjunga")                          return SkillId::SJUNGA;
    if (name == "Slagsmål")                        return SkillId::SLAGSMAL;
    if (name == "Smyga")                           return SkillId::SMYGA;
    if (name == "Spåra")                           return SkillId::SPARA;
    if (name == "Stjäla föremål")                  return SkillId::STJALA_FOREMAL;
    if (name == "Tala modersmål")                  return SkillId::TALA_MODERSMAL;
    if (name == "Upptäcka fara")                   return SkillId::UPPTACKA_FARA;
    if (name == "Värdera")                         return SkillId::VARDERA;
    if (name == "Övertala")                        return SkillId::OVERTALA;
    if (name == "Administration")                  return SkillId::ADMINISTRATION;
    if (name == "Akrobatik")                       return SkillId::AKROBATIK;
    if (name == "Alkemi")                          return SkillId::ALKEMI;
    if (name == "Animism")                         return SkillId::ANIMISM;
    if (name == "Astrologi")                       return SkillId::ASTROLOGI;
    if (name == "Avväpna")                         return SkillId::AVVAPNA;
    if (name == "Buktala")                         return SkillId::BUKTALA;
    if (name == "Bärsärkagång")                    return SkillId::BARSARKAGANGA;
    if (name == "Dans")                            return SkillId::DANS;
    if (name == "Djurhelning")                     return SkillId::DJURHELNING;
    if (name == "Djurträning")                     return SkillId::DJURTRANING;
    if (name == "Dolk")                            return SkillId::DOLK;
    if (name == "Dra vapen")                       return SkillId::DRA_VAPEN;
    if (name == "Drogkunskap")                     return SkillId::DROGKUNSKAP;
    if (name == "Elementarmagi")                   return SkillId::ELEMENTARMAGI;
    if (name == "Förfalskning")                    return SkillId::FORFALSKNING;
    if (name == "Geografi")                        return SkillId::GEOGRAFI;
    if (name == "Geologi")                         return SkillId::GEOLOGI;
    if (name == "Giftkunskap")                     return SkillId::GIFTKUNSKAP;
    if (name == "Gyckelkonster")                   return SkillId::GYCKELKONSTER;
    if (name == "Hantera fällor")                  return SkillId::HANTERA_FALLOR;
    if (name == "Hantverk")                        return SkillId::HANTVERK;
    if (name == "Hasardspel")                      return SkillId::HASARDSPEL;
    if (name == "Heraldik")                        return SkillId::HERALDIK;
    if (name == "Historia")                        return SkillId::HISTORIA;
    if (name == "Hypnotisera")                     return SkillId::HYPNOTISERA;
    if (name == "Knopar")                          return SkillId::KNOPAR;
    if (name == "Kulturkännedom")                  return SkillId::KULTURKANNEDOM;
    if (name == "Kunskap om demoner")              return SkillId::KUNSKAP_OM_DEMONER;
    if (name == "Kunskap om magi")                 return SkillId::KUNSKAP_OM_MAGI;
    if (name == "Kunskap om odöda")                return SkillId::KUNSKAP_OM_ODODA;
    if (name == "Känna magi")                      return SkillId::KANNA_MAGI;
    if (name == "Låsdyrkning")                     return SkillId::LASDYRKNING;
    if (name == "Läkekonst")                       return SkillId::LAKEKONST;
    if (name == "Läppläsning")                     return SkillId::LAPPLASNING;
    if (name == "Läsa/Skriva främmande språk")     return SkillId::LASA_SKRIVA_FRAMMANDE_SPRAK;
    if (name == "Magisk kanalisering")             return SkillId::MAGISK_KANALISERING;
    if (name == "Massage")                         return SkillId::MASSAGE;
    if (name == "Mentalism")                       return SkillId::MENTALISM;
    if (name == "Muta")                            return SkillId::MUTA;
    if (name == "Målning")                         return SkillId::MALNING;
    if (name == "Navigera")                        return SkillId::NAVIGERA;
    if (name == "Orientering")                     return SkillId::ORIENTERING;
    if (name == "Räkning")                         return SkillId::RAKNING;
    if (name == "Schack & brädspel")               return SkillId::SCHACK_BRADSPEL;
    if (name == "Simma")                           return SkillId::SIMMA;
    if (name == "Sjökunnighet")                    return SkillId::SJOKUNNIGHET;
    if (name == "Skådespeleri")                    return SkillId::SKADESPELERI;
    if (name == "Spela instrument")                return SkillId::SPELA_INSTRUMENT;
    if (name == "Språkkunskap")                    return SkillId::SPRAKKUNSKAP;
    if (name == "Spå väder")                       return SkillId::SPA_VADER;
    if (name == "Stavhopp")                        return SkillId::STAVHOPP;
    if (name == "Stridskonster")                   return SkillId::STRIDSKONSTER;
    if (name == "Tala främmande språk")            return SkillId::TALA_FRAMMANDE_SPRAK;
    if (name == "Teckenspråk")                     return SkillId::TECKENSPRAK;
    if (name == "Trästav")                         return SkillId::TRASTAV;
    if (name == "Två vapen")                       return SkillId::TVA_VAPEN;
    if (name == "Undre världen")                   return SkillId::UNDRE_VARLDEN;
    if (name == "Vapenfärdighet")                  return SkillId::VAPENFARDIGHET;
    if (name == "Zoologi")                         return SkillId::ZOOLOGI;
    if (name == "Änterhake")                       return SkillId::ANTERHAKE;
    if (name == "Örtkunskap")                      return SkillId::ORTKUNSKAP;
    if (name == "Överlevnad")                      return SkillId::OVERLEVNAD;
    return SkillId::PLAYER_CHOICE;
}

/// Reverse lookup from ASCII key to SkillId.
/// Used when deserialising JSON that contains a "skill_id" key.
/// Returns SkillId::PLAYER_CHOICE for any unrecognised key.
inline SkillId skillIdFromKey(const std::string& key) {
    if (key == "BLUFFA")                        return SkillId::BLUFFA;
    if (key == "FINNA_DOLDA_TING")              return SkillId::FINNA_DOLDA_TING;
    if (key == "FORSTA_HJALPEN")                return SkillId::FORSTA_HJALPEN;
    if (key == "GOMMA_SIG")                     return SkillId::GOMMA_SIG;
    if (key == "HOPPA")                         return SkillId::HOPPA;
    if (key == "KLATRA")                        return SkillId::KLATRA;
    if (key == "KOPSLA")                        return SkillId::KOPSLA;
    if (key == "LYSSNA")                        return SkillId::LYSSNA;
    if (key == "LASA_SKRIVA_MODERSMAL")         return SkillId::LASA_SKRIVA_MODERSMAL;
    if (key == "RIDA")                          return SkillId::RIDA;
    if (key == "SJUNGA")                        return SkillId::SJUNGA;
    if (key == "SLAGSMAL")                      return SkillId::SLAGSMAL;
    if (key == "SMYGA")                         return SkillId::SMYGA;
    if (key == "SPARA")                         return SkillId::SPARA;
    if (key == "STJALA_FOREMAL")                return SkillId::STJALA_FOREMAL;
    if (key == "TALA_MODERSMAL")                return SkillId::TALA_MODERSMAL;
    if (key == "UPPTACKA_FARA")                 return SkillId::UPPTACKA_FARA;
    if (key == "VARDERA")                       return SkillId::VARDERA;
    if (key == "OVERTALA")                      return SkillId::OVERTALA;
    if (key == "ADMINISTRATION")                return SkillId::ADMINISTRATION;
    if (key == "AKROBATIK")                     return SkillId::AKROBATIK;
    if (key == "ALKEMI")                        return SkillId::ALKEMI;
    if (key == "ANIMISM")                       return SkillId::ANIMISM;
    if (key == "ASTROLOGI")                     return SkillId::ASTROLOGI;
    if (key == "AVVAPNA")                       return SkillId::AVVAPNA;
    if (key == "BUKTALA")                       return SkillId::BUKTALA;
    if (key == "BARSARKAGANGA")                 return SkillId::BARSARKAGANGA;
    if (key == "DANS")                          return SkillId::DANS;
    if (key == "DJURHELNING")                   return SkillId::DJURHELNING;
    if (key == "DJURTRANING")                   return SkillId::DJURTRANING;
    if (key == "DOLK")                          return SkillId::DOLK;
    if (key == "DRA_VAPEN")                     return SkillId::DRA_VAPEN;
    if (key == "DROGKUNSKAP")                   return SkillId::DROGKUNSKAP;
    if (key == "ELEMENTARMAGI")                 return SkillId::ELEMENTARMAGI;
    if (key == "FORFALSKNING")                  return SkillId::FORFALSKNING;
    if (key == "GEOGRAFI")                      return SkillId::GEOGRAFI;
    if (key == "GEOLOGI")                       return SkillId::GEOLOGI;
    if (key == "GIFTKUNSKAP")                   return SkillId::GIFTKUNSKAP;
    if (key == "GYCKELKONSTER")                 return SkillId::GYCKELKONSTER;
    if (key == "HANTERA_FALLOR")                return SkillId::HANTERA_FALLOR;
    if (key == "HANTVERK")                      return SkillId::HANTVERK;
    if (key == "HASARDSPEL")                    return SkillId::HASARDSPEL;
    if (key == "HERALDIK")                      return SkillId::HERALDIK;
    if (key == "HISTORIA")                      return SkillId::HISTORIA;
    if (key == "HYPNOTISERA")                   return SkillId::HYPNOTISERA;
    if (key == "KNOPAR")                        return SkillId::KNOPAR;
    if (key == "KULTURKANNEDOM")                return SkillId::KULTURKANNEDOM;
    if (key == "KUNSKAP_OM_DEMONER")            return SkillId::KUNSKAP_OM_DEMONER;
    if (key == "KUNSKAP_OM_MAGI")               return SkillId::KUNSKAP_OM_MAGI;
    if (key == "KUNSKAP_OM_ODODA")              return SkillId::KUNSKAP_OM_ODODA;
    if (key == "KANNA_MAGI")                    return SkillId::KANNA_MAGI;
    if (key == "LASDYRKNING")                   return SkillId::LASDYRKNING;
    if (key == "LAKEKONST")                     return SkillId::LAKEKONST;
    if (key == "LAPPLASNING")                   return SkillId::LAPPLASNING;
    if (key == "LASA_SKRIVA_FRAMMANDE_SPRAK")   return SkillId::LASA_SKRIVA_FRAMMANDE_SPRAK;
    if (key == "MAGISK_KANALISERING")           return SkillId::MAGISK_KANALISERING;
    if (key == "MASSAGE")                       return SkillId::MASSAGE;
    if (key == "MENTALISM")                     return SkillId::MENTALISM;
    if (key == "MUTA")                          return SkillId::MUTA;
    if (key == "MALNING")                       return SkillId::MALNING;
    if (key == "NAVIGERA")                      return SkillId::NAVIGERA;
    if (key == "ORIENTERING")                   return SkillId::ORIENTERING;
    if (key == "RAKNING")                       return SkillId::RAKNING;
    if (key == "SCHACK_BRADSPEL")               return SkillId::SCHACK_BRADSPEL;
    if (key == "SIMMA")                         return SkillId::SIMMA;
    if (key == "SJOKUNNIGHET")                  return SkillId::SJOKUNNIGHET;
    if (key == "SKADESPELERI")                  return SkillId::SKADESPELERI;
    if (key == "SPELA_INSTRUMENT")              return SkillId::SPELA_INSTRUMENT;
    if (key == "SPRAKKUNSKAP")                  return SkillId::SPRAKKUNSKAP;
    if (key == "SPA_VADER")                     return SkillId::SPA_VADER;
    if (key == "STAVHOPP")                      return SkillId::STAVHOPP;
    if (key == "STRIDSKONSTER")                 return SkillId::STRIDSKONSTER;
    if (key == "TALA_FRAMMANDE_SPRAK")          return SkillId::TALA_FRAMMANDE_SPRAK;
    if (key == "TECKENSPRAK")                   return SkillId::TECKENSPRAK;
    if (key == "TRASTAV")                       return SkillId::TRASTAV;
    if (key == "TVA_VAPEN")                     return SkillId::TVA_VAPEN;
    if (key == "UNDRE_VARLDEN")                 return SkillId::UNDRE_VARLDEN;
    if (key == "VAPENFARDIGHET")                return SkillId::VAPENFARDIGHET;
    if (key == "ZOOLOGI")                       return SkillId::ZOOLOGI;
    if (key == "ANTERHAKE")                     return SkillId::ANTERHAKE;
    if (key == "ORTKUNSKAP")                    return SkillId::ORTKUNSKAP;
    if (key == "OVERLEVNAD")                    return SkillId::OVERLEVNAD;
    return SkillId::PLAYER_CHOICE;
}

} // namespace dod
