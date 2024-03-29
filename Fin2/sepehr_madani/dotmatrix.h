#ifndef DOTMATRIX_H
#define DOTMATRIX_H

/*,

### DOCUMENTATION ###

The given character array, named alphabet, is a unique representation of the
standard ASCII characters in a visual and more detailed form.

### STRUCTURE ###

`alphabet`, declared as `static char alphabet[95][211]` is an array of 95 arrays
of 211-long character arrays ("blocks"). Each letter in the ASCII form resides
in one of these blocks, starting from ASCII code 32 (' ', space) through 126
('~', tilde). Each block is 21 characters wide and 10 lines tall, for a total of
211 bytes, counting the null terminator.

For simplicity, each letter's representation starts at the rightmost column of a
block, and a column of '$' separates the leftmost section of the letter's block
from empty space. For example, since the letter `I` is only 9 pixels wide, the
representation of `i` in the array would look like this:

```
char letter_I[] =
" _  _  _ "
"(_)(_)(_)"
"   (_)   "
"   (_)   "
"   (_)   "
"   (_)   "
" _ (_) _ "
"(_)(_)(_)"
"         "
"         ";
```

Note that when accessing ascii characters in the array, the index is the ascii
code minus 32. For example, to access the letter `i`, the index would be 105
(ascii code of `i` minus 32).

*/

#define CHAR_WIDTH 21
#define CHAR_HEIGHT 10
#define CHAR_SPACE_SIZE (CHAR_WIDTH * CHAR_HEIGHT)
#define LINE_LIMIT 160

static char alphabet[95][211] = { "                   "
				  "                   "
				  "                   "
				  "                   "
				  "                   "
				  "                   "
				  "                   "
				  "                   "
				  "                   "
				  "                   ",

				  " _ "
				  "(_)"
				  "(_)"
				  "(_)"
				  "(_)"
				  "(_)"
				  "   "
				  " _ "
				  "(_)"
				  "   ",

				  "  _   _  "
				  " (_) (_) "
				  " (_) (_) "
				  " (_) (_) "
				  "         "
				  "         "
				  "         "
				  "         "
				  "         "
				  "         ",

				  "    _     _    "
				  "   (_)   (_)   "
				  " _ (_) _ (_) _ "
				  "(_)(_)(_)(_)(_)"
				  " _ (_) _ (_) _ "
				  "(_)(_)(_)(_)(_)"
				  "   (_)   (_)   "
				  "   (_)   (_)   "
				  "               "
				  "               ",

				  "      _         "
				  "   _ (_) _  _   "
				  " _(_)(_)(_)(_)  "
				  "(_)_ (_) _  _   "
				  "  (_)(_)(_)(_)_ "
				  "   _ (_) _  _(_)"
				  "  (_)(_)(_)(_)  "
				  "     (_)        "
				  "                "
				  "                ",

				  " _  _        _ "
				  "(_)(_)     _(_)"
				  "(_)(_)   _(_)  "
				  "       _(_)    "
				  "     _(_)      "
				  "   _(_)   _  _ "
				  " _(_)    (_)(_)"
				  "(_)      (_)(_)"
				  "               "
				  "               ",

				  "   _  _       "
				  " _(_)(_)_     "
				  "(_)_  _(_)    "
				  "  (_)(_)    _ "
				  " _ (_)_   _(_)"
				  "(_)  (_)_(_)  "
				  "(_)_  _(_)_   "
				  "  (_)(_) (_)  "
				  "              "
				  "              ",

				  "  _  _  "
				  " (_)(_) "
				  " (_)(_) "
				  "   (_)  "
				  "  (_)   "
				  "        "
				  "        "
				  "        "
				  "        "
				  "        ",

				  "      _ "
				  "   _ (_)"
				  " _(_)   "
				  "(_)     "
				  "(_)     "
				  "(_)_    "
				  "  (_) _ "
				  "     (_)"
				  "        "
				  "        ",

				  " _      "
				  "(_) _   "
				  "   (_)_ "
				  "     (_)"
				  "     (_)"
				  "    _(_)"
				  " _ (_)  "
				  "(_)     "
				  "        "
				  "        ",

				  "               "
				  "   _       _   "
				  "  (_)_   _(_)  "
				  " _  (_)_(_)  _ "
				  "(_)(_)(_)(_)(_)"
				  "   _(_) (_)_   "
				  "  (_)     (_)  "
				  "               "
				  "               "
				  "               ",

				  "               "
				  "       _       "
				  "      (_)      "
				  " _  _ (_) _  _ "
				  "(_)(_)(_)(_)(_)"
				  "      (_)      "
				  "      (_)      "
				  "               "
				  "               "
				  "               ",

				  "       "
				  "       "
				  "       "
				  "       "
				  "       "
				  " _  _  "
				  "(_)(_) "
				  "(_)(_) "
				  "  (_)  "
				  " (_)   ",

				  "               "
				  "               "
				  "               "
				  " _  _  _  _  _ "
				  "(_)(_)(_)(_)(_)"
				  "               "
				  "               "
				  "               "
				  "               "
				  "               ",

				  "       "
				  "       "
				  "       "
				  "       "
				  "       "
				  " _  _  "
				  "(_)(_) "
				  "(_)(_) "
				  "       "
				  "       ",

				  "               _   "
				  "             _(_)  "
				  "           _(_)    "
				  "         _(_)      "
				  "       _(_)        "
				  "     _(_)          "
				  "   _(_)            "
				  "  (_)              "
				  "                   "
				  "                   ",

				  "     _  _     "
				  "  _ (_)(_) _  "
				  " (_)      (_) "
				  "(_)        (_)"
				  "(_)        (_)"
				  "(_)        (_)"
				  " (_) _  _ (_) "
				  "    (_)(_)    "
				  "              "
				  "              ",

				  "    _    "
				  " _ (_)   "
				  "(_)(_)   "
				  "   (_)   "
				  "   (_)   "
				  "   (_)   "
				  " _ (_) _ "
				  "(_)(_)(_)"
				  "         "
				  "         ",

				  "    _  _  _    "
				  " _ (_)(_)(_) _ "
				  "(_)         (_)"
				  "          _ (_)"
				  "       _ (_)   "
				  "    _ (_)      "
				  " _ (_) _  _  _ "
				  "(_)(_)(_)(_)(_)"
				  "               "
				  "               ",

				  "   _  _  _  _   "
				  " _(_)(_)(_)(_)_ "
				  "(_)          (_)"
				  "         _  _(_)"
				  "        (_)(_)_ "
				  " _           (_)"
				  "(_)_  _  _  _(_)"
				  "  (_)(_)(_)(_)  "
				  "                "
				  "                ",

				  "          _    "
				  "       _ (_)   "
				  "    _ (_)(_)   "
				  " _ (_)   (_)   "
				  "(_) _  _ (_) _ "
				  "(_)(_)(_)(_)(_)"
				  "         (_)   "
				  "         (_)   "
				  "               "
				  "               ",

				  " _  _  _  _  _ "
				  "(_)(_)(_)(_)(_)"
				  "(_) _  _  _    "
				  "(_)(_)(_)(_) _ "
				  "            (_)"
				  " _          (_)"
				  "(_) _  _  _ (_)"
				  "   (_)(_)(_)   "
				  "               "
				  "               ",

				  "     _  _  _  "
				  "   _(_)(_)(_) "
				  " _(_)         "
				  "(_) _  _  _   "
				  "(_)(_)(_)(_)_ "
				  "(_)        (_)"
				  "(_)_  _  _ (_)"
				  "  (_)(_)(_)   "
				  "              "
				  "              ",

				  " _  _  _  _  _ "
				  "(_)(_)(_)(_)(_)"
				  "          _(_) "
				  "        _(_)   "
				  "      _(_)     "
				  "    _(_)       "
				  "  _(_)         "
				  " (_)           "
				  "               "
				  "               ",

				  "   _  _  _  _   "
				  " _(_)(_)(_)(_)_ "
				  "(_)          (_)"
				  "(_)_  _  _  _(_)"
				  " _(_)(_)(_)(_)_ "
				  "(_)          (_)"
				  "(_)_  _  _  _(_)"
				  "  (_)(_)(_)(_)  "
				  "                "
				  "                ",

				  "    _  _  _    "
				  " _ (_)(_)(_) _ "
				  "(_)         (_)"
				  "(_) _  _  _ (_)"
				  "   (_)(_)(_)(_)"
				  "           _(_)"
				  "   _  _  _(_)  "
				  "  (_)(_)(_)    "
				  "               "
				  "               ",

				  "       "
				  "       "
				  " _  _  "
				  "(_)(_) "
				  "(_)(_) "
				  " _  _  "
				  "(_)(_) "
				  "(_)(_) "
				  "       "
				  "       ",

				  "       "
				  "       "
				  " _  _  "
				  "(_)(_) "
				  "(_)(_) "
				  " _  _  "
				  "(_)(_) "
				  "(_)(_) "
				  "  (_)  "
				  " (_)   ",

				  "          _ "
				  "       _ (_)"
				  "    _ (_)   "
				  " _ (_)      "
				  "(_) _       "
				  "   (_) _    "
				  "      (_) _ "
				  "         (_)"
				  "            "
				  "            ",

				  "               "
				  "               "
				  " _  _  _  _  _ "
				  "(_)(_)(_)(_)(_)"
				  " _  _  _  _  _ "
				  "(_)(_)(_)(_)(_)"
				  "               "
				  "               "
				  "               "
				  "               ",

				  " _          "
				  "(_) _       "
				  "   (_) _    "
				  "      (_) _ "
				  "       _ (_)"
				  "    _ (_)   "
				  " _ (_)      "
				  "(_)         "
				  "            "
				  "            ",

				  "    _  _  _   "
				  " _ (_)(_)(_)_ "
				  "(_)        (_)"
				  "         _ (_)"
				  "      _ (_)   "
				  "     (_)      "
				  "      _       "
				  "     (_)      "
				  "              "
				  "              ",

				  "    _  _  _   "
				  "  _(_)(_)(_)_ "
				  " (_)  _  _ (_)"
				  "(_)  (_)(_)(_)"
				  "(_) (_)  _ (_)"
				  "(_)  (_)(_)(_)"
				  " (_)  _  _  _ "
				  "  (_)(_)(_)(_)"
				  "              "
				  "              ",

				  "       _       "
				  "     _(_)_     "
				  "   _(_) (_)_   "
				  " _(_)     (_)_ "
				  "(_) _  _  _ (_)"
				  "(_)(_)(_)(_)(_)"
				  "(_)         (_)"
				  "(_)         (_)"
				  "               "
				  "               ",

				  " _  _  _  _    "
				  "(_)(_)(_)(_) _ "
				  " (_)        (_)"
				  " (_) _  _  _(_)"
				  " (_)(_)(_)(_)_ "
				  " (_)        (_)"
				  " (_)_  _  _ (_)"
				  "(_)(_)(_)(_)   "
				  "               "
				  "               ",

				  "    _  _  _    "
				  " _ (_)(_)(_) _ "
				  "(_)         (_)"
				  "(_)            "
				  "(_)            "
				  "(_)          _ "
				  "(_) _  _  _ (_)"
				  "   (_)(_)(_)   "
				  "               "
				  "               ",

				  " _  _  _  _    "
				  "(_)(_)(_)(_)   "
				  " (_)      (_)_ "
				  " (_)        (_)"
				  " (_)        (_)"
				  " (_)       _(_)"
				  " (_)_  _  (_)  "
				  "(_)(_)(_)(_)   "
				  "               "
				  "               ",

				  " _  _  _  _  _ "
				  "(_)(_)(_)(_)(_)"
				  "(_)            "
				  "(_) _  _       "
				  "(_)(_)(_)      "
				  "(_)            "
				  "(_) _  _  _  _ "
				  "(_)(_)(_)(_)(_)"
				  "               "
				  "               ",

				  " _  _  _  _  _ "
				  "(_)(_)(_)(_)(_)"
				  "(_)            "
				  "(_) _  _       "
				  "(_)(_)(_)      "
				  "(_)            "
				  "(_)            "
				  "(_)            "
				  "               "
				  "               ",

				  "    _  _  _    "
				  " _ (_)(_)(_) _ "
				  "(_)         (_)"
				  "(_)    _  _  _ "
				  "(_)   (_)(_)(_)"
				  "(_)         (_)"
				  "(_) _  _  _ (_)"
				  "   (_)(_)(_)(_)"
				  "               "
				  "               ",

				  " _           _ "
				  "(_)         (_)"
				  "(_)         (_)"
				  "(_) _  _  _ (_)"
				  "(_)(_)(_)(_)(_)"
				  "(_)         (_)"
				  "(_)         (_)"
				  "(_)         (_)"
				  "               "
				  "               ",

				  " _  _  _ "
				  "(_)(_)(_)"
				  "   (_)   "
				  "   (_)   "
				  "   (_)   "
				  "   (_)   "
				  " _ (_) _ "
				  "(_)(_)(_)"
				  "         "
				  "         ",

				  "      _  _  _ "
				  "     (_)(_)(_)"
				  "        (_)   "
				  "        (_)   "
				  "        (_)   "
				  " _      (_)   "
				  "(_)  _  (_)   "
				  " (_)(_)(_)    "
				  "              "
				  "              ",

				  " _           _ "
				  "(_)       _ (_)"
				  "(_)    _ (_)   "
				  "(_) _ (_)      "
				  "(_)(_) _       "
				  "(_)   (_) _    "
				  "(_)      (_) _ "
				  "(_)         (_)"
				  "               "
				  "               ",

				  " _             "
				  "(_)            "
				  "(_)            "
				  "(_)            "
				  "(_)            "
				  "(_)            "
				  "(_) _  _  _  _ "
				  "(_)(_)(_)(_)(_)"
				  "               "
				  "               ",

				  " _           _ "
				  "(_) _     _ (_)"
				  "(_)(_)   (_)(_)"
				  "(_) (_)_(_) (_)"
				  "(_)   (_)   (_)"
				  "(_)         (_)"
				  "(_)         (_)"
				  "(_)         (_)"
				  "               "
				  "               ",

				  " _           _ "
				  "(_) _       (_)"
				  "(_)(_)_     (_)"
				  "(_)  (_)_   (_)"
				  "(_)    (_)_ (_)"
				  "(_)      (_)(_)"
				  "(_)         (_)"
				  "(_)         (_)"
				  "               "
				  "               ",

				  "   _  _  _  _   "
				  " _(_)(_)(_)(_)_ "
				  "(_)          (_)"
				  "(_)          (_)"
				  "(_)          (_)"
				  "(_)          (_)"
				  "(_)_  _  _  _(_)"
				  "  (_)(_)(_)(_)  "
				  "                "
				  "                ",

				  " _  _  _  _   "
				  "(_)(_)(_)(_)_ "
				  "(_)        (_)"
				  "(_) _  _  _(_)"
				  "(_)(_)(_)(_)  "
				  "(_)           "
				  "(_)           "
				  "(_)           "
				  "              "
				  "              ",

				  "   _  _  _  _   "
				  " _(_)(_)(_)(_)_ "
				  "(_)          (_)"
				  "(_)          (_)"
				  "(_)     _    (_)"
				  "(_)    (_) _ (_)"
				  "(_)_  _  _(_) _ "
				  "  (_)(_)(_)  (_)"
				  "                "
				  "                ",

				  " _  _  _  _    "
				  "(_)(_)(_)(_) _ "
				  "(_)         (_)"
				  "(_) _  _  _ (_)"
				  "(_)(_)(_)(_)   "
				  "(_)   (_) _    "
				  "(_)      (_) _ "
				  "(_)         (_)"
				  "               "
				  "               ",

				  "   _  _  _  _   "
				  " _(_)(_)(_)(_)_ "
				  "(_)          (_)"
				  "(_)_  _  _  _   "
				  "  (_)(_)(_)(_)_ "
				  " _           (_)"
				  "(_)_  _  _  _(_)"
				  "  (_)(_)(_)(_)  "
				  "                "
				  "                ",

				  " _  _  _  _  _ "
				  "(_)(_)(_)(_)(_)"
				  "      (_)      "
				  "      (_)      "
				  "      (_)      "
				  "      (_)      "
				  "      (_)      "
				  "      (_)      "
				  "               "
				  "               ",

				  " _            _ "
				  "(_)          (_)"
				  "(_)          (_)"
				  "(_)          (_)"
				  "(_)          (_)"
				  "(_)          (_)"
				  "(_)_  _  _  _(_)"
				  "  (_)(_)(_)(_)  "
				  "                "
				  "                ",

				  " _           _ "
				  "(_)         (_)"
				  "(_)         (_)"
				  "(_)_       _(_)"
				  "  (_)     (_)  "
				  "   (_)   (_)   "
				  "    (_)_(_)    "
				  "      (_)      "
				  "               "
				  "               ",

				  " _             _ "
				  "(_)           (_)"
				  "(_)           (_)"
				  "(_)     _     (_)"
				  "(_)   _(_)_   (_)"
				  "(_)  (_) (_)  (_)"
				  "(_)_(_)   (_)_(_)"
				  "  (_)       (_)  "
				  "                 "
				  "                 ",

				  " _           _ "
				  "(_)_       _(_)"
				  "  (_)_   _(_)  "
				  "    (_)_(_)    "
				  "     _(_)_     "
				  "   _(_) (_)_   "
				  " _(_)     (_)_ "
				  "(_)         (_)"
				  "               "
				  "               ",

				  " _           _ "
				  "(_)_       _(_)"
				  "  (_)_   _(_)  "
				  "    (_)_(_)    "
				  "      (_)      "
				  "      (_)      "
				  "      (_)      "
				  "      (_)      "
				  "               "
				  "               ",

				  " _  _  _  _  _ "
				  "(_)(_)(_)(_)(_)"
				  "          _(_) "
				  "        _(_)   "
				  "      _(_)     "
				  "    _(_)       "
				  " _ (_) _  _  _ "
				  "(_)(_)(_)(_)(_)"
				  "               "
				  "               ",

				  " _  _  _ "
				  "(_)(_)(_)"
				  "(_)      "
				  "(_)      "
				  "(_)      "
				  "(_)      "
				  "(_) _  _ "
				  "(_)(_)(_)"
				  "         "
				  "         ",

				  "   _               "
				  "  (_)_             "
				  "    (_)_           "
				  "      (_)_         "
				  "        (_)_       "
				  "          (_)_     "
				  "            (_)_   "
				  "              (_)  "
				  "                   "
				  "                   ",

				  " _  _  _ "
				  "(_)(_)(_)"
				  "      (_)"
				  "      (_)"
				  "      (_)"
				  "      (_)"
				  " _  _ (_)"
				  "(_)(_)(_)"
				  "         "
				  "         ",

				  "       _       "
				  "    _ (_) _    "
				  " _ (_)   (_) _ "
				  "(_)         (_)"
				  "               "
				  "               "
				  "               "
				  "               "
				  "               "
				  "               ",

				  "                   "
				  "                   "
				  "                   "
				  "                   "
				  "                   "
				  "                   "
				  "                   "
				  "                   "
				  "  _  _  _  _  _  _ "
				  " (_)(_)(_)(_)(_)(_)",

				  "  _  _  "
				  " (_)(_) "
				  " (_)(_) "
				  "  (_)   "
				  "   (_)  "
				  "        "
				  "        "
				  "        "
				  "        "
				  "        ",

				  "                "
				  "                "
				  "   _  _  _      "
				  "  (_)(_)(_) _   "
				  "   _  _  _ (_)  "
				  " _(_)(_)(_)(_)  "
				  "(_)_  _  _ (_)_ "
				  "  (_)(_)(_)  (_)"
				  "                "
				  "                ",

				  " _            "
				  "(_)           "
				  "(_) _  _  _   "
				  "(_)(_)(_)(_)_ "
				  "(_)        (_)"
				  "(_)        (_)"
				  "(_) _  _  _(_)"
				  "(_)(_)(_)(_)  "
				  "              "
				  "              ",

				  "           "
				  "           "
				  "   _  _  _ "
				  " _(_)(_)(_)"
				  "(_)        "
				  "(_)        "
				  "(_)_  _  _ "
				  "  (_)(_)(_)"
				  "           "
				  "           ",

				  "            _ "
				  "           (_)"
				  "   _  _  _ (_)"
				  " _(_)(_)(_)(_)"
				  "(_)        (_)"
				  "(_)        (_)"
				  "(_)_  _  _ (_)"
				  "  (_)(_)(_)(_)"
				  "              "
				  "              ",

				  "               "
				  "               "
				  "  _  _  _  _   "
				  " (_)(_)(_)(_)_ "
				  "(_) _  _  _ (_)"
				  "(_)(_)(_)(_)(_)"
				  "(_)_  _  _  _  "
				  "  (_)(_)(_)(_) "
				  "               "
				  "               ",

				  "      _  _ "
				  "    _(_)(_)"
				  " _ (_) _   "
				  "(_)(_)(_)  "
				  "   (_)     "
				  "   (_)     "
				  "   (_)     "
				  "   (_)     "
				  "           "
				  "           ",

				  "              "
				  "              "
				  "   _  _  _  _ "
				  " _(_)(_)(_)(_)"
				  "(_)        (_)"
				  "(_)        (_)"
				  "(_)_  _  _ (_)"
				  "  (_)(_)(_)(_)"
				  "   _  _  _ (_)"
				  "  (_)(_)(_)   ",

				  " _            "
				  "(_)           "
				  "(_) _  _  _   "
				  "(_)(_)(_)(_)_ "
				  "(_)        (_)"
				  "(_)        (_)"
				  "(_)        (_)"
				  "(_)        (_)"
				  "              "
				  "              ",

				  "    _    "
				  "   (_)   "
				  " _  _    "
				  "(_)(_)   "
				  "   (_)   "
				  "   (_)   "
				  " _ (_) _ "
				  "(_)(_)(_)"
				  "         "
				  "         ",

				  "          _ "
				  "         (_)"
				  "       _  _ "
				  "      (_)(_)"
				  "         (_)"
				  "         (_)"
				  "         (_)"
				  " _      _(_)"
				  "(_)_  _(_)  "
				  "  (_)(_)    ",

				  " _        "
				  "(_)       "
				  "(_)     _ "
				  "(_)   _(_)"
				  "(_) _(_)  "
				  "(_)(_)_   "
				  "(_)  (_)_ "
				  "(_)    (_)"
				  "          "
				  "          ",

				  " _  _    "
				  "(_)(_)   "
				  "   (_)   "
				  "   (_)   "
				  "   (_)   "
				  "   (_)   "
				  " _ (_) _ "
				  "(_)(_)(_)"
				  "         "
				  "         ",

				  "               "
				  "               "
				  "  _  _   _  _  "
				  " (_)(_)_(_)(_) "
				  "(_)   (_)   (_)"
				  "(_)   (_)   (_)"
				  "(_)   (_)   (_)"
				  "(_)   (_)   (_)"
				  "               "
				  "               ",

				  "              "
				  "              "
				  " _  _  _  _   "
				  "(_)(_)(_)(_)_ "
				  "(_)        (_)"
				  "(_)        (_)"
				  "(_)        (_)"
				  "(_)        (_)"
				  "              "
				  "              ",

				  "               "
				  "               "
				  "    _  _  _    "
				  " _ (_)(_)(_) _ "
				  "(_)         (_)"
				  "(_)         (_)"
				  "(_) _  _  _ (_)"
				  "   (_)(_)(_)   "
				  "               "
				  "               ",

				  "              "
				  "              "
				  " _  _  _  _   "
				  "(_)(_)(_)(_)_ "
				  "(_)        (_)"
				  "(_)        (_)"
				  "(_) _  _  _(_)"
				  "(_)(_)(_)(_)  "
				  "(_)           "
				  "(_)           ",

				  "              "
				  "              "
				  "   _  _  _  _ "
				  " _(_)(_)(_)(_)"
				  "(_)        (_)"
				  "(_)        (_)"
				  "(_)_  _  _ (_)"
				  "  (_)(_)(_)(_)"
				  "           (_)"
				  "           (_)",

				  "              "
				  "              "
				  " _       _  _ "
				  "(_)_  _ (_)(_)"
				  "  (_)(_)      "
				  "  (_)         "
				  "  (_)         "
				  "  (_)         "
				  "              "
				  "              ",

				  "                "
				  "                "
				  "   _  _  _  _   "
				  " _(_)(_)(_)(_)  "
				  "(_)_  _  _  _   "
				  "  (_)(_)(_)(_)_ "
				  "   _  _  _  _(_)"
				  "  (_)(_)(_)(_)  "
				  "                "
				  "                ",

				  "    _        "
				  "   (_)       "
				  " _ (_) _  _  "
				  "(_)(_)(_)(_) "
				  "   (_)       "
				  "   (_)     _ "
				  "   (_)_  _(_)"
				  "     (_)(_)  "
				  "             "
				  "             ",

				  "               "
				  "               "
				  " _         _   "
				  "(_)       (_)  "
				  "(_)       (_)  "
				  "(_)       (_)  "
				  "(_)_  _  _(_)_ "
				  "  (_)(_)(_) (_)"
				  "               "
				  "               ",

				  "                   "
				  "                   "
				  " _               _ "
				  "(_)_           _(_)"
				  "  (_)_       _(_)  "
				  "    (_)_   _(_)    "
				  "      (_)_(_)      "
				  "        (_)        "
				  "                   "
				  "                   ",

				  "                 "
				  "                 "
				  " _             _ "
				  "(_)           (_)"
				  "(_)     _     (_)"
				  "(_)_  _(_)_  _(_)"
				  "  (_)(_) (_)(_)  "
				  "    (_)   (_)    "
				  "                 "
				  "                 ",

				  "             "
				  "             "
				  " _         _ "
				  "(_) _   _ (_)"
				  "   (_)_(_)   "
				  "    _(_)_    "
				  " _ (_) (_) _ "
				  "(_)       (_)"
				  "             "
				  "             ",

				  "                   "
				  "                   "
				  " _               _ "
				  "(_)_           _(_)"
				  "  (_)_       _(_)  "
				  "    (_)_   _(_)    "
				  "      (_)_(_)      "
				  "       _(_)        "
				  "  _  _(_)          "
				  " (_)(_)            ",

				  "            "
				  "            "
				  " _  _  _  _ "
				  "(_)(_)(_)(_)"
				  "      _ (_) "
				  "   _ (_)    "
				  " _(_)  _  _ "
				  "(_)(_)(_)(_)"
				  "            "
				  "            ",

				  "      _  _ "
				  "    _(_)(_)"
				  "   (_)     "
				  " _ (_)     "
				  "(_) _      "
				  "   (_)     "
				  "   (_)_  _ "
				  "     (_)(_)"
				  "           "
				  "           ",

				  "  _  "
				  " (_) "
				  " (_) "
				  " (_) "
				  "  _  "
				  " (_) "
				  " (_) "
				  " (_) "
				  "     "
				  "     ",

				  " _  _      "
				  "(_)(_)_    "
				  "     (_)   "
				  "     (_) _ "
				  "      _ (_)"
				  "     (_)   "
				  " _  _(_)   "
				  "(_)(_)     "
				  "           "
				  "           ",

				  "               "
				  "   _  _      _ "
				  " _(_)(_)_  _(_)"
				  "(_)    (_)(_)  "
				  "               "
				  "               "
				  "               "
				  "               "
				  "               "
				  "               " };

#endif // DOTMATRIX_H
