//  Powiter
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
/*
*Created by Alexandre GAUTHIER-FOICHAT on 6/1/2012. 
*contact: immarespond at gmail dot com
*
*/

#include "Timeline.h"

#include <QtGui/QPainter>
#if QT_VERSION < 0x050000
CLANG_DIAG_OFF(unused-private-field);
#include <QtGui/qmime.h>
CLANG_DIAG_ON(unused-private-field);
#endif
#include <QtGui/QMouseEvent>

#include "Global/Macros.h"
#include "Global/GlobalDefines.h"

#include "Engine/Node.h"

#include "Gui/ViewerTab.h"

#define UNITS_PIXELS_SPACING 3

using namespace std;
using namespace Powiter;


TimeLineGui::TimeLineGui(TimeLine& timeline,ViewerTab* parentTab):
QWidget(parentTab),
_first(0),
_last(100),
_alphaCursor(false),
_state(IDLE),
_timeLine(timeline),
_viewerTab(parentTab),
_cursorColor(243,149,0),
_boundsColor(207,69,6),
_cachedLineColor(143,201,103),
_backgroundColor(50,50,50),
_ticksColor(200,200,200),
_scaleColor(100,100,100)
{
    
    QObject::connect(&timeline, SIGNAL(frameChanged(int)), this, SLOT(seek(int)));
    setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    setMouseTracking(true);
        
}

QSize TimeLineGui::minimumSizeHint() const{
    return QSize(500,30);
}
QSize TimeLineGui::sizeHint() const{
    return QSize(500,30);
}

void TimeLineGui::updateScale(){
    _values.clear();
    _displayedValues.clear();
    for (int i = _timeLine.firstFrame(); i <= _timeLine.lastFrame(); ++i) {
        _values.push_back(i);
    }
    
    int averageUnitTextWidth = (fontMetrics().width(QString::number(_timeLine.firstFrame())) +
                            fontMetrics().width(QString::number(_timeLine.lastFrame())))/2;
    
    /*The maximum number of diplayed units is the width of the widget minus 2 (because
     we always display the first and last units) divided by the average unit text width.
     This number indicates for example between 0 and 120 how many units we should display
     apart from 0 and 120.*/
    int maximumDisplayedUnits = (sizeHint().width()- 2*averageUnitTextWidth - 2*BORDER_OFFSET_)/ averageUnitTextWidth;
    
    _displayedValues << _timeLine.firstFrame();
    _increment = (_timeLine.lastFrame() - _timeLine.firstFrame() +1)/(maximumDisplayedUnits + 1);
    if(_increment == 0){
        _increment = 1;
    }else{
        while((_timeLine.firstFrame()+_increment)%5!=0) ++_increment;
    }
    
    int c = _timeLine.firstFrame() + _increment;
    while (c < _timeLine.lastFrame()) {
        _displayedValues << c;
        c+=_increment;
    }
    _displayedValues << _timeLine.lastFrame();
    
    _XValues.clear();
    double _Xincrement = ((size().width()-BORDER_OFFSET_) - BORDER_OFFSET_ )/(double)(_values.size()-1);
    double xValue = BORDER_OFFSET_;
    for(unsigned int i=0;i<_values.size();++i) {
        _XValues.push_back(xValue);
        xValue+=_Xincrement;
    }
    
}
void TimeLineGui::paintEvent(QPaintEvent *){
    int w = size().width();
    int h = size().height();
    QPainter p(this);
    p.setWindow(0, 0, w, h);
    p.fillRect(0, 0, w, h,_backgroundColor );
    
    QFont f("Helvetica",10);
    p.setPen(_scaleColor);
    p.setFont(f);
    p.drawLine(BORDER_OFFSET_, LINE_START, BORDER_OFFSET_, LINE_START+BORDER_HEIGHT_); // left border
    p.drawLine(w-BORDER_OFFSET_,LINE_START,w-BORDER_OFFSET_,LINE_START+BORDER_HEIGHT_); // right border
    p.drawLine(BORDER_OFFSET_,BORDER_HEIGHT_+LINE_START,w-BORDER_OFFSET_,BORDER_HEIGHT_+LINE_START); // horizontal line
    updateScale();
    // draws ticks & sub-ticks
    drawTicks(&p,_scaleColor);
    QPolygonF cursorPoly;
    p.setPen(_cursorColor);
    double cursorX = getCoordPosition(_timeLine.currentFrame());
    double lastX = getCoordPosition(_last);
    double firstX = getCoordPosition(_first);
    
    if(_alphaCursor){
        double currentPos = _Mouse.x();
        QColor _currentColor(_cursorColor);
        _currentColor.setAlpha(100);
        p.setPen(_currentColor);
        QPolygonF currentPoly;
        currentPoly.push_back(QPointF(currentPos ,BORDER_HEIGHT_+LINE_START - 2));
        currentPoly.push_back(QPointF(currentPos - CURSOR_WIDTH/2 ,BORDER_HEIGHT_+LINE_START - 2 - CURSOR_HEIGHT));
        currentPoly.push_back(QPointF(currentPos + CURSOR_WIDTH/2,BORDER_HEIGHT_+LINE_START -2 -CURSOR_HEIGHT));
        QString mouseNumber(QString::number(getScalePosition(_Mouse.x())));
        QPointF mouseNbTextPos(currentPos,BORDER_HEIGHT_+LINE_START-CURSOR_HEIGHT-5);
        int offset = mouseNumber.size()*2.5;
        p.drawText(QPointF(mouseNbTextPos.x()-offset,mouseNbTextPos.y()), mouseNumber,'g',3);
        
        p.setRenderHint(QPainter::Antialiasing);

        QPainterPath currentPath_;
        currentPath_.addPolygon(currentPoly);
        currentPath_.closeSubpath();
        p.drawPath(currentPath_);
        p.fillPath(currentPath_, _currentColor);
        p.setRenderHint(QPainter::Antialiasing,false);
        p.setPen(_cursorColor);
    }
    
    cursorPoly.push_back(QPointF(cursorX ,BORDER_HEIGHT_+LINE_START - 2));
    cursorPoly.push_back(QPointF(cursorX - CURSOR_WIDTH/2 ,BORDER_HEIGHT_+LINE_START - 2 - CURSOR_HEIGHT));
    cursorPoly.push_back(QPointF(cursorX + CURSOR_WIDTH/2,BORDER_HEIGHT_+LINE_START -2 -CURSOR_HEIGHT));
    QString curNumber(QString::number(_timeLine.currentFrame()));
    QPointF textPos(cursorX,BORDER_HEIGHT_+LINE_START-CURSOR_HEIGHT-5);
    int textOffset = curNumber.size()*2.5;
    p.drawText(QPointF(textPos.x()-textOffset,textPos.y()), curNumber,'g',3);
    p.setPen(_boundsColor);
    QString firstNumber(QString::number(_first));
    QPointF firstPos(firstX,BORDER_HEIGHT_+LINE_START-CURSOR_HEIGHT-5);
    textOffset = firstNumber.size()*2.5;
    p.drawText(QPointF(firstPos.x()-textOffset,firstPos.y()), firstNumber,'g',3);
    QString lastNumber(QString::number(_last));
    QPointF lastPos(lastX,BORDER_HEIGHT_+LINE_START-CURSOR_HEIGHT-5);
    textOffset = lastNumber.size()*2.5;
    p.drawText(QPointF(lastPos.x()-textOffset,lastPos.y()), lastNumber,'g',3);
    QPainterPath cursor;
    p.setRenderHint(QPainter::Antialiasing);
    cursor.addPolygon(cursorPoly);
    cursor.closeSubpath();
    p.setPen(_cursorColor);
    p.drawPath(cursor);
    p.fillPath(cursor, _cursorColor);
    
    
    QPolygonF lastPoly,firstPoly;
    lastPoly.push_back(QPointF(lastX,BORDER_HEIGHT_+LINE_START));
    lastPoly.push_back(QPointF(lastX,BORDER_HEIGHT_+LINE_START  - CURSOR_HEIGHT));
    lastPoly.push_back(QPointF(lastX-CURSOR_HEIGHT,BORDER_HEIGHT_+LINE_START ));
    firstPoly.push_back(QPointF(firstX,BORDER_HEIGHT_+LINE_START));
    firstPoly.push_back(QPointF(firstX,BORDER_HEIGHT_+LINE_START  - CURSOR_HEIGHT));
    firstPoly.push_back(QPointF(firstX+CURSOR_HEIGHT,BORDER_HEIGHT_+LINE_START ));
    QPainterPath firstP,lastP;
    firstP.addPolygon(firstPoly);
    firstP.closeSubpath();
    lastP.addPolygon(lastPoly);
    lastP.closeSubpath();
    p.setPen(_boundsColor);
    p.drawPath(firstP);
    p.drawPath(lastP);
    p.fillPath(firstP,_boundsColor);
    p.fillPath(lastP, _boundsColor);
    
    
    /*drawing cached line*/
    p.setPen(_cachedLineColor);
    for(U32 i =0 ; i < _cached.size() ; ++i) {
        double pos = getCoordPosition(_cached[i]);
        double previousOrNext;
        if (_cached[i] == _timeLine.firstFrame()) {
            previousOrNext = getCoordPosition(_cached[i]+1);
        }else{
            previousOrNext = getCoordPosition(_cached[i]-1);
        }
        double width = pos - previousOrNext;
        p.drawLine(pos-width/2.0,BORDER_HEIGHT_+LINE_START+2,pos+width/2.0,BORDER_HEIGHT_+LINE_START+2);
        p.drawLine(pos-width/2.0,BORDER_HEIGHT_+LINE_START+3,pos+width/2.0,BORDER_HEIGHT_+LINE_START+3);
        p.drawLine(pos-width/2.0,BORDER_HEIGHT_+LINE_START+4,pos+width/2.0,BORDER_HEIGHT_+LINE_START+4);
    }
}
void TimeLineGui::drawTicks(QPainter *p,QColor& scaleColor){
    int w = size().width();
    double scaleW = w-2*BORDER_OFFSET_ ;
    double incr=((scaleW)/(double)(_displayedValues.size()-1))/(double)_increment;
    for(int i =0;i<_displayedValues.size();++i) {
        int x = getCoordPosition(_displayedValues[i]);
        int y = LINE_START;
        p->setPen(_ticksColor);
        QString nbStr = QString::number(_displayedValues[i]);
        int offset = nbStr.size()*2;
        if(_displayedValues[i] != _first
           && _displayedValues[i] != _last
           && _timeLine.currentFrame() != _displayedValues[i])
            p->drawText(QPointF(x-offset,y), nbStr,'g',3);
        p->setPen(scaleColor);
        if(i != _displayedValues.size()-1){
            for(int j=1;j<_increment;++j) {
                p->drawLine(x+j*incr,2+BORDER_HEIGHT_+LINE_START,x+j*incr,BORDER_HEIGHT_+LINE_START-TICK_HEIGHT_/2);
            }
        }
        
        if(i>0 && i<_displayedValues.size()-1){
            p->drawLine(x,2+BORDER_HEIGHT_+LINE_START,x,2+BORDER_HEIGHT_+LINE_START-TICK_HEIGHT_);
        }
        //  pos.setX(pos.x()+incr*_increment);
    }
    
}

double TimeLineGui::getScalePosition(double pos){
    if(pos <= _XValues [0])
        return _values[0];
    if(pos >= _XValues[_XValues.size()-1])
        return _values[_XValues.size()-1];
    for(unsigned int i=0;i<_values.size();++i) {
        if(i<_XValues.size()-1 &&  pos >= _XValues[i] && pos < _XValues[i+1]){
            return _values[i];
        }
        if(i == _XValues.size()-1 ){
            return  _values[i];
            
        }
    }
    return -1;

}
double TimeLineGui::getCoordPosition(double pos){
    if(pos < _values[0]){
        return _XValues[0];
    }
    if(pos > _values[_values.size()-1]){
        return _XValues[_XValues.size()-1];
    }
    for(unsigned int i=0;i<_values.size();++i) {
        if(i<_values.size()-1 &&  pos >= _values[i] && pos < _values[i+1]){
            return  _XValues[i];
        }
        if(i == _values.size()-1 ){
            return  _XValues[i];
        }
    }
    return -1;
}
void TimeLineGui::changeFirst(int v){
    _first = v;
    repaint();
}
void TimeLineGui::changeLast(int v){
    _last = v;
    repaint();
}


void TimeLineGui::seek(int v){
    if(v >=_first && v<=_last)
        _timeLine.seek_noEmit(v);
    emit currentFrameChanged(v);
    repaint();
}

void TimeLineGui::seek_notSlot(int v){
    if(v >=_first && v<=_last)
        _timeLine.seek_noEmit(v);
    emit currentFrameChanged(v);
    QMetaObject::invokeMethod(this, "repaint", Qt::QueuedConnection);
}

void TimeLineGui::changeFirstAndLast(QString str){
    Q_UNUSED(str);
}

void TimeLineGui::mousePressEvent(QMouseEvent* e){
    
    int c = getScalePosition(e->x());
    if(e->modifiers().testFlag(Qt::ControlModifier)){
        _state = DRAGGING_BOUNDARY;
        int firstPos,lastPos;
        firstPos = getCoordPosition(_first);
        lastPos = getCoordPosition(_last);
        if(abs( e->x()-firstPos ) > abs(e->x() - lastPos) ){ // moving last frame anchor
            if( c >= _timeLine.firstFrame() && c<= _timeLine.lastFrame()){
                _last = c;
            }
        }else{ // moving first frame anchor
            if( c >= _timeLine.firstFrame() && c<= _timeLine.lastFrame()){
                _first = c;
            }
        }
            
    }else{
        _state = DRAGGING_CURSOR;
        if(c >= _first && c <=_last){
            _timeLine.seek_noEmit(c);
            emit positionChanged(_timeLine.currentFrame());
        }
    }
    
    repaint();
    QWidget::mousePressEvent(e);

}
void TimeLineGui::mouseMoveEvent(QMouseEvent* e){
    _alphaCursor=true;
    _Mouse = e->pos();
    if(_state==DRAGGING_CURSOR){
        int c = getScalePosition(e->x());
        if(c >= _first && c <=_last){
            _timeLine.seek_noEmit(c);
            emit positionChanged(_timeLine.currentFrame());
        }
    }else if(_state == DRAGGING_BOUNDARY){
        int c = getScalePosition(e->x());
        int firstPos,lastPos;
        firstPos = getCoordPosition(_first);
        lastPos = getCoordPosition(_last);
        if(abs( e->x()-firstPos ) > abs(e->x() - lastPos) ){ // moving last frame anchor
            if( c >= _timeLine.firstFrame() && c<= _timeLine.lastFrame() && c >= _first){
                _last = c;
            }
        }else{ // moving first frame anchor
            if( c >= _timeLine.firstFrame() && c<= _timeLine.lastFrame() && c<=_last){
                _first = c;
            }
        }

    }
    QWidget::mouseMoveEvent(e);
    repaint();

}
void TimeLineGui::enterEvent(QEvent* e){
    _alphaCursor = true;
   // grabMouse();
    QWidget::enterEvent(e);
};
void TimeLineGui::leaveEvent(QEvent* e){
    _alphaCursor = false;
    repaint();
   // releaseMouse();
    QWidget::leaveEvent(e);
}


void TimeLineGui::mouseReleaseEvent(QMouseEvent* e){
    _state = IDLE;
    QWidget::mouseReleaseEvent(e);
}


void TimeLineGui::addCachedFrame(int f){
    _cached.push_back(f);
 //  repaint();
}
void TimeLineGui::removeCachedFrame(){
    _cached.erase(_cached.begin());
   // repaint();
}

void TimeLineGui::setFrameRange(int min,int max){
    _timeLine.setFirstFrame(min);
    _timeLine.setLastFrame(max);
}
/*initialises the boundaries on the timeline*/
void TimeLineGui::setBoundaries(int first,int last){
    _first = first;
    _last = last;
    // repaint();
}
int TimeLineGui::currentFrame() const{return _timeLine.currentFrame();}
