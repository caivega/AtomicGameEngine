// Copyright (c) 2014-2015, THUNDERBEAST GAMES LLC All rights reserved
// Please see LICENSE.md in repository root for license information
// https://github.com/AtomicGameEngine/AtomicGameEngine

#pragma once

#include <Atomic/Core/Object.h>

#include "Subprocess.h"

using namespace Atomic;

namespace ToolCore
{

EVENT(E_SUBPROCESSOUTPUT, SubprocessOutput)
{
    PARAM(P_TEXT, Text);      // string
}

EVENT(E_SUBPROCESSCOMPLETE, SubprocessComplete)
{
    PARAM(P_PROCESSKEY, ProcessKey); // unsigned
    PARAM(P_RETCODE, RetCode);      // int (return code of process)
}

class SubprocessSystem : public Object
{
    OBJECT(SubprocessSystem);

public:
    /// Construct.
    SubprocessSystem(Context* context);
    /// Destruct.
    virtual ~SubprocessSystem();


    Subprocess* Launch(const String& command, const Vector<String>& args, const String& initialDirectory = "");
    Subprocess* Launch(const String& command, const Vector<String>& args, const String& initialDirectory, const Poco::Process::Env& env);

private:

    void HandleUpdate(StringHash eventType, VariantMap& eventData);

    Vector<SharedPtr<Subprocess> > processes_;
    float updateTimer_;

};

}
