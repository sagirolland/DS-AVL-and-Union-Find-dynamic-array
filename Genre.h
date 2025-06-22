#ifndef WET2_GENRE_H
#define WET2_GENRE_H

#include <memory>

class Genre {
private:
    int id;
    int songs_count;
    int index;

public:
    Genre(int gid) : id(gid), songs_count(0), index(-1) {}

    int getId() {
        return id;
    }

    int getSongsCount() {
        return songs_count;
    }

    int getIndex() {
        return index;
    }

    void setIndex(int ndx) {
        index = ndx;
    }

    void addSongs(int delta) {
        songs_count += delta; 
    }

    void removeAllSongs() {
        songs_count = 0; 
    }

};

#endif // WET2_GENRE_H