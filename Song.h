#ifndef WET2_SONG_H
#define WET2_SONG_H

#include <memory>
#include "Genre.h"

class Song {
private:
    int id;
    int index;

public:
    Song(int sid, int ndx) : id(sid), index(ndx) {}

    int getId() {
        return id;
    }

    int getIndex() {
        return index;
    }

};

#endif // WET2_SONG_H
