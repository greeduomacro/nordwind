/*
 * OSIImageIOHandlerPrivate.cpp
 *
 *  Created on: Sep 7, 2010
 *      Author: idstein
 */
#include "OSIImageIOHandlerPrivate.hpp"
#include <qvariant.h>
#include <qrect.h>
#include <qimage.h>

OSIImageIOHandlerPrivate::OSIImageIOHandlerPrivate(OSIImageIOHandler* q) :
	q_ptr(q) {
	QFile* file = qobject_cast<QFile*> (q_ptr->device());
	if (!file || !file->fileEngine() || !file->fileEngine()->supportsExtension(
		(QAbstractFileEngine::Extension)data::Index::IndexExtension))
		return;
	file->fileEngine()->extension((QAbstractFileEngine::Extension)data::Index::IndexExtension, NULL,
		static_cast<QAbstractFileEngine::ExtensionReturn*>(const_cast<data::Index*>(&mIndex)));
}

OSIImageIOHandlerPrivate::~OSIImageIOHandlerPrivate() {
}

bool OSIImageIOHandlerPrivate::canRead() const {
	return mIndex.isValid();
}

bool OSIImageIOHandlerPrivate::read(QImage* image) {
	Q_UNUSED(image);
	return false;
}

int OSIImageIOHandlerPrivate::currentImageNumber() const {
	return 0;
}

QRect OSIImageIOHandlerPrivate::currentImageRect() const {
	return QRect();
}

int OSIImageIOHandlerPrivate::imageCount() const {
	return 0;
}

bool OSIImageIOHandlerPrivate::jumpToImage(int imageNumber) {
	Q_UNUSED(imageNumber);
	return false;
}

bool OSIImageIOHandlerPrivate::jumpToNextImage() {
	return false;
}

int OSIImageIOHandlerPrivate::loopCount() const {
	return 0;
}

int OSIImageIOHandlerPrivate::nextImageDelay() const {
	return 0;
}

QVariant OSIImageIOHandlerPrivate::option(QImageIOHandler::ImageOption option) const {
	switch (option) {
	case QImageIOHandler::ImageFormat:
		return QImage::Format_ARGB32_Premultiplied;
	default:
		return QVariant();
	}
}

void OSIImageIOHandlerPrivate::setOption(QImageIOHandler::ImageOption option,
		const QVariant& value) {
	Q_UNUSED(option);
	Q_UNUSED(value);
}

bool OSIImageIOHandlerPrivate::supportsOption(
		QImageIOHandler::ImageOption option) const {
	Q_UNUSED(option);
	return option == QImageIOHandler::ImageFormat;
}

bool OSIImageIOHandlerPrivate::write(const QImage& image) {
	Q_UNUSED(image);
	return false;
}
