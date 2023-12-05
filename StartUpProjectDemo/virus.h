#pragma once
#ifndef Q_ARROW_ITEM_
#define Q_ARROW_ITEM_


#include <QGraphicsItem>
#include <QBrush>


class virus : public QGraphicsItem
{
public:
	// On choisi un constructeur par défaut offrant optionnellement un accès à tous les paramètres de la forme.
	virus(double lifeExpectancy = 1.0, QPointF const& initialPosition = QPointF(), qreal initialOrientationDegrees = 0.0, qreal initialSpeed = 1.0, qreal scale = 1.0, QBrush const& brush = Qt::white, QGraphicsItem* parent = nullptr);
	~virus() override = default;
	virus(virus const&) = default;
	virus& operator=(virus const&) = delete;

	// Mutateur de couleur et de vitesse
	QBrush getColor();
	qreal getSpeed();
	void setColor(QBrush const& brush);
	void setSpeed(qreal speed);

	// Fonction abstraite de QGraphicsItem (DOIT être "override") : retourne la taille de l'item
	QRectF boundingRect() const override;
	// Fonction abstraite de QGraphicsItem (DOIT être "override") : dessine la forme
	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

	// Fonction virtuelle de QGraphicsItem (peut être "override") : fait un tick de simulation
	void advance(int phase) override;

	bool isAlive() const;

	void assimiler();
	void suivreCellule();
	void fuirGlobuleBlanche();

protected:
	// L'espérance de vie et l'âge
	double mLifeExpectancy;
	double mAge;
	// La couleur de fond
	QBrush mBrush;
	// La forme 
	QPolygonF mShape;
	// La vitesse
	qreal mSpeed;

	// 
	QPointF mNewPosition;
	qreal mNewOrientation;

	// Fonctions mathématiques utilitaires servant à réaliser la "téléportation".
	// Idéalement, devraient être dans une autre classe utilitaire de calcul.
	static qreal warp(qreal value, qreal begin, qreal end);
	void warp(QPointF& point);
};

#endif // Q_ARROW_ITEM_H
