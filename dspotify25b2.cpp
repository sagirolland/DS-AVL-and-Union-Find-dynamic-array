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

StatusType DSpotify::addSong(int songId, int genreId)
{
	if (songId <= 0 || genreId <= 0)
		return StatusType::INVALID_INPUT;

	if (songs->findItem(songId) || genres->findItem(genreId) == NULL)
		return StatusType::FAILURE;

	std::shared_ptr<Genre> genre = genres->findItem(genreId);

	std::shared_ptr<Song> song;
	int ndx = uf->makeSet();

	try
	{
		song = std::make_shared<Song>(songId, ndx);
	}
	catch (const std::bad_alloc &e)
	{
		return StatusType::ALLOCATION_ERROR;
	}

	int genreRoot = -1;
	if (genre->getIndex() != -1)
	{
		genreRoot = uf->find(genre->getIndex());
	}
	else
	{
		int foundRoot = -1;
		for (int i = 0; i < uf->num_merges.size(); ++i)
		{
			if (uf->getGenre(i) == genreId)
			{
				int candidateRoot = uf->find(i);
				if (uf->getGenre(candidateRoot) == genreId)
				{
					foundRoot = candidateRoot;
					// לא עוצרים, ממשיכים עד הסוף כדי לקבל את השורש הכי עדכני
				}
			}
		}
		if (foundRoot != -1)
		{
			genreRoot = foundRoot;
		}
		else
		{
			// אין שורש קיים - ניצור קבוצה חדשה
			genre->setIndex(ndx);
			uf->setGenreIndex(ndx, genreId);
			songs->insertItem(songId, song);
			genre->addSongs(1);
			return StatusType::SUCCESS;
		}
	}
	//std::cout << "Before Union: ndx=" << ndx << " genreRoot=" << genreRoot
	//		  << " num_merges[ndx]=" << uf->getNumChanges(ndx)
	//		  << " num_merges[genreRoot]=" << uf->getNumChanges(genreRoot) << std::endl;

	int newRoot = uf->Union(ndx, genreRoot, true);

	//std::cout << "After Union: ndx=" << ndx << " parent=" << uf->parent[ndx] <<
	//	" num_merges[ndx]=" << uf->getNumChanges(ndx) << std::endl;

	uf->setNumMerges(ndx, 0);

	//std::cout << "After setNumMerges: ndx=" << ndx
	//		  << " num_merges[ndx]=" << uf->getNumChanges(ndx) << std::endl;
	genre->setIndex(newRoot);
	uf->setGenreIndex(newRoot, genreId);
	uf->find(ndx); // path compression

	songs->insertItem(songId, song);
	genre->addSongs(1);
	return StatusType::SUCCESS;
}

StatusType DSpotify::mergeGenres(int genreId1, int genreId2, int genreId3)
{
	if (genreId1 <= 0 || genreId2 <= 0 || genreId3 <= 0)
		return StatusType::INVALID_INPUT;

	if (genreId1 == genreId2 || genreId2 == genreId3 || genreId3 == genreId1)
		return StatusType::INVALID_INPUT;

	std::shared_ptr<Genre> genre1 = genres->findItem(genreId1);
	std::shared_ptr<Genre> genre2 = genres->findItem(genreId2);
	if (!genre1 || !genre2 || genres->findItem(genreId3))
		return StatusType::FAILURE;

	std::shared_ptr<Genre> genre3;
	try
	{
		genre3 = std::make_shared<Genre>(genreId3);
	}
	catch (const std::bad_alloc &e)
	{
		return StatusType::ALLOCATION_ERROR;
	}

	// Create new singleton node for the new genre
	int newIdx = uf->makeSet();
	genre3->setIndex(newIdx);
	uf->setGenreIndex(newIdx, genreId3);

	// Union both old genres with the new genre node, forcing newIdx as root
	int idx1 = genre1->getIndex();
	int idx2 = genre2->getIndex();
	int root1 = (idx1 != -1) ? uf->find(idx1) : -1;
	int root2 = (idx2 != -1) ? uf->find(idx2) : -1;

	if (root1 != -1 && root1 != newIdx)
		uf->Union(root1, newIdx, true);
	if (root2 != -1 && root2 != newIdx && root2 != root1)
		uf->Union(root2, newIdx, true);
	// After union, update the new root and genre index
	int newRoot = uf->find(newIdx);
	genre3->setIndex(newRoot);
	uf->setGenreIndex(newRoot, genreId3);

	// Invalidate old genres' indices
	if (idx1 != -1)
		uf->setGenreIndex(idx1, -1);
	if (idx2 != -1)
		uf->setGenreIndex(idx2, -1);
	genre1->setIndex(-1);
	genre2->setIndex(-1);

	// Update song counts
	genre3->addSongs(genre1->getSongsCount() + genre2->getSongsCount());
	genre1->removeAllSongs();
	genre2->removeAllSongs();

	genres->insertItem(genreId3, genre3);
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

output_t<int> DSpotify::getNumberOfGenreChanges(int songId)
{
	if (songId <= 0)
		return output_t<int>(StatusType::INVALID_INPUT);

	std::shared_ptr<Song> song = songs->findItem(songId);
	if (!song)
		return output_t<int>(StatusType::FAILURE);

	uf->find(song->getIndex()); // path compression and update
	//std::cout << "getNumberOfGenreChanges: songId=" << songId
	//		  << " index=" << song->getIndex()
	//		  << " num_merges=" << uf->getNumChanges(song->getIndex())
	//		  << " result=" << (uf->getNumChanges(song->getIndex()) + 1) << std::endl;
	return output_t<int>(uf->getNumChanges(song->getIndex())+1);
}
