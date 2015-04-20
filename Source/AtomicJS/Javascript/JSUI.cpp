#include <TurboBadger/tb_widgets.h>

#include <Atomic/UI/UIEvents.h>
#include <Atomic/UI/UIWidget.h>
#include <Atomic/UI/UIButton.h>
#include <Atomic/UI/UI.h>
#include "JSVM.h"
#include "JSUI.h"

using namespace tb;

namespace Atomic
{

JSUI::JSUI(Context* context) : Object(context)
{
    ctx_ = JSVM::GetJSVM(nullptr)->GetJSContext();
    SubscribeToEvent(E_WIDGETEVENT, HANDLER(JSUI, HandleWidgetEvent));
    SubscribeToEvent(E_WIDGETLOADED, HANDLER(JSUI, HandleWidgetLoaded));
    SubscribeToEvent(E_POPUPMENUSELECT, HANDLER(JSUI, HandlePopupMenuSelect));
}

JSUI::~JSUI()
{

}

void JSUI::GatherWidgets(tb::TBWidget* widget, PODVector<tb::TBWidget*>& widgets)
{
    if (widget->GetID() != TBID())
        widgets.Push(widget);

    for (TBWidget *n = widget->GetFirstChild(); n; n = n->GetNext())
    {
        GatherWidgets(n, widgets);
    }

}

void JSUI::HandleWidgetLoaded(StringHash eventType, VariantMap& eventData)
{
    using namespace WidgetLoaded;

    UIWidget* widget = static_cast<UIWidget*>(eventData[P_WIDGET].GetPtr());
    if (!widget)
        return;

    void* heapptr = widget->JSGetHeapPtr();

    if (!heapptr)
        return;

    // a loaded widget recursively gathers children which have id's and
    // stashes them in an internal array, so that they don't go out of scope
    // for instance var button = window.getWidgetByID("reveal");
    // if we didn't stash, any callback on button wouldn't work if button went out
    // of scope, which isn't expected behavior (in JS you would think the button is
    // in some way attached to the window object)

    TBWidget* tbwidget = widget->GetInternalWidget();
    assert(tbwidget);

    PODVector<TBWidget*> widgets;
    GatherWidgets(tbwidget, widgets);

    UI* ui = GetSubsystem<UI>();

    duk_push_heapptr(ctx_, heapptr);
    duk_push_array(ctx_);

    unsigned arrayCount = 0;
    for (unsigned i = 0; i < widgets.Size(); i++)
    {
        UIWidget* o =  ui->WrapWidget(widgets.At(i));

        if (!o)
            continue;

        js_push_class_object_instance(ctx_, o);

        duk_put_prop_index(ctx_, -2, arrayCount++);
    }

    duk_put_prop_string(ctx_, -2, "__child_widgets");
    duk_pop(ctx_);

}

void JSUI::HandlePopupMenuSelect(StringHash eventType, VariantMap& eventData)
{
    using namespace PopupMenuSelect;

    UIButton* button = static_cast<UIButton*>(eventData[P_BUTTON].GetPtr());
    if (!button)
        return;

    void* heapptr = button->JSGetHeapPtr();

    if (!heapptr) // gc'd
        return;

    int top = duk_get_top(ctx_);

    duk_push_heapptr(ctx_, heapptr);
    duk_get_prop_string(ctx_, -1, "__popup_menu_callback");
    if (!duk_is_callable(ctx_, -1))
    {
        duk_pop_n(ctx_, 2);
        assert(top == duk_get_top(ctx_));
        return;
    }

    UI* ui = GetSubsystem<UI>();
    String id;
    ui->GetTBIDString(eventData[P_REFID].GetUInt(), id);

    duk_push_string(ctx_, id.CString());
    duk_insert(ctx_, -1);

    if (duk_pcall(ctx_, 1) != 0)
    {
        JSVM::GetJSVM(nullptr)->SendJSErrorEvent();
    }

    duk_pop_n(ctx_, 2);

    assert(top == duk_get_top(ctx_));

}

void JSUI::PushWidgetEventObject(VariantMap& eventData)
{

    UI* ui = GetSubsystem<UI>();

    using namespace WidgetEvent;

    // create the event object
    duk_push_object(ctx_);

    // target
    UIWidget* target = static_cast<UIWidget*>(eventData[P_TARGET].GetPtr());

    if (target)
    {
        assert(target->JSGetHeapPtr());
        duk_push_heapptr(ctx_, target->JSGetHeapPtr());
    }
    else
    {
        duk_push_null(ctx_);
    }

    duk_put_prop_string(ctx_, -2, "target");

    duk_push_number(ctx_, (duk_double_t) eventData[P_TYPE].GetUInt());
    duk_put_prop_string(ctx_, -2, "type");

    duk_push_number(ctx_, (duk_double_t) eventData[P_X].GetInt());
    duk_put_prop_string(ctx_, -2, "x");

    duk_push_number(ctx_, (duk_double_t) eventData[P_Y].GetInt());
    duk_put_prop_string(ctx_, -2, "y");

    duk_push_number(ctx_, (duk_double_t) eventData[P_DELTAX].GetInt());
    duk_put_prop_string(ctx_, -2, "deltaX");

    duk_push_number(ctx_, (duk_double_t) eventData[P_DELTAX].GetInt());
    duk_put_prop_string(ctx_, -2, "deltaY");

    duk_push_number(ctx_, (duk_double_t) eventData[P_COUNT].GetInt());
    duk_put_prop_string(ctx_, -2, "count");

    duk_push_number(ctx_, (duk_double_t) eventData[P_KEY].GetInt());
    duk_put_prop_string(ctx_, -2, "key");

    duk_push_number(ctx_, (duk_double_t) eventData[P_SPECIALKEY].GetInt());
    duk_put_prop_string(ctx_, -2, "specialKey");

    duk_push_number(ctx_, (duk_double_t) eventData[P_MODIFIERKEYS].GetInt());
    duk_put_prop_string(ctx_, -2, "modifierKeys");

    duk_push_number(ctx_, (duk_double_t) eventData[P_MODIFIERKEYS].GetInt());
    duk_put_prop_string(ctx_, -2, "modifierKeys");

    String id;
    unsigned blah = eventData[P_REFID].GetUInt();
    ui->GetTBIDString(eventData[P_REFID].GetUInt(), id);
    duk_push_string(ctx_, id.CString() );
    duk_put_prop_string(ctx_, -2, "refID");

    duk_push_boolean(ctx_,eventData[P_TOUCH].GetBool() ? 1 : 0);
    duk_put_prop_string(ctx_, -2, "touch");

}

void JSUI::HandleWidgetEvent(StringHash eventType, VariantMap& eventData)
{
    using namespace WidgetEvent;

    UIWidget* handler = static_cast<UIWidget*>(eventData[P_HANDLER].GetPtr());
    UIWidget* target = static_cast<UIWidget*>(eventData[P_TARGET].GetPtr());

    if (!handler)
        return;

    void* handlerHeapPtr = handler->JSGetHeapPtr();

    if (!handlerHeapPtr)
        return;

    // if we have a target, however no corresponding JS object return
    if (target && !target->JSGetHeapPtr())
        return;

    tb::EVENT_TYPE type = (tb::EVENT_TYPE) eventData[P_TYPE].GetUInt();

    // general event handler, bubbles to (wrapped) parent widgets unless handled (returns true)
    if (type == tb::EVENT_TYPE_CLICK)
    {
        int top = duk_get_top(ctx_);
        duk_push_heapptr(ctx_, handlerHeapPtr);
        duk_get_prop_string(ctx_, -1, "onEvent");
        if (duk_is_callable(ctx_, -1)) {

            PushWidgetEventObject(eventData);

            duk_call(ctx_, 1);
        }
        duk_pop_n(ctx_, 2);
        assert(top == duk_get_top(ctx_));
    }

    // specific event handlers

    if (type == tb::EVENT_TYPE_CLICK)
    {
        int top = duk_get_top(ctx_);
        duk_push_heapptr(ctx_, handlerHeapPtr);
        duk_get_prop_string(ctx_, -1, "onClick");
        if (duk_is_callable(ctx_, -1)) {
            duk_call(ctx_, 0);
        }
        duk_pop_n(ctx_, 2);
        assert(top == duk_get_top(ctx_));
        return;
    }

}

}