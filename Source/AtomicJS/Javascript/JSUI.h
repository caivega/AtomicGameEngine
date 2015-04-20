
#pragma once

#include <Duktape/duktape.h>
#include <Atomic/Core/Object.h>

namespace tb
{
class TBWidget;
}

namespace Atomic
{

class JSUI : public Object
{
    OBJECT(JSUI)

public:

    JSUI(Context* context);
    virtual ~JSUI();

private:

    duk_context* ctx_;

    void PushWidgetEventObject(VariantMap& eventData);

    void HandleWidgetEvent(StringHash eventType, VariantMap& eventData);
    void HandleWidgetLoaded(StringHash eventType, VariantMap& eventData);
    void HandlePopupMenuSelect(StringHash eventType, VariantMap& eventData);

    void GatherWidgets(tb::TBWidget* widget, PODVector<tb::TBWidget*>& widgets);

};

}