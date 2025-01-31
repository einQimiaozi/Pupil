#pragma once

#include "_generated\serializer\primitive.serializer.gen.h"
namespace Pupil {
    template<>
    Json Serializer::write(const Vertex& instance) {
        Json::object  ret_context;
        
        ret_context.insert_or_assign("alpha", Serializer::write(instance.alpha));
        return  Json(ret_context);
    }
    template<>
    Vertex& Serializer::read(const Json& json_context, Vertex& instance) {
        assert(json_context.is_object());
        
        if(!json_context["alpha"].is_null()){
            Serializer::read(json_context["alpha"], instance.alpha);
        }
        return instance;
    }
}

