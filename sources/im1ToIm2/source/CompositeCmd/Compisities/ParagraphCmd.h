//
// Created by Safonov Vadim on 11/20/15.
//

#ifndef IM1TOIM2_PARAGRAPHCMD_H
#define IM1TOIM2_PARAGRAPHCMD_H

#include "../CompositeCmd.h"

class ParagraphCmd : public CompositeCmd{
public:
    ParagraphCmd(Json::Value processJson);

    virtual Json::Value toJson() override;
};


#endif //IM1TOIM2_PARAGRAPHCMD_H
