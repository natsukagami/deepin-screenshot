/*
 * Maintainer: Peng Hui<penghui@deepin.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "configsettings.h"

#include <QDir>
#include <QFileInfo>
#include <QApplication>
#include <QTemporaryFile>
#include <QDebug>

ConfigSettings::ConfigSettings(QObject *parent)
    : QObject(parent)
{
    m_settings = new QSettings("deepin", "deepin-screenshot");

    if (m_settings->allKeys().isEmpty()) {
        setValue("common", "color_index", 3);
        setValue ("common", "default_savepath", "");

        setValue("arrow", "color_index", 3);
        setValue("arrow", "arrow_linewidth_index", 1);
        setValue("arrow", "straightline_linewidth_index", 1);
        setValue("arrow", "is_straight", false);
        setValue("oval", "color_index", 3);
        setValue("oval", "linewidth_index", 1);
        setValue("line", "color_index", 3);
        setValue("line", "linewidth_index", 1);
        setValue("rectangle", "color_index", 3);
        setValue("rectangle", "linewidth_index", 1);
        setValue("text", "color_index", 3);
        setValue("text", "fontsize", 12);

        setValue("save", "save_op", 0);
        setValue("save", "save_quality", 100);
    }

    setValue("effect", "is_blur", false);
    setValue("effect", "is_mosaic", false);

    qDebug() << "Setting file:" << m_settings->fileName();
}

ConfigSettings* ConfigSettings::m_configSettings = nullptr;
ConfigSettings* ConfigSettings::instance() {
    if (!m_configSettings) {
        m_configSettings = new ConfigSettings();
    }

    return m_configSettings;
}

void ConfigSettings::setTemporarySaveAction(int save_op)
{
    m_temporary_save_op = save_op;
}

bool ConfigSettings::hasTemporarySaveAction()
{
    return m_temporary_save_op >= 0;
}

int ConfigSettings::getAndResetTemporarySaveAction()
{
    int ret = m_temporary_save_op;
    m_temporary_save_op = -1;

    return ret;
}

void ConfigSettings::setValue(const QString &group, const QString &key,
                              QVariant val) {
    m_settings->beginGroup(group);
    m_settings->setValue(key, val);
    m_settings->endGroup();
    m_settings->sync();

    if (val.type() == QVariant::Int) {
        emit shapeConfigChanged(group, key, val.toInt());
    }

    if (group == "arrow" && key == "is_straight") {
        emit straightLineConfigChanged(val.toBool());
    }

    qDebug() << "ConfigSettings:" << group << key << val;
}

QVariant ConfigSettings::value(const QString &group, const QString &key,
                               const QVariant &defaultValue) {
    QMutexLocker locker(&m_mutex);

    QVariant value;
    m_settings->beginGroup(group);
    value = m_settings->value(key, defaultValue);
    m_settings->endGroup();

    return value;
}

QStringList ConfigSettings::keys(const QString &group) {
    QStringList v;
    m_settings->beginGroup(group);
    v = m_settings->childKeys();
    m_settings->endGroup();

    return v;
}

ConfigSettings::~ConfigSettings() {
}
