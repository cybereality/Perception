#include <Ui_cProfileSelectDialog.h>
#include <cGameProfile.h>

class cProfileSelectDialog : public QDialog {
Q_OBJECT
public:
	cGameProfile* selectedProfile;

	cProfileSelectDialog( QWidget* parent );

private:
	Ui_cProfileSelectDialog ui;

private slots:
	void on_ok_clicked();
};
