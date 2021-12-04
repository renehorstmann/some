#ifndef HIGHSCORESERVER_HIGHSCORE_H
#define HIGHSCORESERVER_HIGHSCORE_H

#include <stdint.h>
#include <stdbool.h>
#include "rhc/types.h"

#define HIGHSCORE_NAME_MAX_LENGTH 16
#define HIGHSCORE_TOPIC_MAX_LENGTH 64
#define HIGHSCORE_ADDRESS_MAX_LENGTH 128
#define HIGHSCORE_MAX_ENTRY_LENGTH 128

typedef struct {
    char name[HIGHSCORE_NAME_MAX_LENGTH];   // null terminated
    int score;
} HighscoreEntry_s;

typedef struct {
    char topic[HIGHSCORE_TOPIC_MAX_LENGTH];   // null terminated
    char address[HIGHSCORE_ADDRESS_MAX_LENGTH];   // null terminated
    uint16_t port;
    HighscoreEntry_s *entries;
    int entries_size;
} Highscore;


//
// function prototype!
// this function implementation should not be made public available
// return the checksum for the given entry
//
uint64_t highscore_entry_get_checksum(HighscoreEntry_s self);


//
// public highscore stuff:
//

Highscore highscore_new_msg(Str_s highscore_msg);

void highscore_kill(Highscore *self);

// create a new entry by name and score
HighscoreEntry_s highscore_entry_new(const char *name, int score);

String highscore_entry_to_string(HighscoreEntry_s self);

// buffer should be at least HIGHSCORE_MAX_ENTRY_LENGTH big
// returns buffer with the new size
Str_s highscore_entry_into_buffer(HighscoreEntry_s self, Str_s buffer);


#endif //HIGHSCORESERVER_HIGHSCORE_H
