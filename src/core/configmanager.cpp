/**
 * Copyright (c) 2020 ChungZH. ALl Rights Reserved.
 * Licensed under the MIT license.
 * See file LICENSE for detail or copy at <https://opensource.org/licenses/MIT>
 *
 * This file is a part of Notepanda.
 *
 * @file configmanager.cpp
 * @brief This file implements the ConfigManager class.
 *
 */
#include "configmanager.h"
#include "../constant/constant.h"

#include <QDebug>
#include <QFontDatabase>
#include <QJsonDocument>

ConfigManager::ConfigManager(const QString &configuration, QObject *parent)
    : QObject(parent)
    , settings_(nullptr)
    , configFile_(configuration)
    , editorFontFamily_(DEFAULT_EDITOR_FONT_FAMILY)
    , styleTheme_(DEFAULT_EDITOR_STYLE_THEME)
    , editorFontSize_(DEFAULT_EDITOR_FONT_SIZE)
    , editorColorTheme_(DEFAULT_EDITOR_COLOR_THEME)
    , editorTabSize_(DEFAULT_EDITOR_TAB_SIZE)
    , editorIndentMode_(DEFAULT_EDITOR_INDENT_MODE)
{
    /*Use a custom format of config file to persistent with QSettings*/
    const QSettings::Format jsonFormat =
        QSettings::registerFormat("json", readJsonFile, writeJsonFile);
    settings_ = new QSettings(configFile_, jsonFormat, this);

    readGeneralSettings();

    // print all settings
    qDebug() << ""
             << "All settings:";
    for (auto i : settings_->allKeys()) {
        qDebug() << i << ":" << settings_->value(i);
    }
    qDebug() << "";
}

bool ConfigManager::readJsonFile(QIODevice &device, QSettings::SettingsMap &map)
{
    QJsonParseError error;
    map = QJsonDocument::fromJson(device.readAll(), &error).toVariant().toMap();
    return error.error == QJsonParseError::NoError;
}

bool ConfigManager::writeJsonFile(QIODevice &device,
                                  const QSettings::SettingsMap &map)
{
    const auto json = QJsonDocument::fromVariant(map).toJson();
    return device.write(json) == json.size();
}

/**
 * @brief Save the settings.
 */
void ConfigManager::save()
{
    settings_->beginGroup("Editor");
    settings_->setValue("FontFamily", QVariant(editorFontFamily_));
    settings_->setValue("FontSize", QVariant(editorFontSize_));
    settings_->setValue("ColorTheme", QVariant(editorColorTheme_));
    settings_->setValue("TabSize", QVariant(editorTabSize_));
    settings_->setValue("IndentMode", QVariant(editorIndentMode_));
    settings_->endGroup();
    settings_->setValue("styleTheme_", QVariant(styleTheme_));
}

/**
 * @brief Read general settings from `settings`.
 */
void ConfigManager::readGeneralSettings()
{
    if (settings_->contains("Editor/FontFamily")) {
        editorFontFamily_ = settings_->value("Editor/FontFamily").toString();
    } else {
        editorFontFamily_ =
            QFontDatabase::systemFont(QFontDatabase::FixedFont).toString();
    }
    styleTheme_ = settings_->value("styleTheme_", "Fusion").toString();
    editorFontSize_ = settings_->value("Editor/FontSize", 16).toInt();
    editorColorTheme_ =
        settings_->value("Editor/ColorTheme", "Default").toString();
    editorTabSize_ = settings_->value("Editor/TabSize", 4).toInt();
    editorIndentMode_ =
        settings_->value("Editor/IndentMode", "Spaces").toString();
}

QString ConfigManager::getEditorFontFamily() const { return editorFontFamily_; }
void ConfigManager::setEditorFontFamily(const QString &fontname)
{
    editorFontFamily_ = fontname;
}

QString ConfigManager::getStyleTheme() const { return styleTheme_; }
void ConfigManager::setStyleTheme(const QString &stylename)
{
    styleTheme_ = stylename;
}

int ConfigManager::getEditorFontSize() const { return editorFontSize_; }
void ConfigManager::setEditorFontSize(const int &fontsize)
{
    editorFontSize_ = fontsize;
}

QString ConfigManager::getEditorColorTheme() const { return editorColorTheme_; }
void ConfigManager::setEditorColorTheme(const QString &ctname)
{
    editorColorTheme_ = ctname;
}

int ConfigManager::getEditorTabSize() const { return editorTabSize_; }
void ConfigManager::setEditorTabSize(const int &tabsize)
{
    editorTabSize_ = tabsize;
}

QString ConfigManager::getEditorIndentMode() const { return editorIndentMode_; }
void ConfigManager::setEditorIndentMode(const QString &indentmode)
{
    editorIndentMode_ = indentmode;
}