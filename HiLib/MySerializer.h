#pragma once
#include "MyMPL.h"
//HAS_MEMBER_FUNCTION1(serialize)
//
//HAS_MEMBER_FUNCTION1(save)
//
//HAS_MEMBER_FUNCTION1(load)

struct has_serialize_impl{ \
template <class T, class U>\
static auto check(T*, U*)->decltype(std::declval<T>().serialize(std::declval<U&>()), std::true_type{});\
\
template <class T, class U>\
static auto check(...)->std::false_type;\
};\
\
template <class T, class U>\
struct has_serialize :\
	public decltype(has_serialize_impl::check<T, U>(nullptr, nullptr)) {};

struct has_save_impl {
	\
		template <class T, class U>\
		static auto check(T*, U*)->decltype(std::declval<T>().save(std::declval<U&>()), std::true_type{}); \
		\
		template <class T, class U>\
		static auto check(...)->std::false_type; \
}; \
\
template <class T, class U>\
struct has_save :\
	public decltype(has_save_impl::check<T, U>(nullptr, nullptr)) {};

struct has_load_impl {
	\
		template <class T, class U>\
		static auto check(T*, U*)->decltype(std::declval<T>().load(std::declval<U&>()), std::true_type{}); \
		\
		template <class T, class U>\
		static auto check(...)->std::false_type; \
}; \
	\
		template <class T, class U>\
		struct has_load :\
		public decltype(has_load_impl::check<T, U>(nullptr, nullptr)) {};
