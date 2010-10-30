/*
 * TileInfoLand.cpp
 *
 *  Created on: 13.12.2009
 *      Author: idstein
 */

#include "TileData.hpp"
#include <qfile.h>
#include <qdebug.h>

using namespace data;

Info::Info() 
: mFullInfo(false),
mGeneric(0) {
}

Info::Flags Info::flags() const {
	return mFlags;
}

quint16 Info::textureID() const {
	return mGeneric;
}

QString Info::name() const {
	return mName;
}

quint8 Info::weight() const {
	return mGeneric >> 8;
}

quint8 Info::quality() const {
	return mGeneric & 0xFF;
}

quint16 Info::hitpoints() const {
	return mHitpoints;
}

quint8 Info::quantity() const {
	return mQuantity;
}

quint16 Info::animation() const {
	return mAnimation;
}

quint8 Info::hue() const {
	return mHue;
}

quint8 Info::stackOffset() const {
	return mStackOffset;
}

quint8 Info::value() const {
	return mValue;
}

quint8 Info::height() const {
	return mHeight;
}

bool Info::hasFullInfo() const {
	return mFullInfo;
}

TileData::TileData(QString fileName) {
	setObjectName("TileData");
	setParent(qApp);
	QFile file(fileName);
	if(!file.open(QIODevice::ReadOnly)){ 
		qWarning() << "Unable to open" << fileName;	
		return;
	}
	QDataStream stream(file.readAll());
	stream.setByteOrder(QDataStream::LittleEndian);
        qDebug() << "TileData" << stream.device()->size() << "Bytes";
        // 16384 land infos + bytes left / block size * 32 elements per block
        resize(16384+ ((stream.device()->size() - 512*(32*26+4)) / (32*37+4))
			* 32);
		int flags;
		char tmp[20];
		ID i = 0;
		for (QVector<Info>::iterator iter = begin(); iter!=end(); i++,iter++) {
			if (i % 32 == 0)
				stream.skipRawData(4); // skip 4 byte header;
		stream >> flags >> iter->mGeneric;
		if (i > 0x4000) {
			iter->mFullInfo = true;
			stream >> iter->mHitpoints
			>> iter->mUnknown1 >> iter->mQuantity >> iter->mAnimation
			>> iter->mUnknown2 >> iter->mHue >> iter->mStackOffset
			>> iter->mValue >> iter->mHeight;
		}
		stream.readRawData(tmp, 20);
		iter->mFlags = Info::Flags(flags);
		iter->mName = QString::fromAscii(tmp,qstrlen(tmp));
	}
	qDebug() << size() << "Indices read.";
}

TileData::~TileData() {
}
