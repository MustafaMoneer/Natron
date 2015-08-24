//  Natron
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
/*
 * Created by Alexandre GAUTHIER-FOICHAT on 6/1/2012.
 * contact: immarespond at gmail dot com
 *
 */
// from <https://docs.python.org/3/c-api/intro.html#include-files>:
// "Since Python may define some pre-processor definitions which affect the standard headers on some systems, you must include Python.h before any standard headers are included."
#include <Python.h>

#include "Group_KnobGui.h"

#include <cfloat>
#include <algorithm> // min, max

CLANG_DIAG_OFF(deprecated)
CLANG_DIAG_OFF(uninitialized)
#include <QGridLayout>
#include <QHBoxLayout>
#include <QStyle>
#include <QColorDialog>
#include <QToolTip>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QHeaderView>
#include <QApplication>
#include <QScrollArea>
GCC_DIAG_UNUSED_PRIVATE_FIELD_OFF
// /opt/local/include/QtGui/qmime.h:119:10: warning: private field 'type' is not used [-Wunused-private-field]
#include <QKeyEvent>
GCC_DIAG_UNUSED_PRIVATE_FIELD_ON
#include <QDebug>
#include <QFontComboBox>
#include <QDialogButtonBox>
CLANG_DIAG_ON(deprecated)
CLANG_DIAG_ON(uninitialized)

#include "Engine/Image.h"
#include "Engine/KnobTypes.h"
#include "Engine/Lut.h"
#include "Engine/Node.h"
#include "Engine/Project.h"
#include "Engine/Settings.h"
#include "Engine/TimeLine.h"

#include "Gui/Button.h"
#include "Gui/ClickableLabel.h"
#include "Gui/ComboBox.h"
#include "Gui/CurveGui.h"
#include "Gui/DockablePanel.h"
#include "Gui/GroupBoxLabel.h"
#include "Gui/Gui.h"
#include "Gui/GuiApplicationManager.h"
#include "Gui/GuiMacros.h"
#include "Gui/KnobUndoCommand.h"
#include "Gui/Label.h"
#include "Gui/NewLayerDialog.h"
#include "Gui/ProjectGui.h"
#include "Gui/ScaleSliderQWidget.h"
#include "Gui/SpinBox.h"
#include "Gui/TabGroup.h"
#include "Gui/Utils.h"

#include "ofxNatron.h"


using namespace Natron;
using std::make_pair;


//=============================GROUP_KNOB_GUI===================================
GroupBoxLabel::GroupBoxLabel(QWidget *parent)
: Natron::Label(parent)
, _checked(false)

{
    QObject::connect( this, SIGNAL( checked(bool) ), this, SLOT( setChecked(bool) ) );
}

void
GroupBoxLabel::setChecked(bool b)
{
    _checked = b;
    QPixmap pix;
    if (b) {
        appPTR->getIcon(NATRON_PIXMAP_GROUPBOX_UNFOLDED, &pix);
    } else {
        appPTR->getIcon(NATRON_PIXMAP_GROUPBOX_FOLDED, &pix);
    }
    setPixmap(pix);
}

Group_KnobGui::Group_KnobGui(boost::shared_ptr<KnobI> knob,
                             DockablePanel *container)
: KnobGui(knob, container)
, _checked(false)
, _button(0)
, _children()
, _childrenToEnable()
, _tabGroup(0)
, _knob( boost::dynamic_pointer_cast<Group_Knob>(knob))
{

}

Group_KnobGui::~Group_KnobGui()
{
    
}

TabGroup*
Group_KnobGui::getOrCreateTabWidget()
{
    if (_tabGroup) {
        return _tabGroup;
    }
    
    _tabGroup = new TabGroup(getContainer());
    return _tabGroup;
}

void
Group_KnobGui::removeTabWidget()
{
    delete _tabGroup;
    _tabGroup = 0;
}

void Group_KnobGui::removeSpecificGui()
{
    delete _button;
//    for (std::list<KnobGui*>::iterator it = _children.begin() ; it != _children.end(); ++it) {
//        (*it)->removeSpecificGui();
//        delete *it;
//    }
}

void
Group_KnobGui::addKnob(KnobGui *child)
{
    _children.push_back(child);
}

bool
Group_KnobGui::isChecked() const
{
    return hasWidgetBeenCreated() ? _button->isChecked() : true;
}

void
Group_KnobGui::createWidget(QHBoxLayout* layout)
{
    _button = new GroupBoxLabel( layout->parentWidget() );
    if ( hasToolTip() ) {
        _button->setToolTip( toolTip() );
    }
    _button->setFixedSize(NATRON_MEDIUM_BUTTON_SIZE, NATRON_MEDIUM_BUTTON_SIZE);
    _button->setChecked(_checked);
    QObject::connect( _button, SIGNAL( checked(bool) ), this, SLOT( setChecked(bool) ) );
    layout->addWidget(_button);
}

void
Group_KnobGui::setChecked(bool b)
{
    if (b == _checked) {
        return;
    }
    _checked = b;

    ///get the current index of the group knob in the layout, and reinsert
    ///the children back with an offset relative to the group.
    int realIndexInLayout = getActualIndexInLayout();
    int startChildIndex = realIndexInLayout + 1;
    
    for (std::list<KnobGui*>::iterator it = _children.begin(); it != _children.end(); ++it) {
        if (!b) {
            (*it)->hide();
        } else if ( !(*it)->getKnob()->getIsSecret() ) {
            (*it)->show(startChildIndex);
            if ( (*it)->getKnob()->isNewLineActivated() ) {
                ++startChildIndex;
            }
        }
    }
}

bool
Group_KnobGui::eventFilter(QObject */*target*/,
                           QEvent* /*event*/)
{
    //if(e->type() == QEvent::Paint){

    ///discard the paint event
    return true;
    // }
    //return QObject::eventFilter(target, event);
}

void
Group_KnobGui::updateGUI(int /*dimension*/)
{
    bool b = _knob.lock()->getGuiValue(0);

    setChecked(b);
    if (_button) {
        _button->setChecked(b);
    }
}

void
Group_KnobGui::_hide()
{
    if (_button) {
        _button->hide();
    }
    for (std::list<KnobGui*>::iterator it = _children.begin(); it != _children.end(); ++it) {
        (*it)->hide();
    }
}

void
Group_KnobGui::_show()
{
//    if ( _knob->getIsSecret() ) {
//        return;
//    }
    if (_button) {
        _button->show();
    }

    if (_checked) {
        for (std::list<KnobGui*>::iterator it = _children.begin(); it != _children.end(); ++it) {
            (*it)->show();
        }
    }
}

void
Group_KnobGui::setEnabled()
{
    boost::shared_ptr<Group_Knob> knob = _knob.lock();
    bool enabled = knob->isEnabled(0)  && !knob->isSlave(0) && knob->getExpression(0).empty();

    if (_button) {
        _button->setEnabled(enabled);
    }
    if (enabled) {
        for (U32 i = 0; i < _childrenToEnable.size(); ++i) {
            for (U32 j = 0; j < _childrenToEnable[i].second.size(); ++j) {
                _childrenToEnable[i].first->getKnob()->setEnabled(_childrenToEnable[i].second[j], true);
            }
        }
    } else {
        _childrenToEnable.clear();
        for (std::list<KnobGui*>::iterator it = _children.begin(); it != _children.end(); ++it) {
            std::vector<int> dimensions;
            for (int j = 0; j < (*it)->getKnob()->getDimension(); ++j) {
                if ( (*it)->getKnob()->isEnabled(j) ) {
                    (*it)->getKnob()->setEnabled(j, false);
                    dimensions.push_back(j);
                }
            }
            _childrenToEnable.push_back( std::make_pair(*it, dimensions) );
        }
    }
}

boost::shared_ptr<KnobI> Group_KnobGui::getKnob() const
{
    return _knob.lock();
}

