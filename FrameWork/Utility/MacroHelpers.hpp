#pragma once

#ifndef MACRO_HELPERS
#define MACRO_HELPERS

#define DEFINE_GETTER(type, name, member)\
const type& Get##name() const noexcept { return member; }\

#define DEFINE_SETTER(type, name, member)\
void Set##name(const type& value) noexcept { member = value; }\


#endif // !MACRO_HELPERS
#define DEFINE_PROPERTY(type, name, member) \
    DEFINE_GETTER(type, name, member) \
    DEFINE_SETTER(type, name, member)
