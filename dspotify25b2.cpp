// You can edit anything you want in this file.
// However you need to implement all public DSpotify function, as provided below as a template

#include "dspotify25b2.h"
//אין מחיקה של שירים או ז'אנרים, רק הוספה ומיזוג. מתאים ליוניון פיינד 
// מערך האיברים יהיה מערך דינמי של שירים 
// מערך הקבוצות יהיה מערך דינמי של ז'אנרים
// כל שיר יהיה איבר במערך הדינמי של השירים, ויכיל את המזהה שלו ואת המזהה של מספר הז'אנרים שהוא החליף עד כה
// הוספת ז'אנר תיצור איבר חדש במערך הדינמי של הז'אנרים, 
// הוספת שיר תיצור איבר חדש במערך הדינמי של השירים, ותוסיף אותו לז'אנר המתאים
// כלומר ניצור שיר חדש יש לו קבוצה ריקה עם סמן מוכר לקבוצת סינגלטון נחפש את הז'אנר המתאים לו,במערך של הז'אנרים,
// אם נמצא את הז'אנר נבצע איחוד יוניון ביניהם לפי גודל
// איחוד ז'אנרים יבדוק אם הז'אנרים כבר מאוחדים, אם לא אז ניצור ז'אנר חדש במערך הדינמי של הז'אנרים,
// ונאחד את שני הז'אנרים הקיימים לתוך הז'אנר החדש.
// getSongGenre יחפש את השיר במערך הדינמי של השירים, ואם הוא קיים יבצע find על הז'אנר שלו ויחזיר את המזהה של הז'אנר.
// getNumberOfSongsByGenre יחפש את הז'אנר במערך הדינמי של הז'אנרים, ואם הוא קיים יחזיר את מספר השירים בז'אנר.
// getNumberOfGenreChanges יחפש את השיר במערך הדינמי של השירים, ואם הוא קיים יחזיר את מספר הז'אנרים שהשיר החליף עד כה.
// כלומר אין צורך ב hash
// אם כך אתחול המבנה יהיה פשוט, אצור מערכים דינמיים של שירים וז'אנרים, ריקים 
// כל הוספה של ז'אנר תיצור איבר חדש במערך הדינמי של הז'אנרים, עם קבוצה בגודל 0 ועם שם קבוצה של מספר הז'אנר.
// כל הוספה של שיר תחפש את הז'אנר המתאים לו במערך הדינמי של הז'אנרים,
// אם נמצא את הז'אנר, ניצור איבר חדש במערך הדינמי של השירים עם קבוצה פיקטיבית,
// ונאחד את השיר עם הז'אנר המתאים לו.

DSpotify::DSpotify(){
    UnionFind genreUnionFind; // Union-Find structure for genres and songs inside it we have dynamic arrays for song and genres
    AVLTree songGenreMap; // AVL tree to map song IDs to
}

DSpotify::~DSpotify(){
    songGenreMap.clear();   
}

StatusType DSpotify::addGenre(int genreId){
    if (genreId <= 0) {
        return StatusType::INVALID_INPUT;
    }
    if (genreUnionFind.findGenre(genreId) != 0) {
        return StatusType::FAILURE; // Genre already exists
    }
    genreUnionFind.genres.push_back(Genre(genreId));
    std::cout << "Genre added: " << genreId << std::endl;
    return StatusType::SUCCESS;
}

StatusType DSpotify::addSong(int songId, int genreId){
    if (songId <= 0 || genreId <= 0) {
        return StatusType::INVALID_INPUT;
    }
    if (genreUnionFind.findSong(songId) != 0) {
        return StatusType::FAILURE; // Song already exists
    }
    genreUnionFind.Makeset(songId, genreId);
    
    std::cout << "song added: " << songId << " genre " << genreId << std::endl;

    return StatusType::SUCCESS;
}

StatusType DSpotify::mergeGenres(int genreId1, int genreId2, int genreId3){
    return StatusType::FAILURE;
}

output_t<int> DSpotify::getSongGenre(int songId){
    if (songId <= 0) {
        return output_t<int>(StatusType::INVALID_INPUT);
    }
    Song* song = genreUnionFind.findobjectSong(songId);
    if (song == nullptr) {
        return output_t<int>(StatusType::FAILURE); // Song not found
    }
    int genreId = genreUnionFind.Find(song->parent);
    if (genreId == -1) {
        return output_t<int>(StatusType::FAILURE); // Genre not found
    }
    return output_t<int>(genreId);
}

output_t<int> DSpotify::getNumberOfSongsByGenre(int genreId){
    if (genreId <= 0) {
        return output_t<int>(StatusType::INVALID_INPUT);
    }
    Genre* genre = genreUnionFind.findobjectGenre(genreId);
    if (genre == nullptr) { 
    return output_t<int>(StatusType::FAILURE);
    }
    int memberCount = genre->getSize();
    return output_t<int>(memberCount);
}

output_t<int> DSpotify::getNumberOfGenreChanges(int songId){
  
    return 0;
}
