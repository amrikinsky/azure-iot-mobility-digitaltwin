// ---------------------------------------------------------------------------------
//  <copyright company="Microsoft">
//    Copyright (c) Microsoft Corporation. All rights reserved.
//  </copyright>
// ---------------------------------------------------------------------------------

#ifndef DTDL_COMPOSER_H
#define DTDL_COMPOSER_H

#include <jsoncpp/include/json/json.h>

#include "dtdl-definition.h"

namespace microsoft::automotive::data
{
    class DtdlComposer
    {
        public:
            void Compose(const dtdl::Definition& definition);

        protected:
            void ConvertDtdlComponentToJson(const dtdl::Component& component, std::list<std::shared_ptr<Json::Value>>& dtdlAsJson);
    };
} //namespace microsoft::automotive::data

#endif // DTDL_COMPOSER_H
