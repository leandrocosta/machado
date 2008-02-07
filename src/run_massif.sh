valgrind \
  --tool=massif --depth=15 \
  --alloc-fn='std::_Deque_base<Object*, std::allocator<Object*> >::_M_create_nodes(Object***, Object***)' \
  --alloc-fn='std::_Deque_base<Object*, std::allocator<Object*> >::_M_initialize_map(unsigned)' \
  --alloc-fn='std::deque<Object*, std::allocator<Object*> >::_M_push_back_aux(Object* const&)'  \
  --alloc-fn='std::string::_Rep::_S_create(unsigned, unsigned, std::allocator<char> const&)'  \
  --alloc-fn='std::string::_Rep::_M_clone(std::allocator<char> const&, unsigned)' \
  --alloc-fn='std::string::reserve(unsigned)' \
  --alloc-fn='std::string::append(char const*, unsigned)' \
  --alloc-fn='std::basic_istream<char, std::char_traits<char> >& std::getline<char, std::char_traits<char>, std::allocator<char> >(std::basic_istream<char, std::char_traits<char> >&, std::basic_string<char, std::char_traits<char>, std::allocator<char> >&, char)' \
  --alloc-fn='ObjectList::ObjectList(unsigned long long const&)'  \
  --alloc-fn='ObjectList::PushBack(Object*)' \
  ./classifier -i ../data/adrianov/anneal.ac.training -t ../data/adrianov/anneal.ac.testing.5 -s .99 -c .99 -a 3
