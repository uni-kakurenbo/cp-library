#pragma once


#include <functional>
#include <stdexcept>

#include <ext/pb_ds/assoc_container.hpp>



namespace uni {

namespace gnu {


template<class Base>
struct hash_table : Base {
    using key_type = typename Base::key_type;
    using value_type = typename Base::value_type;
    using mapped_type = typename Base::mapped_type;

    inline bool contains(const key_type& key) const noexcept(NO_EXCEPT) {
        return this->Base::find(key) != this->Base::end();
    }

    template<class K, class T>
    inline decltype(auto) emplace(K&& key, T&& val) noexcept(NO_EXCEPT) {
        return this->Base::insert({ std::forward<K>(key), std::forward<T>(val) });
    }

    mapped_type& at(const key_type& key) {
        auto itr = this->Base::find(key);
        if(itr == this->Base::end()) throw std::out_of_range("hash_table::at()");
        return itr->second;
    };

    const mapped_type& at(const key_type & key) const {
        auto itr = this->Base::find(key);
        if(itr == this->Base::end()) throw std::out_of_range("hash_table::at()");
        return itr->second;
    };
};


template<class Key, class T, class Hash = void>
struct cc_hash_table : hash_table<__gnu_pbds::cc_hash_table<Key, T, Hash>> {
    using hash_table<__gnu_pbds::cc_hash_table<Key, T, Hash>>::hash_table;
};

template<class Key, class T>
struct cc_hash_table<Key, T, void> : hash_table<__gnu_pbds::cc_hash_table<Key, T>> {
    using hash_table<__gnu_pbds::cc_hash_table<Key, T>>::hash_table;
};


template<class Key, class T, class Hash = void>
struct gp_hash_table : hash_table<__gnu_pbds::gp_hash_table<Key, T, Hash>> {
    using hash_table<__gnu_pbds::gp_hash_table<Key, T, Hash>>::hash_table;
};

template<class Key, class T>
struct gp_hash_table<Key, T, void> : hash_table<__gnu_pbds::gp_hash_table<Key, T>> {
    using hash_table<__gnu_pbds::gp_hash_table<Key, T>>::hash_table;
};


} // namespace gnu

} // namespace uni
