#ifndef OPENING_BOOK_H
#define OPENING_BOOK_H

#include <QtGlobal>
#include <QMultiMap>
#include "chessboard/bookmove.h"

class QString;
class QDataStream;


/*!
 * \brief A collection of opening moves for chess.
 *
 * OpeningBook is a container (binary tree) class for opening moves that
 * can be played by the GUI. When the game goes "out of book", control
 * of the game is transferred to the players.
 *
 * The opening book is stored externally in a binary file. When it's needed,
 * it is loaded in memory, and positions can be found quickly by searching
 * the book for Zobrist keys that match the current board position.
 */
class LIB_EXPORT OpeningBook
{
	public:
		/*!
		 * Loads a book from a binary file.
		 * \return True if successfull.
		 */
		bool load(const QString& filename);
		
		
		/*!
		 * Imports games in PGN format.
		 * \param filename The PGN file
		 * \param maxMoves Store at most this many full moves per game
		 * \return True if successfull.
		 */
		bool pgnImport(const QString& filename, int maxMoves);
		
		/*!
		 * Stores the book to a binary file.
		 * \return True if successfull.
		 */
		bool save(const QString& filename) const;
		
		/*!
		 * Returns a move that can be played in a position where the
		 * Zobrist key is \a key.
		 *
		 * If no matching moves are found, an empty (illegal) move is
		 * returned.
		 *
		 * If there are multiple matches, a random, weighted move is
		 * returned. Popular moves have a higher probablity of being
		 * selected than unpopular ones.
		 */
		BookMove move(quint64 key) const;
	
	protected:
		/*!
		 * \brief An entry in the opening book.
		 *
		 * \note Each entry is paired with a Zobrist key.
		 * \note The book file may not use the same structure
		 * for the entries.
		 */
		struct Entry
		{
			/*! A book move. */
			BookMove move;
			/*!
			 * A weight or score, usually based on popularity
			 * of the move. The higher the weight, the more
			 * likely the move will be played.
			 */
			quint16 weight;
		};
		
		/*! The type of binary tree. */
		typedef QMultiMap<quint64, Entry> Map;
		
		
		/*! Returns the book's chess variant. */
		virtual Chess::Variant variant() const = 0;
		
		/*! Adds a new entry to the book. */
		void addEntry(const BookMove& move, quint64 key);
		
		/*! Loads a new book entry from \a in. */
		virtual void loadEntry(QDataStream& in) = 0;
		
		/*! Saves the key and entry pointed to by \a it, to \a out. */
		virtual void saveEntry(const Map::const_iterator& it,
		                       QDataStream& out) const = 0;
		
		/*! The binary tree. */
		Map m_map;
};

#endif // OPENING_BOOK_H