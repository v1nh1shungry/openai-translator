#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>
#include <QString>
#include <memory>

class Translator : public QObject {
  Q_OBJECT

public:
  Translator(QString apikey);

public slots:
  void translate(QString text);

signals:
  void complete(QString result);

private:
  QNetworkAccessManager manager;
  QNetworkRequest request;
  QJsonObject body;
  std::unique_ptr<QNetworkReply> reply;
  QString last_text;
};

#endif // TRANSLATOR_H
