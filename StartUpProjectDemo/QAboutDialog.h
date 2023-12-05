#pragma once


#include <QDialog>


class QLabel;


class QAboutDialog : public QDialog
{
	Q_OBJECT

public:
	QAboutDialog(QString const & aboutText, QWidget * parent = nullptr);
	QAboutDialog(QWidget * parent = nullptr);
	~QAboutDialog() override = default;

	void setAboutText(QString const & aboutText);

private:

	QLabel * mAboutLabel;
};
