/*
 * Copyright (C) 2019 Tianjin KYLIN Information Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/&gt;.
 *
 */
#include "ukmedia_slider_tip_label_helper.h"
#include <QWidget>
#include <QMouseEvent>
#include <xcb/xcb.h>
#include <QApplication>
#include <QStyleOption>
#include <QStyle>
#include <QDebug>

SliderTipLabelHelper::SliderTipLabelHelper(QObject *parent) :QObject(parent)
{
//    registerWidget(this);
    m_pTiplabel = new QLabel();
    m_pTiplabel->setWindowFlag(Qt::ToolTip);
    qApp->installEventFilter(new AppEventFilter(this));
    m_pTiplabel->setFixedSize(52,30);
    m_pTiplabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    m_pTiplabel->setStyleSheet("QLabel{background:rgba(26,26,26,0.7);"
                              "border:1px solid rgba(255, 255, 255, 0.2);"
                              "border-radius:6px;padding:7px}");
}


void SliderTipLabelHelper::registerWidget(QWidget *w)
{
    w->removeEventFilter(this);
    w->installEventFilter(this);
}

void SliderTipLabelHelper::unregisterWidget(QWidget *w)
{
    w->removeEventFilter(this);
}

bool SliderTipLabelHelper::eventFilter(QObject *obj, QEvent *e)
{
//    qDebug() << "obj type" << obj << "this type" << this;
    auto slider = qobject_cast<UkmediaVolumeSlider *>(obj);
    if (obj == slider) {
//    qDebug() << e->type();
    switch (e->type()) {
    case QEvent::MouseMove: {
        QMouseEvent *event = static_cast<QMouseEvent*>(e);
        mouseMoveEvent(obj, event);
        qDebug()<<"move";
        return false;
    }
    case QEvent::MouseButtonRelease: {
        qDebug()<<"release";
        QMouseEvent *event = static_cast<QMouseEvent*>(e);
        mouseReleaseEvent(obj, event);
        return false;
    }
    case QEvent::MouseButtonPress:{
        qDebug()<<"press";
        QMouseEvent *event = static_cast<QMouseEvent*>(e);
        mousePressedEvent(obj,event);
    }
    case QEvent::Move: {

    }
    default:
        return false;
    }
    }
    return QObject::eventFilter(obj,e);
}

void SliderTipLabelHelper::buttonPresseEvent(QObject *obj, QMouseEvent *e)
{

}

void SliderTipLabelHelper::mouseMoveEvent(QObject *obj, QMouseEvent *e)
{
    QRect rect;
    QStyleOptionSlider m_option;
    auto slider = qobject_cast<UkmediaVolumeSlider *>(obj);
    slider->initStyleOption(&m_option);
    rect = slider->style()->subControlRect(QStyle::CC_Slider, &m_option,QStyle::SC_SliderHandle,slider);
    QPoint gPos = slider->mapToGlobal(rect.topLeft());
    QPoint lPos = slider->mapToParent(rect.topLeft());
    qDebug() << "鼠标滑动滑块位置" <<  slider->rect() << "滑动条映射到全局坐标" << gPos << "滑动条映射本地位置" << lPos;
    //m_Tiplabel->move(gPos);
    qDebug() << m_option.rect << m_option.activeSubControls<< m_option.sliderValue << m_option.sliderPosition;
    QString percent;
    percent = QString::number(m_option.sliderValue);
    percent.append("%");
    m_pTiplabel->setText(percent);
    m_pTiplabel->move(gPos.x()-(m_pTiplabel->width()/2)+9,gPos.y()-m_pTiplabel->height()-1);
    m_pTiplabel->show();
}

void SliderTipLabelHelper::mouseReleaseEvent(QObject *obj, QMouseEvent *e)
{
    m_pTiplabel->hide();
}

void SliderTipLabelHelper::mousePressedEvent(QObject *obj, QMouseEvent *e)
{
    QRect rect;
    QStyleOptionSlider m_option;
    auto slider = qobject_cast<UkmediaVolumeSlider *>(obj);
    slider->initStyleOption(&m_option);
//    m_option.initFrom(slider);
//    m_option.init(slider);
    rect = slider->style()->subControlRect(QStyle::CC_Slider, &m_option,QStyle::SC_SliderHandle,slider);
    QPoint gPos = slider->mapToGlobal(rect.topLeft());
    QPoint lPos = slider->mapToParent(rect.topLeft());
    qDebug() << "鼠标按下滑块位置" <<  slider->rect() << "滑动条映射到全局坐标" << gPos << "滑动条映射本地位置" << lPos;
    //m_Tiplabel->move(gPos);
    qDebug()<<m_option.rect<<m_option.activeSubControls<<m_option.sliderValue<<m_option.sliderPosition;
    QString percent;
    percent = QString::number(m_option.sliderValue);
    percent.append("%");
    m_pTiplabel->setText(percent);
    m_pTiplabel->move(gPos.x()-(m_pTiplabel->width()/2)+9,gPos.y()-m_pTiplabel->height()-1);
    m_pTiplabel->show();
}

// AppEventFilter
AppEventFilter::AppEventFilter(SliderTipLabelHelper *parent) : QObject(parent)
{
    m_wm = parent;
}

bool AppEventFilter::eventFilter(QObject *obj, QEvent *e)
{
//    if (e->type() == QEvent::MouseButtonRelease) {

//        return true;
//    }

    return false;
}
