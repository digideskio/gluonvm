#pragma once

#include "g_defs.h"
#include <map>

namespace gluon {

namespace containers {

  template <typename Key, typename Value>
  class dict_range_mapping {
  public:
    using storage_t = std::map<Key, Value>;
    using iter_t = typename storage_t::iterator;

    iter_t pos;
    iter_t end;

    dict_range_mapping(const iter_t &b, const iter_t &e): pos(b), end(e) {}

    bool have() const { return pos != end; }
    iter_t current() { return pos; }
    void advance() { pos++; }
  };


  // TODO: std::enable_if magic with const/nonconst members?
  template <typename Key, typename Value>
  class c_dict_range_mapping {
  public:
    using storage_t = std::map<Key, Value>;
    using iter_t = typename storage_t::const_iterator;

    iter_t pos;
    iter_t end;
    c_dict_range_mapping(const iter_t &b, const iter_t &e): pos(b), end(e) {}

    bool have() const { return pos != end; }
    iter_t current() { return pos; }
    void advance() { pos++; }
  };


  // A map wrapper with mapping helper classes (to iterate)
  // TODO: Replace this with self-made structure
  template <typename Key, typename Value>
  class stl_map {
  private:
    using self_t = stl_map<Key, Value>;
    using storage_t = std::map<Key, Value>;
    storage_t map_;
  public:
    using iterator = typename storage_t::iterator;

    stl_map() = default;
    stl_map(const stl_map &) = delete;
    stl_map(stl_map &&) = default;
    stl_map &operator = (stl_map &&) = default;

    const Value &operator[] (const Key &k) const { return map_[k]; }
    Value &operator[] (const Key &k) { return map_[k]; }

    bool contains(const Key &k) const {
      auto i = map_.find(k);
      return i != map_.end();
    }

    // Returns pointer to result or nullptr
    Value *find_ptr(const Key &k) {
      auto i = map_.find(k);
      if (i == map_.end()) { return nullptr; }
      return &i->second;
    }
    // Returns const pointer to result or nullptr
    const Value *find_ptr(const Key &k) const {
      auto i = map_.find(k);
      if (i == map_.end()) { return nullptr; }
      return &i->second;
    }

    // Returns reference to result or a default value you provide
    Value &find_ref(const Key &k, Value &defa) {
      auto i = map_.find(k);
      if (i == map_.end()) { return defa; }
      return i->second;
    }
    // Returns const reference to result or a default value you provide
    const Value &find_ref(const Key &k, const Value &defa) const {
      auto i = map_.find(k);
      if (i == map_.end()) { return defa; }
      return i->second;
    }

    void erase(const Key &k) { map_.erase(k); }
    size_t size() const { return map_.size(); }

    using Mapping = dict_range_mapping<Key, Value>;
    using ConstMapping = c_dict_range_mapping<Key, Value>;

    Mapping all() {
      return Mapping(map_.begin(), map_.end());
    }
    ConstMapping all() const {
      return ConstMapping(map_.begin(), map_.end());
    }
  };

} // ns containers

// A dictionary
// Keys should have 'operator <'
template <typename K, typename V>
using Dict = containers::stl_map<K, V>;


} // ns gluon
