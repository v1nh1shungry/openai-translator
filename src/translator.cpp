#include "translator.h"

#include <QClipboard>
#include <QGuiApplication>
#include <QJsonArray>
#include <QJsonDocument>

Translator::Translator(QString apikey)
    : request{{"https://api.openai.com/v1/chat/completions"}} {
  request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
  request.setRawHeader("Authorization", ("Bearer " + apikey).toUtf8());
  body["model"] = "gpt-3.5-turbo";
  body["temperature"] = 0;
  body["max_tokens"] = 1000;
  body["top_p"] = 1;
  body["frequency_penalty"] = 1;
  body["presence_penalty"] = 1;
}

void Translator::translate(QString text) {
  if (text.isEmpty() || text == last_text)
    return;
  last_text = text;
  body["messages"] = QJsonArray{QJsonObject{
                                    {"role", "system"},
                                    {"content", "翻译成简体白话文"},
                                },
                                QJsonObject{
                                    {"role", "user"},
                                    {"content", text},
                                }};
  reply.reset(manager.post(request, QJsonDocument{body}.toJson()));
  connect(reply.get(), &QNetworkReply::readyRead, this, [this]() {
    auto res = QJsonDocument::fromJson(this->reply->readAll()).object();
    emit complete(res["choices"][0]["message"]["content"].toString().trimmed());
  });
}
