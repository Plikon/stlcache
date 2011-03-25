#ifndef STLCACHE_STLCACHE_HPP_INCLUDED
#define STLCACHE_STLCACHE_HPP_INCLUDED

#include <map>

using namespace std;

#include "exceptions.hpp"
#include "policy.hpp"
#include "policy_lru.hpp"
#include "policy_mru.hpp"
#include "policy_lfu.hpp"

namespace stlcache {
    template<class Key, class Data, class Policy = policy_none<Key>, class Compare = less<Key>, class Allocator = allocator<pair<const Key, Data> > >
    class cache {
        typedef map<Key,Data,Compare,Allocator> storageType; 
         storageType _storage;
         std::size_t _maxEntries;
         std::size_t _currEntries;
         Policy _policy;


    public:
        typedef Key                                                                key_type;
        typedef Data                                                               mapped_type;
        typedef pair<const Key, Data>                                         value_type;
        typedef Compare                                                          key_compare;
        typedef Allocator                                                          allocator_type;
        typedef typename storageType::value_compare                                value_compare;
        typedef typename storageType::reference                                        reference;
        typedef typename storageType::const_reference                               const_reference;
        typedef typename storageType::size_type                                       size_type;
        typedef typename storageType::difference_type                               difference_type;
        typedef typename storageType::pointer                                          pointer;
        typedef typename storageType::const_pointer                                 const_pointer;

        //map functions wrappers
        allocator_type get_allocator() const throw() {
            return _storage.get_allocator();
        }

        size_type count ( const key_type& x ) const throw() {
            return _storage.count(x);
        }

        value_compare value_comp ( ) const throw() {
            return _storage.value_comp();
        }

        key_compare key_comp ( ) const throw() {
            return _storage.key_comp();
        }

        //Cache API
        void clear() throw() {
            _storage.clear();
            _policy.clear();
            this->_currEntries=0;
        }

        void swap ( cache<Key,Data,Policy,Compare,Allocator>& mp ) throw() {
            _storage.swap(mp._storage);
            _policy.swap(mp._policy);

            std::size_t m=this->_maxEntries;
            this->_maxEntries=mp._maxEntries;
            mp._maxEntries=m;

            this->_currEntries=this->size();
            mp._currEntries=mp.size();
        }

        size_type erase ( const key_type& x ) throw() {
            _policy.remove(x);

            _currEntries--;

            return _storage.erase(x);
        }

        bool insert(Key _k, Data _d) throw() {
            while (this->_currEntries >= this->_maxEntries) {
                this->erase(_policy.victim());
            }

            _policy.insert(_k);


            bool result=_storage.insert(value_type(_k,_d)).second;
            if (result) {
                _currEntries++;
            }

            return result;
        }

        size_type max_size() const throw() {
            return this->_maxEntries;
        }

        size_type size() const throw() {
            assert(this->_currEntries==_storage.size());
            return this->_currEntries;
        }

        bool empty() const throw() {
            return _storage.empty();
        }

        const Data& fetch(const Key& _k) throw(stlcache_invalid_key) {
            if (!check(_k)) {
                throw stlcache_invalid_key("Key is not in cache",_k);
            }
            _policy.touch(_k);
            return (*(_storage.find(_k))).second;
        }

        const bool check(const Key& _k) throw() {
            _policy.touch(_k);
            return _storage.find(_k)!=_storage.end();
        }

        void touch(const Key& _k) throw() {
            _policy.touch(_k);
        }

        //Container interface
        cache<Key,Data,Policy,Compare,Allocator>& operator= ( const cache<Key,Data,Policy,Compare,Allocator>& x) throw() {
            this->_storage=x._storage;
            this->_maxEntries=x._maxEntries;
            this->_currEntries=this->_storage.size();
            return *this;
        }
        explicit cache(const size_type size, const Compare& comp = Compare(), const Allocator& alloc = Allocator()) throw() {
            this->_storage=storageType(comp,alloc);
            this->_maxEntries=size;
            this->_currEntries=0;
        }
        cache(const cache<Key,Data,Policy,Compare,Allocator>& x) throw() {
            *this=x;
        }
    };
}

#endif /* STLCACHE_STLCACHE_HPP_INCLUDED */