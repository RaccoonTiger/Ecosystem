#include "QAboutDialog.h"


#include <QDialogButtonBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QIcon>


QAboutDialog::QAboutDialog(QString const & aboutText, QWidget * parent)
	: QAboutDialog(parent)
{
	setAboutText(aboutText);
}

QAboutDialog::QAboutDialog(QWidget *parent)
	:	QDialog(parent),
		mAboutLabel{ new QLabel }
{
	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

	QDialogButtonBox * dialogButton{ new QDialogButtonBox };
	dialogButton->addButton(QDialogButtonBox::Ok);

	QLabel * iconLabel{ new QLabel };
	iconLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
	iconLabel->setPixmap(QIcon(":/StartUpProjectDemo/icon").pixmap(64, 64));

	QHBoxLayout * layout{ new QHBoxLayout };
	layout->addWidget(iconLabel);
	layout->addWidget(mAboutLabel);

	QVBoxLayout * mainLayout{ new QVBoxLayout };
	mainLayout->addLayout(layout);
	mainLayout->addWidget(dialogButton);

	setLayout(mainLayout);

	connect(dialogButton, &QDialogButtonBox::accepted, this, &QAboutDialog::accept);
}

void QAboutDialog::setAboutText(QString const & aboutText)
{
	mAboutLabel->setText(aboutText);
}
