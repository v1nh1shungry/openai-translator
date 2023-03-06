#include "popupwindow.h"
#include "translator.h"

#include <QApplication>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <filesystem>

QString getApiKey(char *path) {
  auto dir = std::filesystem::path{path}.parent_path();
  QFile file{dir / "config.json"};
  if (!file.exists())
    return {};
  file.open(QFile::ReadOnly);
  auto config = QJsonDocument::fromJson(file.readAll()).object();
  return config["apikey"].toString();
}

int main(int argc, char *argv[]) {
  QApplication app{argc, argv};
  auto *window = PopupWindow::instance();
  auto apikey = getApiKey(argv[0]);
  if (apikey.isEmpty()) {
    QMessageBox::critical(window, "OpenAI Translator",
                          "Invalid OpenAI API key!!!");
    return 1;
  }
  Translator translator(apikey);
  QObject::connect(window, &PopupWindow::activated, &translator,
                   &Translator::translate);
  QObject::connect(&translator, &Translator::complete, window,
                   &PopupWindow::display);
  window->show();
  return app.exec();
}
