/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
 *
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QPaintEvent>
#include <QObject>
#include <QDebug>
#include <QDir>
#include <QStandardPaths>
#include <QScreen>
#include <QDateTime>
#include <QMouseEvent>
#include <QTimer>
#include <QPixmap>

#include "src/widgets/toptips.h"
#include "src/widgets/toolbar.h"
#include "src/widgets/zoomIndicator.h"
#include "src/widgets/shapeswidget.h"
#include "src/utils/baseutils.h"
#include "src/utils/shortcut.h"
#include "src/utils/configsettings.h"
#include "controller/menucontroller.h"

#include "dbusinterface/dbuscontrolcenter.h"
#include "dbusinterface/dbusnotify.h"
#include "dbusinterface/dbuszone.h"

#include <dscreenwindowsutil.h>

DWIDGET_USE_NAMESPACE
DWM_USE_NAMESPACE

class MainWindow : public QLabel
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    enum ShotMouseStatus {
        Normal,
        Wait,
        Shoting,
    };

    void initOriginUI();
    void initSecondUI();

    void initBackground();
    void initShapeWidget(QString type);
    void initDBusInterface();
    void initShortcut();

signals:
    void deleteShapes();
    void releaseEvent();
    void hideScreenshotUI();
    void unDo();
    void saveActionTriggered();
    void updateZoom();

public slots:
    void fullScreenshot();
    void savePath(const QString &path);
    void saveSpecificedPath(QString path);
//    void delayScreenshot(int num);
    void noNotify();
    void topWindow();
    void expressSaveScreenshot();
    //Indicate that this program's started by clicking desktop file.
     //void startByIcon();

    void startScreenshot();
    void shotFullScreen();
    void shotCurrentImg();
    void shotImgWidthEffect();
    void saveScreenshot();
    bool saveAction(const QPixmap &pix);
    void sendNotify(int saveIndex, QString saveFilePath, const bool succeed);
    void reloadImage(QString effect);
    void onViewShortcut();
    void onHelp();
    void exitApp();
    void updateToolBarPos();

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    int   getDirection(QEvent *event);
    void updateCursor(QEvent *event);
    void resizeDirection(ResizeDirection direction, QMouseEvent* e);

    void keyPressEvent(QKeyEvent *keyEvent) Q_DECL_OVERRIDE;
    void keyReleaseEvent(QKeyEvent *keyEvent) Q_DECL_OVERRIDE;

    void mouseDoubleClickEvent(QMouseEvent* ev) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent* ev) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *ev) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *ev) Q_DECL_OVERRIDE;
    void hideEvent(QHideEvent *event) Q_DECL_OVERRIDE;

private:
    QPixmap getPixmapofRect(const QRect &rect);

    QList<QRect> m_windowRects;
    QRect m_backgroundRect;
    QPixmap m_backgroundPixmap;
    QList<QString> m_windowNames;

    //SaveIndex indicate the save option(save to desktop, save to Picture dir,...)
    int m_saveIndex = 0;
    //m_saveFileName is the storage path of the screenshot image.
    QString m_saveFileName;

    int m_screenNum;
    int m_recordX;
    int m_recordY;
    int m_recordWidth;
    int m_recordHeight;

    int m_mouseStatus;
    int m_dragAction;

    int m_dragRecordHeight;
    int m_dragRecordWidth;
    int m_dragRecordX;
    int m_dragRecordY;
    int m_dragStartX;
    int m_dragStartY;

    bool m_needDrawSelectedPoint;
    bool m_drawNothing = false;
    bool m_isFirstDrag;
    bool m_isFirstMove;
    bool m_isFirstPressButton;
    bool m_isFirstReleaseButton;
    bool m_isPressButton;
    bool m_isReleaseButton;
    bool m_moving = false;
    bool m_isShiftPressed = false;
    bool m_noNotify = false;
    bool m_needSaveScreenshot = false;

    QString m_selectAreaName;
    QPixmap m_resizeBigPix;
    QPixmap m_resizeSmallPix;
    QPixmap m_resultPixmap;

    TopTips* m_sizeTips;
    ToolBar* m_toolBar;
    ZoomIndicator* m_zoomIndicator;
    ShapesWidget* m_shapesWidget;
    ConfigSettings* m_configSettings;

    bool m_isShapesWidgetExist = false;
    bool m_interfaceExist = false;

    QString m_specificedPath = "";
    MenuController* m_menuController;
    DBusControlCenter* m_controlCenterDBInterface;
    DBusNotify* m_notifyDBInterface;
    DBusZone* m_hotZoneInterface;
    DScreenWindowsUtil* m_swUtil;
};

#endif // MAINWINDOW_H
