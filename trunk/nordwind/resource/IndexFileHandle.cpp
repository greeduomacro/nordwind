/*
 * IndexFileHandle.cpp
 *
 *  Created on: Sep 8, 2010
 *      Author: idstein
 */

#include "IndexFileHandle.hpp"
#include <qdebug.h>
#include <qdatastream.h>

using namespace resource;

QHash<QString, QSharedPointer<IndexFileHandle> > IndexFileHandle::mInstances;

QSharedPointer<IndexFileHandle> IndexFileHandle::instance(
		const QString& mulFile, const QString& indexFile) {
	if (!mInstances.contains(mulFile))
		new IndexFileHandle(mulFile, indexFile);
	return mInstances[mulFile];
}

IndexFileHandle::IndexFileHandle(const QString& mulFile,
		const QString& indexFile) {
	QFile indexFileHandle(indexFile);
	qDebug() << "Index build of " << mulFile << " from " << indexFile;
	if (indexFileHandle.open(QIODevice::ReadOnly)) {
		qDebug() << "Index build started";
		QDataStream indexStream(indexFileHandle.readAll());
		indexStream.setByteOrder(QDataStream::LittleEndian);
		resize(indexFileHandle.size()/12);
		for (QVector<Index>::iterator iter = begin(); iter!=end(); iter++)
			indexStream >> iter->mOffset >> iter->mSize >> iter->mExtra;
		qDebug() << count() << " indices read.";
	} else
		qWarning() << indexFileHandle.errorString();
	mInstances.insert(mulFile, QSharedPointer<IndexFileHandle> (this));
}

IndexFileHandle::~IndexFileHandle() {
}