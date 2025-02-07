#include <assert.h>

#include "core/json.h"
#include "runtime/tools/reflection/reflection.h"
#include "runtime/tools/reflection/reflection_register.h"
#include "runtime/tools/serializer/serializer.h"
#include "_generated/reflection/all_reflection.h"
#include "_generated/serializer/all_serializer.ipp"

namespace Pupil {
    namespace Reflection {
        void TypeMetaRegister::metaUnregister() { TypeMetaRegisterinterface::unregisterAll(); }
    } 
}