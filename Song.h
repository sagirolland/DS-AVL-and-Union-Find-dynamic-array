#ifndef WET2_SONG_H
#define WET2_SONG_H

#include <memory>
#include "Genre.h"

class Song {
private:
    int id;
    int index;
    int base_merges; // NEW: stores the genre's merge count at addition

public:
    Song(int sid, int ndx) : id(sid), index(ndx), base_merges(0) {}

    int getId() { return id; }
    int getIndex() { return index; }
    void setBaseMerges(int bm) { base_merges = bm; }
    int getBaseMerges() const { return base_merges; }
};

#endif // WET2_SONG_H
