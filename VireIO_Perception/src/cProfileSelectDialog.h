#include <Ui_cProfileSelectDialog.h>

class cProfileSelectDialog : public QDialog {
Q_OBJECT
public:
	QString selectedProfileName;

	cProfileSelectDialog( QWidget* parent );

private:
	Ui_cProfileSelectDialog ui;

private slots:
	void on_ok_clicked();
};
