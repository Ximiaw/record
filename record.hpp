// Copyright (c) 2026 Ximiaw
// SPDX-License-Identifier: MIT
#include<concepts>
#include<type_traits>
#include<array>
#include<tuple>
#include<stdexcept>

template<typename... Types>
requires (!(std::is_reference_v<Types>||...))&&(std::default_initializable<Types>&&...)
class record{
    private:
    std::tuple<Types...> tuple;
    std::array<void*,sizeof...(Types)> array;
    template<typename T>
    static constexpr std::array<bool,sizeof...(Types)> is_type_at{
        std::is_same_v<T,Types>...
    };
    private:
    template<size_t... Is>
    void init(std::index_sequence<Is...>){
        ((array[Is]=&std::get<Is>(tuple)),...);
    };
    public:
    ~record()=default;
    record(const record<Types...>& other):tuple(other.tuple){
        init(std::index_sequence_for<Types...>{});
    };
    record(record<Types...>&& other):tuple(std::move(other.tuple)){
        init(std::index_sequence_for<Types...>{});
    };
    record<Types...>& operator=(const record<Types...>& other){
        if(this==&other) return *this;
        tuple=other.tuple;
        return *this;
    };
    record<Types...>& operator=(record<Types...>&& other){
        if(this==&other) return *this;
        tuple=std::move(other.tuple);
        return *this;
    };
    public:
    template<size_t index>
    using type=std::tuple_element_t<index,std::tuple<Types...>>;
    explicit record():tuple(Types{}...){
        init(std::index_sequence_for<Types...>{});
    };
    template<typename... Args> 
    requires (std::convertible_to<Args,Types>&&...)
    explicit record(Args&&... args):tuple(std::forward<Args>(args)...){
        init(std::index_sequence_for<Args...>{});
    };
    template<typename T>
    const T& get(size_t i) const{
        if (i>=sizeof...(Types)) throw std::out_of_range("overstep the boundary");
        if (!is_type_at<T>[i]) throw std::runtime_error("type error");
        return *static_cast<const T*>(array[i]);
    };
    template<typename T>
    T& get(size_t i){
        if (i>=sizeof...(Types)) throw std::out_of_range("overstep the boundary");
        if (!is_type_at<T>[i]) throw std::runtime_error("type error");
        return *static_cast<T*>(array[i]);;
    };
    size_t size() const{
        return sizeof...(Types);
    };
};