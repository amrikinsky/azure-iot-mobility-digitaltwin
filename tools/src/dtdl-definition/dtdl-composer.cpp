// ---------------------------------------------------------------------------------
//  <copyright company="Microsoft">
//    Copyright (c) Microsoft Corporation. All rights reserved.
//  </copyright>
// ---------------------------------------------------------------------------------

#include <iostream>
#include "dtdl-composer.h"

using namespace microsoft::automotive::data;

void DtdlComposer::Compose(const dtdl::Definition& definition)
{
    // std::list<std::shared_ptr<Json::Value>>& dtdl);

    std::list<std::shared_ptr<Json::Value>> dtdlAsJson;

    ConvertDtdlComponentToJson(definition.Root, dtdlAsJson);

    std::cout << "[" << std::endl;

    for (auto it = dtdlAsJson.begin(); it != dtdlAsJson.end(); it++)
    {
            if (it != dtdlAsJson.begin())
            {
                    std::cout << "," << std::endl;
            }
            std::cout << **it;
    }
    std::cout << std::endl;
    std::cout << "]" << std::endl;
}

void DtdlComposer::ConvertDtdlComponentToJson(const dtdl::Component& component, std::list<std::shared_ptr<Json::Value>>& dtdlAsJson)
{
    std::shared_ptr<Json::Value> interfaceValue(new Json::Value());
    dtdlAsJson.push_back(interfaceValue);
    (*interfaceValue)["@type"] = "Interface";
    (*interfaceValue)["displayName"] = component.Name.c_str();
    (*interfaceValue)["@context"] = "dtmi:dtdl:context;2";
    // const Json::Value& contents = (*interfaceValue)["contents"]; // array of contents
    int i = 0;
    for (auto it = component.Telemetries.begin(); it != component.Telemetries.end(); it++)
    {
        (*interfaceValue)["contents"][i]["@type"] = "Telemetry";
        (*interfaceValue)["contents"][i]["name"] = (*it)->Name.c_str();
        (*interfaceValue)["contents"][i]["schena"] = (*it)->Schema.c_str();
        if (!(*it)->Unit.empty())
        {
            (*interfaceValue)["contents"][i]["unit"] = (*it)->Unit.c_str();
        }

        i++;

    }
    for (auto it = component.Properties.begin(); it != component.Properties.end(); it++)
    {
        (*interfaceValue)["contents"][i]["@type"] = "Property";
        (*interfaceValue)["contents"][i]["name"] = (*it)->Name.c_str();
        (*interfaceValue)["contents"][i]["schena"] = (*it)->Schema.c_str();
        (*interfaceValue)["contents"][i]["writable"] = (*it)->Writable;
        if (!(*it)->Unit.empty())
        {
            (*interfaceValue)["contents"][i]["unit"] = (*it)->Unit.c_str();
        }

        i++;

    }
    for (auto it = component.Components.begin(); it != component.Components.end(); it++)
    {
        ConvertDtdlComponentToJson(*(it->get()), dtdlAsJson);

        (*interfaceValue)["contents"][i]["@type"] = "Component";
        (*interfaceValue)["contents"][i]["name"] = (*it)->Name.c_str();
        i++;
    }
}
