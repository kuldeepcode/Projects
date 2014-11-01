#include<stdio.h>
#include<string.h>

#define dictionary2_longest 15
#define dictionary2_shortest 4
#define dictionary2_length 200

const int D2_ALPHA_INDEX[] = {
    0, //a
    16, //b
    21, //c
    38, //d
    47, //e
    61, //f
    74, //g
    77, //h
    83, //i
    -1, //j
    -1, //k
    91, //l
    99, //m
    114, //n
    120, //o
    127, //p
    151, //q
    152, //r
    159, //s
    176, //t
    185, //u
    189, //v
    195, //w
    -1, //x
    -1, //y
    199, //z
};

const int D2_ALPHA_INDEX_END[] = {
    16, //a
    21, //b
    38, //c
    47, //d
    61, //e
    74, //f
    77, //g
    83, //h
    91, //i
    -1, //j
    -1, //k
    99, //l
    114, //m
    120, //n
    127, //o
    151, //p
    152, //q
    159, //r
    176, //s
    185, //t
    189, //u
    195, //v
    199, //w
    -1, //x
    -1, //y
    200, //z
};

char DICTIONARY2[dictionary2_length][dictionary2_longest + 1] = {
    "abdomen",
    "accolade",
    "acuity",
    "advocate",
    "aesthetic",
    "allure",
    "aloof",
    "amorphous",
    "analogue",
    "anecdote",
    "anticipation",
    "antipathy",
    "apathetic",
    "appointment",
    "approval",
    "attenuate",
    "blackout",
    "bother",
    "bucket",
    "bureaucracy",
    "butter",
    "calibrate",
    "cardinal",
    "caustic",
    "cerebral",
    "chronic",
    "circumvent",
    "class",
    "cliche",
    "coding",
    "collage",
    "computer",
    "congruent",
    "conversation",
    "convolution",
    "cringe",
    "cryptography",
    "cupid",
    "decoy",
    "default",
    "devote",
    "discard",
    "dispatch",
    "disruption",
    "divert",
    "doodle",
    "dynamic",
    "elaborate",
    "eloquent",
    "emerge",
    "endorse",
    "engineering",
    "enhance",
    "enigma",
    "enormous",
    "ensure",
    "epigram",
    "evidence",
    "examination",
    "exorcism",
    "extrinsic",
    "fabricated",
    "facination",
    "farewell",
    "fender",
    "flatter",
    "follow",
    "forehead",
    "forever",
    "fragment",
    "freedom",
    "freedom",
    "furry",
    "futile",
    "goalpost",
    "grail",
    "gravity",
    "haircut",
    "hallow",
    "heterodox",
    "hinder",
    "hounds",
    "hyperbole",
    "important",
    "incredible",
    "incredulously",
    "induction",
    "information",
    "innovate",
    "intellect",
    "interpretation",
    "label",
    "language",
    "latent",
    "lavish",
    "legal",
    "legend",
    "lethal",
    "lucid",
    "machinist",
    "magical",
    "malleable",
    "master",
    "matter",
    "meaning",
    "medicine",
    "mellow",
    "microphone",
    "microscope",
    "minion",
    "modern",
    "modest",
    "moment",
    "monologue",
    "negate",
    "negligence",
    "network",
    "nonstick",
    "nostalgia",
    "novel",
    "obligate",
    "oblique",
    "obsession",
    "obsolete",
    "opaque",
    "operate",
    "opportunity",
    "palpable",
    "palpate",
    "paragon",
    "parody",
    "party",
    "pertain",
    "phenomenon",
    "picture",
    "pluck",
    "poised",
    "poker",
    "portable",
    "potent",
    "pottery",
    "precipitous",
    "preprocessing",
    "privacy",
    "profound",
    "program",
    "prohibitive",
    "protocol",
    "proximate",
    "psychosis",
    "purchase",
    "quote",
    "reconnaissance",
    "remedy",
    "reprehensible",
    "resistance",
    "resonant",
    "restore",
    "robust",
    "sanity",
    "score",
    "security",
    "seminal",
    "skeptical",
    "slake",
    "smoke",
    "software",
    "stipple",
    "stomach",
    "stream",
    "surface",
    "suspect",
    "swirl",
    "sword",
    "symphony",
    "system",
    "tangent",
    "temper",
    "tentative",
    "therapeutic",
    "thermal",
    "thought",
    "torrid",
    "trivial",
    "tuberoid",
    "ultra",
    "undercutting",
    "undermined",
    "underscore",
    "verbose",
    "vertigo",
    "visual",
    "vital",
    "volatile",
    "voltage",
    "waffle",
    "weather",
    "withdraw",
    "worship",
    "zenith",
    };
