#include <assert.h>

#include "core/json.h"
#include "core/reflection/reflection.h"
#include "core/reflection/reflection_register.h"
#include "core/serializer/serializer.h"
#include "_generated/reflection/all_reflection.h"
#include "_generated/serializer/all_serializer.ipp"

namespace Pupil {
    namespace Reflection {
        void TypeMetaRegister::metaUnregister() { TypeMetaRegisterinterface::unregisterAll(); }
    } 
}