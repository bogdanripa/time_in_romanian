#include "num2words.h"
#include "string.h"

static const char* const ONES[] = {
  "zero",
  "unu",
  "două",
  "trei",
  "patru",
  "cinci",
  "sase",
  "sapte",
  "opt",
  "nouă"
};

static const char* const TEENS[] ={
  "",
  "unspe",
  "doispe",
  "treispe",
  "paispe",
  "cinspe",
  "saispe",
  "saptespe",
  "optaspe",
  "nouăspe"
};

static const char* const TENS[] = {
  "",
  "zece",
  "douăzeci",
  "treizeci",
  "partuzeci",
  "cincizeci",
  "saizeci",
  "saptezeci",
  "optzeci",
  "nouăzeci"
};

static const char* STR_OH_CLOCK = "fix";
static const char* STR_NOON = "doispe";
static const char* STR_MIDNIGHT = "doispe";
static const char* STR_QUARTER = "un sfert";
static const char* STR_TO = "fără";
static const char* STR_PAST = "si";
static const char* STR_HALF = "jumătate";
static const char* STR_AFTER = "si";

static size_t append_number(char* words, int num) {
  int tens_val = num / 10 % 10;
  int ones_val = num % 10;

  size_t len = 0;

  if (tens_val > 0) {
    if (tens_val == 1 && num != 10) {
      strcat(words, TEENS[ones_val]);
      return strlen(TEENS[ones_val]);
    }
    strcat(words, TENS[tens_val]);
    len += strlen(TENS[tens_val]);
    if (ones_val > 0) {
      strcat(words, " ");
      len += 1;
    }
  }

  if (ones_val > 0 || num == 0) {
    if (tens_val > 0) {
      strcat(words, STR_PAST);
      len += strlen(STR_PAST);
      strcat(words, " ");
      len += 1;
    }
    strcat(words, ONES[ones_val]);
    len += strlen(ONES[ones_val]);
  }
  return len;
}

static size_t append_string(char* buffer, const size_t length, const char* str) {
  strncat(buffer, str, length);

  size_t written = strlen(str);
  return (length > written) ? written : length;
}

void fuzzy_time_to_words(int hours, int minutes, char* words, size_t length) {

  int fuzzy_hours = hours;
  int fuzzy_minutes = ((minutes + 2) / 5) * 5;

  // Handle hour & minute roll-over.
  if (fuzzy_minutes > 55) {
    fuzzy_minutes = 0;
    fuzzy_hours += 1;
    if (fuzzy_hours > 23) {
      fuzzy_hours = 0;
    }
  }

  size_t remaining = length;
  memset(words, 0, length);

  if (fuzzy_minutes != 0 && (fuzzy_minutes >= 10 || fuzzy_minutes == 5 || fuzzy_hours == 0 || fuzzy_hours == 12)) {
    if (fuzzy_minutes == 15) {
    } else if (fuzzy_minutes == 45) {
      fuzzy_hours = (fuzzy_hours + 1) % 24;
    } else if (fuzzy_minutes == 30) {
    } else if (fuzzy_minutes < 30) {
    } else {
      fuzzy_hours = (fuzzy_hours + 1) % 24;
    }
  }

  if (fuzzy_hours == 0) {
    remaining -= append_string(words, remaining, STR_MIDNIGHT);
  } else if (fuzzy_hours == 12) {
    remaining -= append_string(words, remaining, STR_NOON);
  } else {
    remaining -= append_number(words, fuzzy_hours % 12);
  }

  if (fuzzy_minutes != 0 && (fuzzy_minutes >= 10 || fuzzy_minutes == 5 || fuzzy_hours == 0 || fuzzy_hours == 12)) {
    if (fuzzy_minutes == 15) {
	// si un sfert
      remaining -= append_string(words, remaining, " ");
      remaining -= append_string(words, remaining, STR_AFTER);
      remaining -= append_string(words, remaining, " ");
      remaining -= append_string(words, remaining, STR_QUARTER);
    } else if (fuzzy_minutes == 45) {
	// fara un sfert
      remaining -= append_string(words, remaining, " ");
      remaining -= append_string(words, remaining, STR_TO);
      remaining -= append_string(words, remaining, " ");
      remaining -= append_string(words, remaining, STR_QUARTER);
    } else if (fuzzy_minutes == 30) {
	// si jumatate
      remaining -= append_string(words, remaining, " ");
      remaining -= append_string(words, remaining, STR_PAST);
      remaining -= append_string(words, remaining, " ");
      remaining -= append_string(words, remaining, STR_HALF);
    } else if (fuzzy_minutes < 30) {
	// si zece, doua zeci, doua zeci si cinci
      remaining -= append_string(words, remaining, " ");
      remaining -= append_string(words, remaining, STR_AFTER);
      remaining -= append_string(words, remaining, " ");
      remaining -= append_number(words, fuzzy_minutes);
    } else {
	// si zece, doua zeci, doua zeci si cinci
      remaining -= append_string(words, remaining, " ");
      remaining -= append_string(words, remaining, STR_TO);
      remaining -= append_string(words, remaining, " ");
      remaining -= append_number(words, 60 - fuzzy_minutes);
    }
  }

  if (fuzzy_minutes == 0) {
    remaining -= append_string(words, remaining, " ");
    remaining -= append_string(words, remaining, STR_OH_CLOCK);
  }
}
