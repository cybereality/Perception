#include <qmainwindow.h>
#include <qsettings.h>
#include <Ui_cMainWindow.h>


class cMainWindow : public QMainWindow {
Q_OBJECT
public:
	cMainWindow();
	~cMainWindow();

private:
	Ui_cMainWindow ui;
	QSettings      settings;
	void ScanGames();

private slots:
	void on_games_customContextMenuRequested( const QPoint& );
	void on_games_itemDoubleClicked         ( QTreeWidgetItem *item , int column );
};
