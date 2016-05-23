//
// Created by Safonov Vadim on 11/6/15.
//

#ifndef IM1TOIM2_PROCCMD_H
#define IM1TOIM2_PROCCMD_H


#include "../CompositeCmd.h"

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


#endif //IM1TOIM2_PROCCMD_H
