#ifndef POPUPWINDOW_H
#define POPUPWINDOW_H

#include <QWidget>
#include <Windows.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class PopupWindow;
}
QT_END_NAMESPACE

class PopupWindow : public QWidget {
  Q_OBJECT

public:
  static PopupWindow *instance();
  ~PopupWindow();

public slots:
  void display(QString text);

signals:
  void activated(QString text);

private:
  PopupWindow(QWidget *parent = nullptr);
  void setupJob();

  friend LRESULT CALLBACK MouseHookProc(int, WPARAM, LPARAM);

private:
  Ui::PopupWindow *ui;
};
#endif // POPUPWINDOW_H
