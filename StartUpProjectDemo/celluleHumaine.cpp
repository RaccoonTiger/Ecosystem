#include "celluleHumaine.h"

#include <QPainter>
#include <QRandomGenerator>
#include <QtMath>
#include <QGraphicsScene>
#include <QtMath>


	celluleHumaine::celluleHumaine(double lifeExpectancy, QPointF const& initialPosition, qreal initialOrientationDegrees, qreal initialSpeed, qreal scale, QBrush const& brush, QGraphicsItem* parent)
	: QGraphicsItem(parent),
	mLifeExpectancy{ lifeExpectancy },
	mAge{ 0.0 },
	mBrush(brush),
	mSpeed{ initialSpeed }
{
	// note: le polygone pointe vers la droite naturellement, donc attention!

	mShape
		//centre en premier
		<< QPointF(0, 0)
		<< QPointF(-0.25, 0.5)
		<< QPointF(0.25, 0.5)
		<< QPointF(1, 0)
		<< QPointF(0.25, -0.5)
		<< QPointF(-0.25, -0.5);

	setPos(initialPosition);
	setRotation(initialOrientationDegrees);
	setScale(scale);
}
QBrush celluleHumaine::getColor()
{
	return mBrush;
}

qreal celluleHumaine::getSpeed()
{
	return mSpeed;
}

void celluleHumaine::setColor(QBrush const& brush)
{
	mBrush = brush;
}

void celluleHumaine::setSpeed(qreal speed)
{
	mSpeed = speed;
}

QRectF celluleHumaine::boundingRect() const
{
	return mShape.boundingRect();
}

void celluleHumaine::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	painter->setPen(Qt::NoPen);
	painter->setBrush(mBrush);
	painter->drawPolygon(mShape);
}

void celluleHumaine::advance(int phase)
{
	//
	// Détermine le comportement
	if (phase == 0) {
		static qreal const maxDeltaOrientation{ 12.5 }; // in °
		// Détermine la nouvelle orientation selon une variation aléatoire dans l'intervalle [-maxDeltaOrientation, maxDeltaOrientation] => ± 12.5°
		mNewOrientation = rotation() + QRandomGenerator::global()->bounded(2.0 * maxDeltaOrientation) - maxDeltaOrientation;
		qreal newOrientationRadians{ qDegreesToRadians(mNewOrientation) };
		// Détermine la nouvelle position selon la nouvelle orientation et la vitesse
		mNewPosition = pos() + QPointF(qCos(newOrientationRadians), qSin(newOrientationRadians)) * mSpeed;
		// Si la nouvelle position est à l'extérieur de la scène, la nouvelle position est téléportée à la région opposée de la scène
		warp(mNewPosition);
		//
		// Applique le comportement
	}
	else if (phase == 1) {
		// Applique la nouvelle orientation et la nouvelle position
		setRotation(mNewOrientation);
		setPos(mNewPosition);

		// Plus vieux... le temps fil si rapidement...
		mAge += 0.030;
	}
}

bool celluleHumaine::isAlive() const
{
	return mAge <= mLifeExpectancy;
}

void celluleHumaine::splitCellule()
{
}

qreal celluleHumaine::warp(qreal value, qreal begin, qreal end) {
	qreal const width = end - begin;
	return value - qFloor((value - begin) / width) * width;
}

void celluleHumaine::warp(QPointF& point) {
	point.setX(warp(point.x(), scene()->sceneRect().left(), scene()->sceneRect().right()));
	point.setY(warp(point.y(), scene()->sceneRect().top(), scene()->sceneRect().bottom()));
}