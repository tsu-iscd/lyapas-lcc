#pragma once

#include "CompositeCmd.h"

class ProcedureCmd : public CompositeCmd {
public:
    enum class Type {
        definition, call
    };

    ProcedureCmd(Json::Value processJson, Type type);

    virtual Json::Value toJson() override;
private:
    Json::Value buildSignature();

    Type _type;
};
