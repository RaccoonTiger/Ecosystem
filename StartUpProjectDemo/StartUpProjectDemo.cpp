#include "StartUpProjectDemo.h"


#include <QRandomGenerator>
#include <QGraphicsView>
#include "QControlBar.h"
#include "QParameters.h"
#include <QPushButton>
#include "QAboutDialog.h"
#include <QHBoxLayout>
#include "celluleDeBase.h"
#include "virus.h"
#include "celluleHumaine.h"


size_t	const StartUpProjectDemo::sMaxNbrOfItems{ 1000 };
QSize	const StartUpProjectDemo::sSceneSize(1000, 600);
QColor	const StartUpProjectDemo::sSceneBackgroundColor(QColor::fromHsl(0, 0, 0));
QString	const StartUpProjectDemo::sControlStartText("Démarrer la simulation");
QString	const StartUpProjectDemo::sControlStopText("Terminer la simulation");
QString	const StartUpProjectDemo::sControlPauseText("Mettre la simulation sur pause");
QString	const StartUpProjectDemo::sControlResumeText("Reprendre la simulation");
QString	const StartUpProjectDemo::sControlStepText("Faire un pas de simulation");
QColor	const StartUpProjectDemo::sControlRunningColor(0, 255, 0);
QColor	const StartUpProjectDemo::sControlPauseColor(255, 196, 0);
QColor	const StartUpProjectDemo::sControlStoppedColor(255, 0, 0);
QColor	const StartUpProjectDemo::sControlNotReadyColor(230, 230, 230);
qreal	const StartUpProjectDemo::sMinLifeExpectancy{ 1.0 };
qreal	const StartUpProjectDemo::sMaxLifeExpectancy{ 50.0 };
qreal	const StartUpProjectDemo::sCenterRadius{ 25 };
qreal	const StartUpProjectDemo::sMinOrientationDegrees{ 0.0 };
qreal	const StartUpProjectDemo::sMaxOrientationDegrees{ 360.0 };
qreal	const StartUpProjectDemo::sMinSpeed{ 1.0 };
qreal	const StartUpProjectDemo::sMaxSpeed{ 5.0 };
qreal	const StartUpProjectDemo::sMinSize{ 7.5 };
qreal	const StartUpProjectDemo::sMaxSize{ 15.0 };

const QString StartUpProjectDemo::sAboutButtonText("À propos...");
const QString StartUpProjectDemo::sAboutText( 
R"..(GPA434 
Ingénierie des systèmes orientés objets

Laboratoire  3
Simulation d'un écosystème + UML

Ce projet consiste à simuler l'intérieur d'un vaisseau sanguin,
consistant de globules blanches, de virus et de cellules humaines.
Le virus est supposé chasser la cellule humaine (statique),
la globule blanche est supposée protéger la même cellule,
Créeant ainsi une dynamique de va-et-vient entre 
prédateur et sentinel.

Le projet à du être accéléré et testé pour qu'il fonctionne, mais 
il manque beaucoup de fonctionnalités encore.

Réalisé par :
   - Stanley Vu
   - ...
).." );



StartUpProjectDemo::StartUpProjectDemo(QWidget *parent)
	:	QMainWindow(parent),
		mSimulationView{ new QGraphicsView(&mGraphicsScene) },
		mParameters{ new QParameters(sMaxNbrOfItems) },
		mControlBar{ new QControlBar(Qt::Vertical) },
		mAboutButton{ new QPushButton(sAboutButtonText) }
{
	setWindowTitle("TP3 GPA434");
	setWindowIcon(QIcon(":/StartUpProjectDemo/icon"));

	ui.setupUi(this);

	mControlBar->setReady();
	mControlBar->setActionText(QControlBar::Action::Start, sControlStartText);
	mControlBar->setActionText(QControlBar::Action::Stop, sControlStopText);
	mControlBar->setActionText(QControlBar::Action::Pause, sControlPauseText);
	mControlBar->setActionText(QControlBar::Action::Resume, sControlResumeText);
	mControlBar->setActionText(QControlBar::Action::Step, sControlStepText);
	mControlBar->setStateColor(QControlBar::State::NotReady, sControlNotReadyColor);
	mControlBar->setStateColor(QControlBar::State::Paused, sControlPauseColor);
	mControlBar->setStateColor(QControlBar::State::Running, sControlRunningColor);
	mControlBar->setStateColor(QControlBar::State::Stopped, sControlStoppedColor);

	mGraphicsScene.setSceneRect(-sSceneSize.width() / 2, -sSceneSize.height() / 2, sSceneSize.width(), sSceneSize.height());
	
	QWidget * controlWidget{ new QWidget };
	QVBoxLayout * controlLayout{ new QVBoxLayout };
	controlWidget->setLayout(controlLayout);
	controlLayout->addWidget(mParameters);
	controlLayout->addStretch();
	controlLayout->addWidget(mControlBar);
	controlLayout->addWidget(mAboutButton);

	QWidget * centralWidget{ new QWidget };
	QHBoxLayout * centralLayout{ new QHBoxLayout };
	centralWidget->setLayout(centralLayout);

	centralLayout->addWidget(mSimulationView);
	centralLayout->addWidget(controlWidget);

	setCentralWidget(centralWidget);

	connect(mControlBar, &QControlBar::started, this, &StartUpProjectDemo::startSimulation);
	connect(mControlBar, &QControlBar::stopped, this, &StartUpProjectDemo::stopSimulation);
	connect(mControlBar, &QControlBar::paused, this, &StartUpProjectDemo::pauseSimulation);
	connect(mControlBar, &QControlBar::resumed, this, &StartUpProjectDemo::resumeSimulation);
	connect(mControlBar, &QControlBar::stepped, this, &StartUpProjectDemo::stepSimulation);

	connect(&mTimer, &QTimer::timeout, this, &StartUpProjectDemo::advance);

	connect(mAboutButton, &QPushButton::clicked, this, &StartUpProjectDemo::about);
}

void StartUpProjectDemo::advance()
{
    mGraphicsScene.advance();

    for (auto & item : mGraphicsScene.items()) {
		celluleDeBase* cellule{ dynamic_cast<celluleDeBase*>(item) };
        if (cellule && !cellule->isAlive()) {
            mGraphicsScene.removeItem(cellule);
            delete cellule;
        }
    }

	for (auto& item : mGraphicsScene.items()) {
		virus* cellule{ dynamic_cast<virus*>(item) };
		if (cellule && !cellule->isAlive()) {
			mGraphicsScene.removeItem(cellule);
			delete cellule;
		}
	}

	for (auto& item : mGraphicsScene.items()) {
		celluleHumaine* cellule{ dynamic_cast<celluleHumaine*>(item) };
		if (cellule && !cellule->isAlive()) {
			mGraphicsScene.removeItem(cellule);
			delete cellule;
		}
	}
}

void StartUpProjectDemo::startSimulation()
{
	// Vide la scène pour démarrer une nouvelle démo
	mGraphicsScene.clear();

	// Met un item rectangulaire pour bien voir les limites de la scène
	QGraphicsRectItem * background{ new QGraphicsRectItem(mGraphicsScene.sceneRect()) };
	background->setPen(Qt::NoPen);
	background->setBrush(sSceneBackgroundColor);
	mGraphicsScene.addItem(background);

	// Ajoute les globules blanches
	for (int i{ 0 }; i < mParameters->nbrOfItems(); ++i) {
		mGraphicsScene.addItem(
			new celluleDeBase(40,													// espérance de vie en seconde
							QPointF(random(-400,400),								// coordonnée x
									random(-400,400)),								// coordoonée y
							random(sMinOrientationDegrees, sMaxOrientationDegrees),	// orientation aléatoire
							3.0,													// vitesse lente
							15.0,													// taille grosse
							QColor::fromRgb(255,255,255)));							// couleur blanche
	}

	// Ajoute les virus
	for (int i{ 0 }; i < mParameters->nbrOfItems(); ++i) {
		mGraphicsScene.addItem(
			new virus(20.0									,			// espérance de vie en seconde
				randomPoint(-sCenterRadius, sCenterRadius),				// ils sont tous près de l'origine au départ!
				random(sMinOrientationDegrees, sMaxOrientationDegrees),	// orientation aléatoire
				5.0,													// vitesse rapide
				7.5						,								// taille petite
				QColor::fromRgb(255, 0, 0)));							// couleur rouge
	}

	// Ajoute les cellules Humaines
	for (int i{ 0 }; i < mParameters->nbrOfItems(); ++i) {
		mGraphicsScene.addItem(
			new celluleHumaine(60.0,									// espérance de vie en seconde
				QPointF(random(-400, 400),								// coordonnée x
						random(-400, 400)),								// random!
				random(sMinOrientationDegrees, sMaxOrientationDegrees),	// orientation aléatoire
				0.0,													// aucune vitesse
				12.5,													// taille moyenne
				QColor::fromRgb(0, 255, 0)));							// couleur verte
	}

	// Démarre la simulation
	mTimer.start(30);

	mParameters->setEnabled(false);
}

void StartUpProjectDemo::stepSimulation()
{
	mGraphicsScene.advance();
}

void StartUpProjectDemo::pauseSimulation()
{
	mTimer.stop();
}

void StartUpProjectDemo::resumeSimulation()
{
	mTimer.start(30);
}

void StartUpProjectDemo::stopSimulation()
{
	mTimer.stop();
	mParameters->setEnabled(true);
}

void StartUpProjectDemo::about()
{
	QAboutDialog about(sAboutText, this);
	about.exec();
}


double StartUpProjectDemo::random(qreal maxValue)
{
	return QRandomGenerator::global()->bounded(maxValue);
}

double StartUpProjectDemo::random(qreal minValue, qreal maxValue)
{
	return random(maxValue - minValue) + minValue;
}

QColor StartUpProjectDemo::randomColor()
{
	return QColor::fromRgb(QRandomGenerator::global()->generate());
}

QPointF StartUpProjectDemo::randomPoint(qreal min, qreal max)
{
	return QPointF(random(min, max), random(min, max));
}
