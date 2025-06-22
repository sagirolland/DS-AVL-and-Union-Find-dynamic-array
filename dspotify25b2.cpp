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
    songs = std::make_shared<HashTable<int, std::shared_ptr<Song>>>();
	genres = std::make_shared<HashTable<int, std::shared_ptr<Genre>>>();
	uf = std::make_shared<UnionFind>();
}

DSpotify::~DSpotify() = default;

StatusType DSpotify::addGenre(int genreId){
    if (genreId <= 0) 
        return StatusType::INVALID_INPUT;

    if (genres->findItem(genreId)) 
        return StatusType::FAILURE;

    std::shared_ptr<Genre> genre;

	try {
		genre = std::make_shared<Genre>(genreId);
	}
	catch (const std::bad_alloc& e) {
		return StatusType::ALLOCATION_ERROR;
	}
	
	genres->insertItem(genreId, genre);
	return StatusType::SUCCESS;
}

StatusType DSpotify::addSong(int songId, int genreId){
    if (songId <= 0 || genreId <= 0)
		return StatusType::INVALID_INPUT;

	if (songs->findItem(songId) || genres->findItem(genreId) == NULL)
		return StatusType::FAILURE;

	std::shared_ptr<Genre> genre = genres->findItem(genreId);

	std::shared_ptr<Song> song;
    int ndx = uf->makeSet();

	try {
		song = std::make_shared<Song>(songId, ndx);
	}
	catch (const std::bad_alloc& e) {
		return StatusType::ALLOCATION_ERROR;
	}

    if (genre->getIndex() == -1){
        genre->setIndex(ndx);
        uf->setGenreIndex(ndx, genreId);
    }
    else {
        genre->setIndex(uf->Union(ndx, genre->getIndex()));
        uf->setGenreIndex(genre->getIndex(), genreId);
    }

	songs->insertItem(songId, song);
	genre->addSongs(1);
    //std::cout << getNumberOfGenreChanges(songId).ans() << std::endl;
	return StatusType::SUCCESS;
}

StatusType DSpotify::mergeGenres(int genreId1, int genreId2, int genreId3){
    if (genreId1 <= 0 || genreId2 <= 0 || genreId3 <= 0)
		return StatusType::INVALID_INPUT;

    if (genreId1 == genreId2 || genreId2 == genreId3 || genreId3 == genreId1)
		return StatusType::INVALID_INPUT;
	
	std::shared_ptr<Genre> genre1 = genres->findItem(genreId1);
	std::shared_ptr<Genre> genre2 = genres->findItem(genreId2);
	if (!genre1 || !genre2 || genres->findItem(genreId3))
		return StatusType :: FAILURE;

    std::shared_ptr<Genre> genre3;

	try {
		genre3 = std::make_shared<Genre>(genreId3);
	}
	catch (const std::bad_alloc& e) {
		return StatusType::ALLOCATION_ERROR;
	}

    genres->insertItem(genreId3, genre3);

    // genre3->setIndex(uf->Union(genre1->getIndex(), genre2->getIndex()));
    // uf->setGenreIndex(genre3->getIndex(), genreId3);
    // uf->setGenreIndex(genre1->getIndex(), -1);
    // genre1->setIndex(-1);
    // uf->setGenreIndex(genre2->getIndex(), -1);
    // genre2->setIndex(-1);

    int idx1 = genre1->getIndex();
    int idx2 = genre2->getIndex();

    int newRoot = uf->Union(idx1, idx2);
    genre3->setIndex(newRoot);
    uf->setGenreIndex(newRoot, genreId3);

    if (idx1 != newRoot) uf->setGenreIndex(idx1, -1);
    if (idx2 != newRoot) uf->setGenreIndex(idx2, -1);

    genre1->setIndex(-1);
    genre2->setIndex(-1);

    genre3->addSongs(genre1->getSongsCount() + genre2->getSongsCount());
    genre1->removeAllSongs();
    genre2->removeAllSongs();

    return StatusType::SUCCESS;
}

output_t<int> DSpotify::getSongGenre(int songId){
    if(songId <= 0)
		return output_t<int>(StatusType::INVALID_INPUT);

	std::shared_ptr<Song> song = songs->findItem(songId);
	if(!song)
		return output_t<int>(StatusType::FAILURE);
	
	return output_t<int>(uf->getGenre(song->getIndex()));
}

output_t<int> DSpotify::getNumberOfSongsByGenre(int genreId){
    if(genreId <= 0)
		return output_t<int>(StatusType::INVALID_INPUT);
	
	std::shared_ptr<Genre> genre = genres->findItem(genreId);
	if(!genre)
		return output_t<int>(StatusType::FAILURE);
	
    return output_t<int>(genre->getSongsCount());
}

output_t<int> DSpotify::getNumberOfGenreChanges(int songId){
    if(songId <= 0)
		return output_t<int>(StatusType::INVALID_INPUT);

	std::shared_ptr<Song> song = songs->findItem(songId);
	if(!song)
		return output_t<int>(StatusType::FAILURE);
	
	return output_t<int>(uf->getNumChanges(song->getIndex()));
}
