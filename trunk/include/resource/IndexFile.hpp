/*
 * OSIIndexFile.hpp
 *
 *  Created on: 13.12.2009
 *      Author: idstein
 */

#ifndef INDEXFILE_HPP_
#define INDEXFILE_HPP_

#include <qobject.h>
#include "..\Typedefs.hpp"
#include "IndexEntry.hpp"

namespace resource {

	class IndexFile : public QObject {
		public:
			struct Entry {
				quint32 mOffset;
				quint32 mSize;
				quint32 mExtra;
				bool isValid() const;
				quint16 getWidth() const;
				quint16 getHeight() const;
			};
			IndexFile( QString _indexFile, QString _dataFile, QObject* _parent );
			virtual ~IndexFile();
		protected:
			QByteArray getData( ID _id );
			QHash< ID, Entry > mEntries;
			QDataStream mIndexStream;
			QDataStream mDataStream;
	};

	inline bool IndexFile::Entry::isValid() const {
		return (mOffset!=0xFFFFFFFF&&mSize!=0xFFFFFFFF&&
				mSize!=0) ? true : false;
	}

	inline quint16 IndexFile::Entry::getWidth() const {
		return mExtra >> 16;
	}

	inline quint16 IndexFile::Entry::getHeight() const {
		return mExtra & 0xFF;
	}

}  // namespace resource
#endif /* OSIINDEXFILE_HPP_ */
